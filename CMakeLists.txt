cmake_minimum_required (VERSION 3.3)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/") 
if(UNIX)
    set(PLATFORM_INCLUDES ${PLATFORM_INCLUDES} include/Platform/Unix)
    set(PLATFORM_INCLUDES ${PLATFORM_INCLUDES} include/Platform/DirectXMath)
    add_definitions(-D "OPENGUI_TARGET_PLATFORM_UNIX")
    if(APPLE)
        add_definitions(-D "OPENGUI_TARGET_PLATFORM_MACOS")
        set(OPENGUI_PLATFORM "mac")
    elseif(${CMAKE_SYSTEM_NAME} MATCHES ANDROID)
        add_definitions(-D "OPENGUI_TARGET_PLATFORM_ANDROID")
        set(OPENGUI_PLATFORM "android")
        set(ANDROID 1)
    elseif(${CMAKE_SYSTEM_NAME} MATCHES Emscripten)
        add_definitions(-D "OPENGUI_TARGET_PLATFORM_EMSCRIPTEN")
        message(STATUS "Platform Web")
        set(OPENGUI_PLATFORM "web")
        set(WA 1)   #Web
    else(APPLE)
        add_definitions(-D "OPENGUI_TARGET_PLATFORM_LINUX")
        set(OPENGUI_PLATFORM "linux")
    endif(APPLE)
elseif(WIN32)
    add_definitions(-D "OPENGUI_TARGET_PLATFORM_WIN")
    set(OPENGUI_PLATFORM "windows")
    message(STATUS "Platform WIN")
elseif(__COMPILER_PS5)
    message(STATUS "Platform PS5")
    set(OPENGUI_PLATFORM "prospero")
    add_definitions(-D "OPENGUI_TARGET_PLATFORM_PLAYSTATION")
endif(UNIX)

set(OPEN_GUI_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(OPENGUI_BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/Binaries) 
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OPENGUI_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OPENGUI_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OPENGUI_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)
link_directories(${OPENGUI_BIN_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>$<$<CONFIG:MinSizeRel>:Release>)

project(OpenGUI)
add_subdirectory(thirdparty)
include(Download) 
set(FILE_SERVER_DIR "http://139.59.27.159//SaeruHikari")

# Build Options.
option(OPENGUI_LIB "Build VM as Static Library." ON)
option(OPENGUI_TESTS "Build Test Targets." ON)
option(OPENGUI_SAMPLES "Build Test Targets." ON)

file(GLOB_RECURSE sources source/*.cpp)
file(GLOB_RECURSE headers include/*.h)

if(OPENGUI_LIB)
add_library(OpenGUI 
    ${sources} 
    ${headers} 
    #"thirdparty/function_ref/function_ref.hpp" 
    #"thirdparty/nameof/nameof.hpp" "thirdparty/magic_enum/magic_enum.hpp"
)
target_compile_definitions(OpenGUI PUBLIC -DOGUI_BUILD_LIB)
else()
add_library(OpenGUI SHARED ${sources} ${headers})
endif(OPENGUI_LIB)

include_directories(${XercesC_INCLUDE_DIR})

target_include_directories(OpenGUI 
    PUBLIC 
    include
    ${PLATFORM_INCLUDES}
)
target_link_libraries(OpenGUI PRIVATE Yoga)
target_link_libraries(OpenGUI INTERFACE hana)


### xerces-c
if(WIN32)
set(XercesC_INCLUDE_DIR "thirdparty/xerces-c/include")
set(XercesC_LIBRARY "thirdparty/xerces-c/lib/xerces-c_4D")
set(XercesC_VERSION "4.0.0")
find_package(XercesC REQUIRED)
target_link_libraries(OpenGUI PRIVATE ${XercesC})
endif(WIN32)

link_libraries(OpenGUI)
if(OPENGUI_SAMPLES)
    add_subdirectory(samples)
endif(OPENGUI_SAMPLES)

if(OPENGUI_TESTS)
    add_subdirectory(tests)
endif(OPENGUI_TESTS)

