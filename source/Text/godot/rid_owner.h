/*************************************************************************/
/*  rid_owner.h                                                          */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2021 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2021 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/
#pragma once
#include "error_macros.h"
#include "config.h"
#include "rid.h"

#include <stdio.h>
#include <typeinfo>
#include <atomic>

namespace godot{
template <class T>
class SafeNumeric {
	std::atomic<T> value;

	static_assert(std::atomic<T>::is_always_lock_free);

public:
	_ALWAYS_INLINE_ void set(T p_value) {
		value.store(p_value, std::memory_order_release);
	}

	_ALWAYS_INLINE_ T get() const {
		return value.load(std::memory_order_acquire);
	}

	_ALWAYS_INLINE_ T increment() {
		return value.fetch_add(1, std::memory_order_acq_rel) + 1;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postincrement() {
		return value.fetch_add(1, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T decrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel) - 1;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postdecrement() {
		return value.fetch_sub(1, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T add(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel) + p_value;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postadd(T p_value) {
		return value.fetch_add(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T sub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel) - p_value;
	}

	// Returns the original value instead of the new one
	_ALWAYS_INLINE_ T postsub(T p_value) {
		return value.fetch_sub(p_value, std::memory_order_acq_rel);
	}

	_ALWAYS_INLINE_ T exchange_if_greater(T p_value) {
		while (true) {
			T tmp = value.load(std::memory_order_acquire);
			if (tmp >= p_value) {
				return tmp; // already greater, or equal
			}
			if (value.compare_exchange_weak(tmp, p_value, std::memory_order_release)) {
				return p_value;
			}
		}
	}

	_ALWAYS_INLINE_ T conditional_increment() {
		while (true) {
			T c = value.load(std::memory_order_acquire);
			if (c == 0) {
				return 0;
			}
			if (value.compare_exchange_weak(c, c + 1, std::memory_order_release)) {
				return c + 1;
			}
		}
	}

	_ALWAYS_INLINE_ explicit SafeNumeric<T>(T p_value = static_cast<T>(0)) {
		set(p_value);
	}
};

class RID_AllocBase {
	static SafeNumeric<uint64_t> base_id;

protected:
	static RID _make_from_id(uint64_t p_id) {
		RID rid;
		rid._id = p_id;
		return rid;
	}

	static RID _gen_rid() {
		return _make_from_id(_gen_id());
	}

	friend struct VariantUtilityFunctions;

	static uint64_t _gen_id() {
		return base_id.increment();
	}

public:
	virtual ~RID_AllocBase() {}
};

class SpinLock {
	std::atomic_flag locked = ATOMIC_FLAG_INIT;

public:
	_ALWAYS_INLINE_ void lock() {
		while (locked.test_and_set(std::memory_order_acquire)) {
			;
		}
	}
	_ALWAYS_INLINE_ void unlock() {
		locked.clear(std::memory_order_release);
	}
};

template <class T, bool THREAD_SAFE = false>
class RID_Alloc : public RID_AllocBase {
	T **chunks = nullptr;
	uint32_t **free_list_chunks = nullptr;
	uint32_t **validator_chunks = nullptr;

	uint32_t elements_in_chunk;
	uint32_t max_alloc = 0;
	uint32_t alloc_count = 0;

	const char *description = nullptr;

	SpinLock spin_lock;

	_FORCE_INLINE_ RID _allocate_rid() {
		if (THREAD_SAFE) {
			spin_lock.lock();
		}

		if (alloc_count == max_alloc) {
			//allocate a new chunk
			uint32_t chunk_count = alloc_count == 0 ? 0 : (max_alloc / elements_in_chunk);

			//grow chunks
			chunks = (T **)memrealloc(chunks, sizeof(T *) * (chunk_count + 1));
			chunks[chunk_count] = (T *)memalloc(sizeof(T) * elements_in_chunk); //but don't initialize

			//grow validators
			validator_chunks = (uint32_t **)memrealloc(validator_chunks, sizeof(uint32_t *) * (chunk_count + 1));
			validator_chunks[chunk_count] = (uint32_t *)memalloc(sizeof(uint32_t) * elements_in_chunk);
			//grow free lists
			free_list_chunks = (uint32_t **)memrealloc(free_list_chunks, sizeof(uint32_t *) * (chunk_count + 1));
			free_list_chunks[chunk_count] = (uint32_t *)memalloc(sizeof(uint32_t) * elements_in_chunk);

			//initialize
			for (uint32_t i = 0; i < elements_in_chunk; i++) {
				// Don't initialize chunk.
				validator_chunks[chunk_count][i] = 0xFFFFFFFF;
				free_list_chunks[chunk_count][i] = alloc_count + i;
			}

			max_alloc += elements_in_chunk;
		}

		uint32_t free_index = free_list_chunks[alloc_count / elements_in_chunk][alloc_count % elements_in_chunk];

		uint32_t free_chunk = free_index / elements_in_chunk;
		uint32_t free_element = free_index % elements_in_chunk;

		uint32_t validator = (uint32_t)(_gen_id() & 0x7FFFFFFF);
		uint64_t id = validator;
		id <<= 32;
		id |= free_index;

		validator_chunks[free_chunk][free_element] = validator;

		validator_chunks[free_chunk][free_element] |= 0x80000000; //mark uninitialized bit

		alloc_count++;

		if (THREAD_SAFE) {
			spin_lock.unlock();
		}

		return _make_from_id(id);
	}

public:
	RID make_rid() {
		RID rid = _allocate_rid();
		initialize_rid(rid);
		return rid;
	}
	RID make_rid(const T &p_value) {
		RID rid = _allocate_rid();
		initialize_rid(rid, p_value);
		return rid;
	}

	//allocate but don't initialize, use initialize_rid afterwards
	RID allocate_rid() {
		return _allocate_rid();
	}

	_FORCE_INLINE_ T *getornull(const RID &p_rid, bool p_initialize = false) {
		if (p_rid == RID()) {
			return nullptr;
		}
		if (THREAD_SAFE) {
			spin_lock.lock();
		}

		uint64_t id = p_rid.get_id();
		uint32_t idx = uint32_t(id & 0xFFFFFFFF);
		if (unlikely(idx >= max_alloc)) {
			if (THREAD_SAFE) {
				spin_lock.unlock();
			}
			return nullptr;
		}

		uint32_t idx_chunk = idx / elements_in_chunk;
		uint32_t idx_element = idx % elements_in_chunk;

		uint32_t validator = uint32_t(id >> 32);

		if (unlikely(p_initialize)) {
			if (unlikely(!(validator_chunks[idx_chunk][idx_element] & 0x80000000))) {
				if (THREAD_SAFE) {
					spin_lock.unlock();
				}
				ERR_FAIL_V_MSG(nullptr, "Initializing already initialized RID");
			}

			if (unlikely((validator_chunks[idx_chunk][idx_element] & 0x7FFFFFFF) != validator)) {
				if (THREAD_SAFE) {
					spin_lock.unlock();
				}
				ERR_FAIL_V_MSG(nullptr, "Attempting to initialize the wrong RID");
				return nullptr;
			}

			validator_chunks[idx_chunk][idx_element] &= 0x7FFFFFFF; //initialized

		} else if (unlikely(validator_chunks[idx_chunk][idx_element] != validator)) {
			if (THREAD_SAFE) {
				spin_lock.unlock();
			}
			if ((validator_chunks[idx_chunk][idx_element] & 0x80000000) && validator_chunks[idx_chunk][idx_element] != 0xFFFFFFFF) {
				ERR_FAIL_V_MSG(nullptr, "Attempting to use an uninitialized RID");
			}
			return nullptr;
		}

		T *ptr = &chunks[idx_chunk][idx_element];

		if (THREAD_SAFE) {
			spin_lock.unlock();
		}

		return ptr;
	}
	void initialize_rid(RID p_rid) {
		T *mem = getornull(p_rid, true);
		ERR_FAIL_COND(!mem);
		memnew_placement(mem, T);
	}
	void initialize_rid(RID p_rid, const T &p_value) {
		T *mem = getornull(p_rid, true);
		ERR_FAIL_COND(!mem);
		memnew_placement(mem, T(p_value));
	}

	_FORCE_INLINE_ bool owns(const RID &p_rid) {
		if (THREAD_SAFE) {
			spin_lock.lock();
		}

		uint64_t id = p_rid.get_id();
		uint32_t idx = uint32_t(id & 0xFFFFFFFF);
		if (unlikely(idx >= max_alloc)) {
			if (THREAD_SAFE) {
				spin_lock.unlock();
			}
			return false;
		}

		uint32_t idx_chunk = idx / elements_in_chunk;
		uint32_t idx_element = idx % elements_in_chunk;

		uint32_t validator = uint32_t(id >> 32);

		bool owned = (validator_chunks[idx_chunk][idx_element] & 0x7FFFFFFF) == validator;

		if (THREAD_SAFE) {
			spin_lock.unlock();
		}

		return owned;
	}

	_FORCE_INLINE_ void free(const RID &p_rid) {
		if (THREAD_SAFE) {
			spin_lock.lock();
		}

		uint64_t id = p_rid.get_id();
		uint32_t idx = uint32_t(id & 0xFFFFFFFF);
		if (unlikely(idx >= max_alloc)) {
			if (THREAD_SAFE) {
				spin_lock.unlock();
			}
			ERR_FAIL();
		}

		uint32_t idx_chunk = idx / elements_in_chunk;
		uint32_t idx_element = idx % elements_in_chunk;

		uint32_t validator = uint32_t(id >> 32);
		if (unlikely(validator_chunks[idx_chunk][idx_element] & 0x80000000)) {
			if (THREAD_SAFE) {
				spin_lock.unlock();
			}
			ERR_FAIL_MSG("Attempted to free an uninitialized or invalid RID");
		} else if (unlikely(validator_chunks[idx_chunk][idx_element] != validator)) {
			if (THREAD_SAFE) {
				spin_lock.unlock();
			}
			ERR_FAIL();
		}

		chunks[idx_chunk][idx_element].~T();
		validator_chunks[idx_chunk][idx_element] = 0xFFFFFFFF; // go invalid

		alloc_count--;
		free_list_chunks[alloc_count / elements_in_chunk][alloc_count % elements_in_chunk] = idx;

		if (THREAD_SAFE) {
			spin_lock.unlock();
		}
	}

	_FORCE_INLINE_ uint32_t get_rid_count() const {
		return alloc_count;
	}

	_FORCE_INLINE_ T *get_ptr_by_index(uint32_t p_index) {
		ERR_FAIL_UNSIGNED_INDEX_V(p_index, alloc_count, nullptr);
		if (THREAD_SAFE) {
			spin_lock.lock();
		}
		uint64_t idx = free_list_chunks[p_index / elements_in_chunk][p_index % elements_in_chunk];
		T *ptr = &chunks[idx / elements_in_chunk][idx % elements_in_chunk];
		if (THREAD_SAFE) {
			spin_lock.unlock();
		}
		return ptr;
	}

	_FORCE_INLINE_ RID get_rid_by_index(uint32_t p_index) {
		ERR_FAIL_INDEX_V(p_index, alloc_count, RID());
		if (THREAD_SAFE) {
			spin_lock.lock();
		}
		uint64_t idx = free_list_chunks[p_index / elements_in_chunk][p_index % elements_in_chunk];
		uint64_t validator = validator_chunks[idx / elements_in_chunk][idx % elements_in_chunk];

		RID rid = _make_from_id((validator << 32) | idx);
		if (THREAD_SAFE) {
			spin_lock.unlock();
		}
		return rid;
	}

	void get_owned_list(List<RID> *p_owned) {
		if (THREAD_SAFE) {
			spin_lock.lock();
		}
		for (size_t i = 0; i < max_alloc; i++) {
			uint64_t validator = validator_chunks[i / elements_in_chunk][i % elements_in_chunk];
			if (validator != 0xFFFFFFFF) {
				p_owned->push_back(_make_from_id((validator << 32) | i));
			}
		}
		if (THREAD_SAFE) {
			spin_lock.unlock();
		}
	}

	void set_description(const char *p_descrption) {
		description = p_descrption;
	}

	RID_Alloc(uint32_t p_target_chunk_byte_size = 65536) {
		elements_in_chunk = sizeof(T) > p_target_chunk_byte_size ? 1 : (p_target_chunk_byte_size / sizeof(T));
	}

	~RID_Alloc() {
		if (alloc_count) {
			if (description) {
				//print_error("ERROR: " + itos(alloc_count) + " RID allocations of type '" + description + "' were leaked at exit.");
			} else {
#ifdef NO_SAFE_CAST
				print_error("ERROR: " + itos(alloc_count) + " RID allocations of type 'unknown' were leaked at exit.");
#else
				//print_error("ERROR: " + itos(alloc_count) + " RID allocations of type '" + typeid(T).name() + "' were leaked at exit.");
#endif
			}

			for (size_t i = 0; i < max_alloc; i++) {
				uint64_t validator = validator_chunks[i / elements_in_chunk][i % elements_in_chunk];
				if (validator & 0x80000000) {
					continue; //uninitialized
				}
				if (validator != 0xFFFFFFFF) {
					chunks[i / elements_in_chunk][i % elements_in_chunk].~T();
				}
			}
		}

		uint32_t chunk_count = max_alloc / elements_in_chunk;
		for (uint32_t i = 0; i < chunk_count; i++) {
			memfree(chunks[i]);
			memfree(validator_chunks[i]);
			memfree(free_list_chunks[i]);
		}

		if (chunks) {
			memfree(chunks);
			memfree(free_list_chunks);
			memfree(validator_chunks);
		}
	}
};

template <class T, bool THREAD_SAFE = false>
class RID_PtrOwner {
	RID_Alloc<T *, THREAD_SAFE> alloc;

public:
	_FORCE_INLINE_ RID make_rid(T *p_ptr) {
		return alloc.make_rid(p_ptr);
	}

	_FORCE_INLINE_ RID allocate_rid() {
		return alloc.allocate_rid();
	}

	_FORCE_INLINE_ void initialize_rid(RID p_rid, T *p_ptr) {
		alloc.initialize_rid(p_rid, p_ptr);
	}

	_FORCE_INLINE_ T *getornull(const RID &p_rid) {
		T **ptr = alloc.getornull(p_rid);
		if (unlikely(!ptr)) {
			return nullptr;
		}
		return *ptr;
	}

	_FORCE_INLINE_ void replace(const RID &p_rid, T *p_new_ptr) {
		T **ptr = alloc.getornull(p_rid);
		ERR_FAIL_COND(!ptr);
		*ptr = p_new_ptr;
	}

	_FORCE_INLINE_ bool owns(const RID &p_rid) {
		return alloc.owns(p_rid);
	}

	_FORCE_INLINE_ void free(const RID &p_rid) {
		alloc.free(p_rid);
	}

	_FORCE_INLINE_ uint32_t get_rid_count() const {
		return alloc.get_rid_count();
	}

	_FORCE_INLINE_ RID get_rid_by_index(uint32_t p_index) {
		return alloc.get_rid_by_index(p_index);
	}

	_FORCE_INLINE_ T *get_ptr_by_index(uint32_t p_index) {
		return *alloc.get_ptr_by_index(p_index);
	}

	_FORCE_INLINE_ void get_owned_list(List<RID> *p_owned) {
		return alloc.get_owned_list(p_owned);
	}

	void set_description(const char *p_descrption) {
		alloc.set_description(p_descrption);
	}

	RID_PtrOwner(uint32_t p_target_chunk_byte_size = 65536) :
			alloc(p_target_chunk_byte_size) {}
};

template <class T, bool THREAD_SAFE = false>
class RID_Owner {
	RID_Alloc<T, THREAD_SAFE> alloc;

public:
	_FORCE_INLINE_ RID make_rid() {
		return alloc.make_rid();
	}
	_FORCE_INLINE_ RID make_rid(const T &p_ptr) {
		return alloc.make_rid(p_ptr);
	}

	_FORCE_INLINE_ RID allocate_rid() {
		return alloc.allocate_rid();
	}

	_FORCE_INLINE_ void initialize_rid(RID p_rid) {
		alloc.initialize_rid(p_rid);
	}

	_FORCE_INLINE_ void initialize_rid(RID p_rid, const T &p_ptr) {
		alloc.initialize_rid(p_rid, p_ptr);
	}

	_FORCE_INLINE_ T *getornull(const RID &p_rid) {
		return alloc.getornull(p_rid);
	}

	_FORCE_INLINE_ bool owns(const RID &p_rid) {
		return alloc.owns(p_rid);
	}

	_FORCE_INLINE_ void free(const RID &p_rid) {
		alloc.free(p_rid);
	}

	_FORCE_INLINE_ uint32_t get_rid_count() const {
		return alloc.get_rid_count();
	}

	_FORCE_INLINE_ RID get_rid_by_index(uint32_t p_index) {
		return alloc.get_rid_by_index(p_index);
	}

	_FORCE_INLINE_ T *get_ptr_by_index(uint32_t p_index) {
		return alloc.get_ptr_by_index(p_index);
	}

	_FORCE_INLINE_ void get_owned_list(List<RID> *p_owned) {
		return alloc.get_owned_list(p_owned);
	}

	void set_description(const char *p_descrption) {
		alloc.set_description(p_descrption);
	}
	RID_Owner(uint32_t p_target_chunk_byte_size = 65536) :
			alloc(p_target_chunk_byte_size) {}
};
}