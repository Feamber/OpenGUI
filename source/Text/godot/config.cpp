#define DLL_IMPLEMENTATION
#include "OpenGUI/Context.h"
#include "config.h"
#include "rid_owner.h"
#include "text_server_adv.h"
#include <cstdlib>

namespace godot
{
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
    return OGUI::Context::Get().GetTextServer();
}
}