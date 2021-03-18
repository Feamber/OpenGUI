
file(GLOB_RECURSE sources yoga/*.cpp)
file(GLOB_RECURSE headers yoga/*.h)
add_library(Yoga ${sources} ${headers})
target_include_directories(Yoga PUBLIC ./)