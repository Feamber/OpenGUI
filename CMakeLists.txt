cmake_minimum_required (VERSION 3.3)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Build Options.
option(OPENGUI_LIB "Build VM as Static Library." ON)
option(OPENGUI_TESTS "Build Test Targets." ON)

include_directories(include)

if(OPENGUI_TESTS)
    project(OpenGUITests)

    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/") 
    enable_testing() 
    include(CTest)
    include(Catch)                                                            

    add_library(TestCore tests/TestCore.cpp)
    link_libraries(TestCore)
    add_subdirectory(tests)
endif(OPENGUI_TESTS)