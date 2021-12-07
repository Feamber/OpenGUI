project(lua_binding)

file(GLOB sources source/*.cpp)
add_library(lua_binding STATIC ${sources})
add_subdirectory(lua)
target_include_directories(lua_binding PUBLIC include)
target_link_libraries(lua_binding PUBLIC OpenGUI lua_static)