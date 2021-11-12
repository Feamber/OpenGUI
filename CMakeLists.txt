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

include(Configure) 
include(Download) 
set(FILE_SERVER_DIR "http://139.59.27.159//SaeruHikari")

# Build Options.
option(OPENGUI_USE_FREETYPE "Use FreeType as Text Rasterization Lib." ON)
option(OPENGUI_USE_HARFBUZZ "Use Harfbuzz as Text Shape Lib." ON)
option(OPENGUI_USE_ICU "Use libICU" ON)
option(OPENGUI_USE_TRACY "Use Tracy" ON)
option(OPENGUI_LIB "Build VM as Static Library." OFF)
option(OPENGUI_TESTS "Build Test Targets." ON)
option(OPENGUI_SAMPLES "Build Test Targets." ON)

add_subdirectory(thirdparty)

# Start Configure OpenGUI
file(GLOB_RECURSE sources source/*.cpp)
file(GLOB_RECURSE headers include/*.h)

if(OPENGUI_LIB)
    add_library(OpenGUI STATIC
        ${sources} 
        ${headers} 
    )
    target_compile_definitions(OpenGUI PUBLIC -DOGUI_BUILD_LIB)
else()
    add_library(OpenGUI SHARED ${sources} ${headers})
    message(STATUS "LibraryType: Shared Lib")
endif(OPENGUI_LIB)

if(OPENGUI_USE_TRACY)
    add_library(TracyD SHARED ${TRACY_SOURCE})
    target_include_directories(TracyD PUBLIC ${TRACY_INCLUDE_DIR})
    target_compile_definitions(OpenGUI PUBLIC -DUSE_TRACY)
    target_link_libraries(OpenGUI PUBLIC TracyD)
endif()

target_include_directories(OpenGUI 
    PUBLIC 
    include
    include/External
    include/External/yoga
    include/External/icu/common
    include/External/harfbuzz
    ${PLATFORM_INCLUDES}
    ${TRACY_INCLUDE_DIR}
)

target_link_libraries(OpenGUI 
    PUBLIC fmt freetype harfbuzz icucommon
)

if(OPENGUI_USE_FREETYPE OR OPENGUI_USE_HARFBUZZ OR OPENGUI_USE_ICU)
    download_and_extract(
        ${FILE_SERVER_DIR}/binaries/${OPENGUI_PLATFORM}/x64/release/FontLibs.zip
        ${OPEN_GUI_ROOT_DIR}/build/PreBuilt/Release/FontLibs.zip
        ${OPEN_GUI_ROOT_DIR}/build/Binaries/Release/
    )
    download_and_extract(
        ${FILE_SERVER_DIR}/binaries/${OPENGUI_PLATFORM}/x64/debug/FontLibs.zip
        ${OPEN_GUI_ROOT_DIR}/build/PreBuilt/Debug/FontLibs.zip
        ${OPEN_GUI_ROOT_DIR}/build/Binaries/Debug/
    )
endif()

# End Configure OpenGUI

        
link_libraries(OpenGUI)
add_subdirectory(extensions)
if(OPENGUI_SAMPLES)
    add_subdirectory(samples)
endif(OPENGUI_SAMPLES)

if(OPENGUI_TESTS)
    add_subdirectory(tests)
endif(OPENGUI_TESTS)