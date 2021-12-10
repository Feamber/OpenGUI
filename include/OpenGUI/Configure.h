#pragma once
#include "assert.h"
#include "meta.h"

#ifdef __cplusplus 
#define BEGIN_EXTERN_C extern "C" {
#define END_EXTERN_C } // end extern "C"
#else
#define BEGIN_EXTERN_C 
#define END_EXTERN_C  
#endif

#ifdef __cplusplus
#ifndef OGUI_NULLPTR
#define OGUI_NULLPTR nullptr
#endif
#else
#ifndef OGUI_NULLPTR
#define OGUI_NULLPTR NULL
#endif
#endif

#if defined(_WIN32) || defined(_WIN64) || defined(_UNIX) || defined(__APPLE__)
#define OGUI_USE_DXMATH
#endif

#ifndef OGUI_MANUAL_CONFIG_CPU_ARCHITECTURE
    #if defined(__x86_64__) || defined(_M_X64) || defined(_AMD64_) || defined(_M_AMD64)
        #define OGUI_PLATFORM_X86_64
    #elif defined(__i386) || defined(_M_IX86) || defined(_X86_)
        #define OGUI_PLATFORM_X86
    #elif defined(__aarch64__) || defined(__AARCH64) || defined(_M_ARM64)
        #define OGUI_PLATFORM_ARM64
    #elif defined(__arm__) || defined(_M_ARM)
        #define OGUI_PLATFORM_ARM32
    #elif defined(__POWERPC64__) || defined(__powerpc64__)
        #define OGUI_PLATFORM_POWERPC64
    #elif defined(__POWERPC__) || defined(__powerpc__)
        #define OGUI_PLATFORM_POWERPC32
    #else
        #error Unrecognized CPU was used.
    #endif
#endif

#ifndef OGUI_MANUAL_CONFIG_CPU_TRAITS
    #if defined(__AVX__)
        #define OGUI_PLATFORM_AVX
    #endif
    #if defined(__AVX2__)
        #define OGUI_PLATFORM_AVX2
    #endif

    #if defined(OGUI_PLATFORM_X86)
        #define OGUI_PLATFORM_32BIT
        #define OGUI_PLATFORM_LITTLE_ENDIAN
        #define OGUI_PLATFORM_SSE
        #define OGUI_PLATFORM_SSE2
    #endif

    #if defined(OGUI_PLATFORM_X86_64)
        #define OGUI_PLATFORM_64BIT
        #define OGUI_PLATFORM_LITTLE_ENDIAN
        #define OGUI_PLATFORM_SSE
        #define OGUI_PLATFORM_SSE2
    #endif

    #if defined(OGUI_PLATFORM_ARM32)
        #define OGUI_PLATFORM_32BIT
        #define OGUI_PLATFORM_LITTLE_ENDIAN
    #endif

    #if defined(OGUI_PLATFORM_ARM64)
        #define OGUI_PLATFORM_64BIT
        #define OGUI_PLATFORM_LITTLE_ENDIAN
        #define OGUI_PLATFORM_SSE
        #define OGUI_PLATFORM_SSE2
    #endif

    #if defined(OGUI_PLATFORM_POWERPC32)
        #define OGUI_PLATFORM_32BIT
        #define OGUI_PLATFORM_BIG_ENDIAN
    #endif

    #if defined(OGUI_PLATFORM_POWERPC64)
        #define OGUI_PLATFORM_64BIT
        #define OGUI_PLATFORM_BIG_ENDIAN
    #endif
#endif

#ifndef OGUI_MANUAL_CONFIG_COMPILER
    #if defined(_MSC_VER)
        #define OGUI_COMPILER_MSVC
    #endif

    #if defined(__clang__)
        #define OGUI_COMPILER_CLANG
    #elif defined(__GNUC__)
        #define OGUI_COMPILER_GCC
    #elif defined(_MSC_VER)
    #else
        #error Unrecognized compiler was used.
    #endif
#endif

