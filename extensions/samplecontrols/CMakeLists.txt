project(WebgpuSample)

file(GLOB_RECURSE sources source/*.cpp)
file(GLOB_RECURSE headers include/*.h)
add_library(SampleControls ${sources} ${headers})
target_include_directories(SampleControls PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")