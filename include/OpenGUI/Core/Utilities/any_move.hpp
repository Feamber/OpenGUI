#pragma once
#include <any>

namespace OGUI
{
    template<class T>
	std::remove_cv_t<T> any_move(std::any& any)
	{
		return std::any_cast<T>(std::move(any));
	}
}