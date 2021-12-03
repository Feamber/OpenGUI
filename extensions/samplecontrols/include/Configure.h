#pragma once
#include "OpenGUI/Configure.h"


// export attribute for shared_lib
#ifdef SAMPLECONTROLS_BUILD_LIB
#define SAMPLECONTROLS_DLL_EXPORT 
#define SAMPLECONTROLS_DLLEXPORT 
#define SAMPLECONTROLS_DLLVISIBLE 
#define SAMPLECONTROLS_DLLLOCAL 
#else
    #ifdef __EMSCRIPTEN__
    #include "emscripten.h"
    #define SAMPLECONTROLS_DLLEXPORT EMSCRIPTEN_KEEPALIVE
    #define SAMPLECONTROLS_DLL_EXPORT EMSCRIPTEN_KEEPALIVE
    #define SAMPLECONTROLS_DLLVISIBLE EMSCRIPTEN_KEEPALIVE
    #define SAMPLECONTROLS_DLLLOCAL __attribute__((visibility("hidden")))
    #define __stdcall 
    #elif defined(__GNUC__)
    #define SAMPLECONTROLS_DLLEXPORT __attribute__((visibility("default")))
    #define SAMPLECONTROLS_DLL_EXPORT __attribute__((visibility("default")))
    #define SAMPLECONTROLS_DLLVISIBLE __attribute__((visibility("default")))
    #define SAMPLECONTROLS_DLLLOCAL __attribute__((visibility("hidden")))
    #define __stdcall 
    #elif defined(OGUI_COMPILER_MSVC)
    #define SAMPLECONTROLS_DLLEXPORT __declspec(dllexport)
    #define SAMPLECONTROLS_DLL_EXPORT __declspec(dllexport)
        #ifdef SAMPLECONTROLS_BUILD_DLL
        #define SAMPLECONTROLS_DLLVISIBLE __declspec(dllexport)
        #else
        #define SAMPLECONTROLS_DLLVISIBLE __declspec(dllimport)
        #endif
    #else
    #endif
#endif

#define SAMPLECONTROLS_API SAMPLECONTROLS_DLLVISIBLE