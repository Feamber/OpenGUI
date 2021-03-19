#pragma once
#include "configure.h"

namespace OGUI
{
	template<typename Storage, uint FractionBits>
    struct fixed
    {
	private:
        Storage storage;
    };
    using fixed64 = fixed<OGUI::int64, 32>;
    using fixed32 = fixed<OGUI::int32, 16>;
}