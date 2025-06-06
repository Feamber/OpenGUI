#pragma once
#ifdef OGUI_USE_DXMATH
#include "OpenGUI/Core/Math/DirectXMath/OGUIDXMathVector.h"
#else

#endif

namespace OGUI
{
namespace math
{
	template<typename T, size_t Dimension>
	FORCEINLINE Vector<T, Dimension> normalize(const Vector<T, Dimension>& vec, const T tolerance = SMALL_NUMBER)
	{
		const float SquareSum = vec.length_squared();
		if (SquareSum > tolerance)
		{
			const float Scale = math::rsqrt(SquareSum);
			return vec * Scale;
		}
		return Vector<T, Dimension>::vector_zero();
	}

	template<typename T>
	FORCEINLINE Vector<T, 3> cross_product(const Vector<T, 3> a, const Vector<T, 3> b)
	{
		return a ^ b;
	}

	template<typename T, size_t N>
	FORCEINLINE T dot_product(const Vector<T, N> a, const Vector<T, N> b)
	{
		T res = T(0);
		for (auto i = 0u; i < N; i++)
		{
			res += a.data_view()[i] * b.data_view()[i];
		}
		return res;
	}

	template<typename T, size_t N>
	FORCEINLINE T length(const Vector<T, N> vec)
	{
		return vec.length();
	}

	template<typename T, size_t N>
	FORCEINLINE Vector<T, N> add(const Vector<T, N> a, const Vector<T, N> b)
	{
		return a + b;
	}

	template<typename T, size_t N>
	FORCEINLINE Vector<T, N> subtract(const Vector<T, N> a, const Vector<T, N> b)
	{
		return a - b;
	}

	template<typename T, size_t N>
	FORCEINLINE T distance(const Vector<T, N> a, const Vector<T, N> b)
	{
		return length(subtract(a, b));
	}
}
}