add_library(sol INTERFACE)
target_include_directories(sol INTERFACE include)
target_link_libraries(sol INTERFACE lua_static)