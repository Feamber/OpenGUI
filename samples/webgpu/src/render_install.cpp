#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "utils.h"
#include "webgpu.h"

using namespace OGUI;

OGUI::PixelFormat CountToFormat(size_t n)
{
	switch (n) {
	case 1 :
		return PF_R8;
	case 2 :
		return PF_R8A8;
	case 4 :
		return PF_R8G8B8A8;
	}
	return PF_R8G8B8A8;
}

size_t FormatToCount(OGUI::PixelFormat f)
{
	switch (f) {
	case PF_R8 :
		return 1;
	case PF_R8A8 :
		return 2;
	case PF_R8G8B8A8 :
		return 4;
	default: 
		return 4;
	}
	return 4;
}

struct BitmapParser final : public OGUI::BitmapParserInterface
{
    inline Bitmap LoadFromFile(const FileHandle file)
    {
		Bitmap bm = {};
		int x, y, n;
		stbi_info_from_file((FILE*)file, &x, &y, &n);
		bm.format = CountToFormat(n);
		const auto channels = FormatToCount(bm.format);
		auto data = stbi_load_from_file((FILE*)file, &x, &y, &n, channels);
		bm.resource.bytes = data;
		bm.resource.size_in_bytes = x * y * channels * sizeof(*data);
		bm.height = y;
		bm.width = x;

		return bm;
    }
    inline virtual Bitmap LoadFromMemory(const void* buffer, size_t length)
    {
		Bitmap bm = {};
		int x, y, n;
		stbi_info_from_memory((const stbi_uc*)buffer, length, &x, &y, &n);
		bm.format = CountToFormat(n);
		const auto channels = FormatToCount(bm.format);
		auto data = stbi_load_from_memory((const stbi_uc*)buffer, length, &x, &y, &n, channels);
		bm.resource.bytes = data;
		bm.resource.size_in_bytes = x * y * channels * sizeof(*data);
		bm.height = y;
		bm.width = x;

		return bm;
    }
    inline void Free(Bitmap bm)
    {
        stbi_image_free(bm.resource.bytes);
    }
};

bool InstallBitmapParser()
{
    auto& ctx = OGUI::Context::Get();
	ctx.bmParserImpl = std::make_unique<BitmapParser>();
    return ctx.bmParserImpl.get();
} 