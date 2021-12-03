project(WebgpuSample)

file(GLOB_RECURSE sources source/*.cpp)
file(GLOB_RECURSE headers include/*.h)
add_library(SampleControls ${sources} ${headers})
target_include_directories(SampleControls PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
target_compile_definitions(SampleControls PRIVATE -DSAMPLECONTROLS_BUILD_DLL)

file(GLOB resources res/*.*)
file(COPY ${resources} DESTINATION ${OPEN_GUI_ROOT_DIR}/build/Binaries/Debug/res/samplecontrols)
file(COPY ${resources} DESTINATION ${OPEN_GUI_ROOT_DIR}/build/Binaries/Release/res/samplecontrols)