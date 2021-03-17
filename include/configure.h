#pragma once

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
        #ifdef DLL_IMPLEMENTATION
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