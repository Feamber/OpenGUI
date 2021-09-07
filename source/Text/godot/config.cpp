#define DLL_IMPLEMENTATION
#include "config.h"
#include "rid_owner.h"
#include "text_server_adv.h"
#include <cstdlib>

void memfree(void* p_ptr)
{
	std::free(p_ptr);
}

void* memalloc(size_t size)
{
	return std::malloc(size);
}

void* memrealloc(void* p_ptr, size_t size)
{
	return std::realloc(p_ptr, size);
}

SafeNumeric<uint64_t> RID_AllocBase::base_id{ 1 };

class TextServer* get_text_server()
{
    static TextServerAdvanced instance;
    return &instance;
}