#ifndef OGUI_MANUAL_CONFIG_COMPILER_TRAITS
    #if defined(OGUI_COMPILER_MSVC)
        #define OGUI_COMPILER_VERSION _MSC_VER
    #elif defined(OGUI_COMPILER_CLANG)
        #define OGUI_COMPILER_VERSION (__clang_major__ * 100 + __clang_minor__)
    #elif defined(OGUI_COMPILER_GCC)
        #define OGUI_COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
    #endif
#endif

#ifndef OGUI_MANUAL_CONFIG_CPP_STANDARD
    #if (defined(OGUI_COMPILER_CLANG) || defined(OGUI_COMPILER_GCC))
		#if __cplusplus >= 201703L
			#define OGUI_COMPILER_CPP17
		#endif
		#if __cplusplus >= 201402L
			#define  OGUI_COMPILER_CPP14
		#endif
    #elif defined(OGUI_COMPILER_MSVC)
        #if (OGUI_COMPILER_VERSION >= 1920)    // VS 2019
            #define OGUI_COMPILER_CPP17
        #endif
		#if (OGUI_COMPILER_VERSION >= 1910)    // VS 2017
			#define OGUI_COMPILER_CPP14
		#endif
    #else
        #error "Failed to delect C++ standard version."
    #endif
#endif // OGUI_MANUAL_CONFIG_CPP_STANDARD_VERSION

// no vtable
#ifdef _MSC_VER
#define OGUI_NOVTABLE __declspec(novtable)
#else
#define OGUI_NOVTABLE
#endif

// inline defs
#ifndef OGUI_FORCEINLINE
#ifdef OGUI_COMPILER_MSVC
#define OGUI_FORCEINLINE __forceinline
#else
#define OGUI_FORCEINLINE inline
#endif
#endif
#define OGUI_INLINE inline
// By Default we use cpp-standard above 2011XXL
#define OGUI_NOEXCEPT noexcept

// export attribute for shared_lib
#ifdef OGUI_BUILD_LIB
#define OGUI_DLL_EXPORT 
#define OGUI_DLLEXPORT 
#define OGUI_DLLVISIBLE 
#define OGUI_DLLLOCAL 
#else
    #ifdef __EMSCRIPTEN__
    #include "emscripten.h"
    #define OGUI_DLLEXPORT EMSCRIPTEN_KEEPALIVE
    #define OGUI_DLL_EXPORT EMSCRIPTEN_KEEPALIVE
    #define OGUI_DLLVISIBLE EMSCRIPTEN_KEEPALIVE
    #define OGUI_DLLLOCAL __attribute__((visibility("hidden")))
    #define __stdcall 
    #elif defined(__GNUC__)
    #define OGUI_DLLEXPORT __attribute__((visibility("default")))
    #define OGUI_DLL_EXPORT __attribute__((visibility("default")))
    #define OGUI_DLLVISIBLE __attribute__((visibility("default")))
    #define OGUI_DLLLOCAL __attribute__((visibility("hidden")))
    #define __stdcall 
    #elif defined(OGUI_COMPILER_MSVC)
    #define OGUI_DLLEXPORT __declspec(dllexport)
    #define OGUI_DLL_EXPORT __declspec(dllexport)
        #ifdef OGUI_BUILD_DLL
        #define OGUI_DLLVISIBLE __declspec(dllexport)
        #else
        #define OGUI_DLLVISIBLE __declspec(dllimport)
        #endif
    #else
    #endif
#endif

#ifdef __cplusplus
#define OGUI_EXTERN_C extern "C"
#define OGUI_NULL nullptr
#else
#define OGUI_EXTERN_C
#define OGUI_NULL 0
#endif 

// define OGUI_BUILD_LIB in module's project config or API's source file, not in public domain.
#if defined(OGUI_BUILD_LIB)
#define OGUI_EXPORT 
#else
#define OGUI_EXPORT OGUI_DLL_EXPORT
#endif

#ifndef OGUI_API
#define OGUI_API OGUI_DLLVISIBLE
#endif

#ifndef OGUI_IL_FUNC
#define OGUI_IL_FUNC OGUI_FORCEINLINE
#endif

#ifndef OGUI_ZERO_LEN_ARRAY
#ifdef __GNUC__
#define OGUI_ZERO_LEN_ARRAY 0
#else
#define OGUI_ZERO_LEN_ARRAY 1
#endif
#endif



