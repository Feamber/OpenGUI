project(WebgpuSample)

find_package(Vulkan)
if(${Vulkan_FOUND})
    include_directories(${Vulkan_INCLUDE_DIRS})
	link_libraries(${Vulkan_LIBRARY})
    message(STATUS "Found VK SDK. Include Dir: ${Vulkan_INCLUDE_DIRS}")
    set(vulkan TRUE)
endif()
	
extract_file(
    ${OPEN_GUI_ROOT_DIR}/resources/pre-built/${OPENGUI_PLATFORM}/Release/dawn_0.0.2.zip
    ${OPEN_GUI_ROOT_DIR}/build/Binaries/Release/
)
extract_file(
    ${OPEN_GUI_ROOT_DIR}/resources/pre-built/${OPENGUI_PLATFORM}/Release/SDL2.zip
    ${OPEN_GUI_ROOT_DIR}/build/Binaries/Release/
)
extract_file(
	${OPEN_GUI_ROOT_DIR}/resources/pre-built/${OPENGUI_PLATFORM}/Debug/dawn_0.0.2.zip
    ${OPEN_GUI_ROOT_DIR}/build/Binaries/Debug/
)
extract_file(
	${OPEN_GUI_ROOT_DIR}/resources/pre-built/${OPENGUI_PLATFORM}/Debug/SDL2.zip
    ${OPEN_GUI_ROOT_DIR}/build/Binaries/Debug/
)

file(GLOB_RECURSE resources res/*.*)
file(COPY ${resources} DESTINATION ${OPEN_GUI_ROOT_DIR}/build/Binaries/Debug/res)
file(COPY ${resources} DESTINATION ${OPEN_GUI_ROOT_DIR}/build/Binaries/Release/res)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nullability-extension \
	-Wno-reserved-id-macro -Wno-unused-command-line-argument -Wno-c++98-compat \
	-Wno-unknown-argument -Wno-nonportable-include-path \
	-fno-exceptions")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -D_DEBUG=1 -Wno-unused -O0")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g0 -DNDEBUG=1 -flto -O3")
endif()
file(GLOB sources src/*.cpp)
file(GLOB_RECURSE headers src/*.h)

if (EMSCRIPTEN)
	file(GLOB_RECURSE platform_sources src/ems/*)
	set(CMAKE_EXECUTABLE_SUFFIX ".html")

	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s ASSERTIONS=2 -s DEMANGLE_SUPPORT=1 -s SAFE_HEAP=1 -s STACK_OVERFLOW_CHECK=2")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s ASSERTIONS=0 -s DISABLE_EXCEPTION_CATCHING=1 -s EVAL_CTORS=1 --closure 1")

	set(CMAKE_EXE_LINKER_FLAGS "-s WASM=1 -s USE_WEBGPU=1 -s NO_EXIT_RUNTIME=1 -s STRICT=1 --shell-file ${CMAKE_CURRENT_LIST_DIR}/src/ems/shell.html")

	# Linker flags to optimize for smallest output code size
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ENVIRONMENT=web -s MINIMAL_RUNTIME=2 -s TEXTDECODER=2 -s ABORTING_MALLOC=0 -s ALLOW_MEMORY_GROWTH=0 -s SUPPORT_ERRNO=0 -s MALLOC=emmalloc -s NO_FILESYSTEM=1 --output_eol=linux")
elseif (WIN32)
	file(GLOB_RECURSE platform_sources src/win/*)
elseif (APPLE)
	file(GLOB_RECURSE platform_sources src/mac/*)
	link_libraries(
		libdawn_platform.dylib 
		libdawn_proc.dylib 
		libdawn_native.dylib
	)
	set(CMAKE_EXE_LINKER_FLAGS 
		"${CMAKE_EXE_LINKER_FLAGS} \
		-framework Foundation \ 
		-framework AppKit \
		-framework CoreVideo \
		-framework QuartzCore \
		"
	)
endif()


add_executable(WebgpuSample ${sources} ${platform_sources} ${headers} ${lua_sources})

target_include_directories(WebgpuSample PRIVATE "${CMAKE_CURRENT_LIST_DIR}/include")
set_property(TARGET WebgpuSample PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/samples/webgpu")

target_link_libraries(WebgpuSample efsw)
target_link_libraries(WebgpuSample 
	debug SDL2d optimized SDL2
	debug SDL2maind optimized SDL2main
)

add_subdirectory(extern)
target_include_directories(WebgpuSample PRIVATE "${CMAKE_CURRENT_LIST_DIR}/extern")
target_link_libraries(WebgpuSample spdlog SampleControls lua_binding)