// error/fatal
#define OGUI_ERROR(node,...) printf_s(node, __VA_ARGS__)
#define OGUI_FATAL(node,...) printf_s(node, __VA_ARGS__)

#if defined(__MINGW32__)
# define OGUI_ISSUE_BREAK() DebugBreak();
#elif defined(_MSC_VER)
# define OGUI_ISSUE_BREAK() __debugbreak();
#elif defined(__powerpc64__)
# define OGUI_ISSUE_BREAK() asm volatile ("tw 31,1,1");
#elif defined(__i386__) || defined(__ia64__) || defined(__x86_64__)
# define OGUI_ISSUE_BREAK() asm("int $3");
#else
# define OGUI_ISSUE_BREAK() abort();
#endif
#ifndef NDEBUG
#ifndef OGUI_ASSERT_ENABLED
#define OGUI_ASSERT_ENABLED
#endif
#endif

#ifdef OGUI_ASSERT_ENABLED
#define OGUI_BREAK() \
  do { \
    OGUI_FATAL("BREAKPOINT HIT\n\tfile = %s\n\tline=%d\n", __FILE__, __LINE__); \
    OGUI_ISSUE_BREAK() \
  } while (false)

#define OGUI_ASSERT(cond) \
  do { \
    if (!(cond)) { \
      OGUI_FATAL("ASSERTION FAILED\n\tfile = %s\n\tline = %d\n\tcond = %s\n", __FILE__, __LINE__, #cond); \
      OGUI_ISSUE_BREAK() \
    } \
  } while (false)

#define OGUI_ASSERT_MSG(cond, ...) \
  do { \
    if (!(cond)) { \
	  OGUI_FATAL("ASSERTION FAILED\n\tfile = %s\n\tline = %d\n\tcond = %s\n\tmessage = ", __FILE__, __LINE__, #cond); \
      OGUI_FATAL(__VA_ARGS__); \
      OGUI_FATAL("\n"); \
      OGUI_ISSUE_BREAK(); \
    } \
  } while (false)

#define OGUI_ASSERT_CMD(cond, cmd) \
  do { \
    if (!(cond)) { \
      cmd; \
    } \
  } while (false)


#else
#define OGUI_BREAK()
#define OGUI_ASSERT(cond)
#define OGUI_ASSERT_MSG(cond, ...)
#define OGUI_ASSERT_CMD(cond, cmd)
#endif

#ifndef FORCEINLINE
    #ifdef _MSC_VER
        #define FORCEINLINE __forceinline
    #else
        #define FORCEINLINE inline
    #endif
#endif

#ifndef restrict
    #define RESTRICT __restrict
#endif
#include <stdint.h>

extern "C"
{
    typedef unsigned int uint;
    typedef unsigned char uchar;
    typedef char char8_t;
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;
    typedef uchar byte;

    typedef struct pos2d
    {
        uint32 x = 0;
        uint32 y = 0;
    } pos2d;
    typedef struct pos3d
    {
        uint32 x;
        uint32 y;
        uint32 z;
    } pos3d;
    typedef struct extent2d
    {
        uint32 width = 0;
        uint32 height = 0;
    } extent2d;
    typedef struct extent3d
    {
        uint32 width = 0;
        uint32 height = 0;
        uint32 depth = 0;
    } extent3d;
    typedef struct double4
    {
        union
        {
            struct { double r, g, b, a; }; //rgba view
            struct { double x, y, z, w; }; //xyzw view
        };
    } double4;
    typedef struct buffer
    {
        byte* data;
        uint64_t length;
    } buffer;
    typedef struct buffer_view
    { 
        byte* data;
        uint64_t length;
    } buffer_view;
}


#ifndef MAX_UINT8
#define MAX_UINT8  ((uint8)  ~((uint8) 0))
#endif
#ifndef MAX_UINT16
#define MAX_UINT16 ((uint16) ~((uint16)0))
#endif
#ifndef MAX_UINT32
#define MAX_UINT32 ((uint32) ~((uint32)0))
#endif
#ifndef MAX_UINT64
#define MAX_UINT64 ((uint64) ~((uint64)0))
#endif
#ifndef MAX_INT8
#define MAX_INT8   ((int8)  (MAX_UINT8  >> 1))
#endif
#ifndef MAX_INT16
#define MAX_INT16  ((int16) (MAX_UINT16 >> 1))
#endif
#ifndef MAX_INT32
#define MAX_INT32  ((int32) (MAX_UINT32 >> 1))
#endif
#ifndef MAX_INT64
#define MAX_INT64  ((int64) (MAX_UINT64 >> 1))
#endif
#ifndef MIN_INT8
#define MIN_INT8   ((int8)  ~MAX_INT8)
#endif
#ifndef MIN_INT16
#define MIN_INT16  ((int16) ~MAX_INT16)
#endif
#ifndef MIN_INT32
#define MIN_INT32  ((int32) ~MAX_INT32)
#endif
#ifndef MIN_INT64
#define MIN_INT64  ((int64) ~MAX_INT64)
#endif

#define OGUI_MAX(a, b) ((a) > (b) ? (a) : (b))



#ifdef __cplusplus
#include <cstddef>

#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

#ifdef _MSC_VER
    #pragma warning(disable:4251)//template dll interface
    #pragma warning(disable:5030)//unknown-attributes
    #pragma warning(disable:26812)//enum -> enum class

    #pragma warning(disable: 4244)
    #pragma warning(disable: 4267)
#endif

namespace OGUI
{
    typedef unsigned int uint;
    typedef unsigned char uchar;

    using char8_t = char;

    using uint8 = uint8_t;
    using uint8_t = uint8_t;
    using uint16 = uint16_t;
    using uint16_t = uint16_t;
    using uint32 = uint32_t;
    using uint32_t = uint32_t;
    using uint64 = uint64_t;
    using uint64_t = uint64_t;
    using int8 = int8_t;
    using int8_t = int8_t;
    using int16 = int16_t;
    using int16_t = int16_t;
    using int32 = int32_t;
    using int32_t = int32_t;
    using int64 = int64_t;
    using int64_t = int64_t;
    using size_t = std::size_t;
    using float32 = float;
    using float32_t = float;
    using float64 = double;
    using float64_t = double;
    using nullptr_t = std::nullptr_t;
    using pos2d = ::pos2d;
    using pos3d = ::pos3d;
    using extent2d = ::extent2d;
    using extent3d = ::extent3d;

	struct double4 : public ::double4
	{
        double4(double _x = 0, double _y = 0, double _z = 0, double _w = 0)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }
		constexpr double operator[](int index) const
		{
			switch (index)
			{
				case 0: return r;
				case 1: return g;
				case 2: return b;
				case 3: return a;
                default: return a;
			}
		}
	};

#ifndef KINDA_SMALL_NUMBER
#define KINDA_SMALL_NUMBER	(1.e-4)
#endif

#ifndef SMALL_NUMBER
#define SMALL_NUMBER		(1.e-8)
#endif

#ifndef THRESH_VECTOR_NORMALIZED
#define THRESH_VECTOR_NORMALIZED 0.01
#endif
}

template<typename To, typename From>
FORCEINLINE To force_cast(From f) 
{
    return (To)f;
}

#define ogui_constexpr constexpr
#define ogui_noexcept noexcept


#define ENUM_CLASS_FLAGS(Enum) \
inline           Enum& operator|=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
inline           Enum& operator&=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
inline           Enum& operator^=(Enum& Lhs, Enum Rhs) { return Lhs = (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
inline constexpr Enum  operator| (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs | (__underlying_type(Enum))Rhs); } \
inline constexpr Enum  operator& (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs & (__underlying_type(Enum))Rhs); } \
inline constexpr Enum  operator^ (Enum  Lhs, Enum Rhs) { return (Enum)((__underlying_type(Enum))Lhs ^ (__underlying_type(Enum))Rhs); } \
inline constexpr bool  operator! (Enum  E)             { return !(__underlying_type(Enum))E; } \
inline constexpr Enum  operator~ (Enum  E)             { return (Enum)~(__underlying_type(Enum))E; }

#endif

template<class T>
bool HasFlag(const T& a, const T& b)
{
    return (a&b) == b;
}