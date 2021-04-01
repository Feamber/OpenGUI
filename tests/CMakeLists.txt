project(OpenGUITests)
enable_testing() 
include(CTest)
include(Catch)    

add_library(TestCore STATIC TestCore.cpp)
link_libraries(TestCore)

FILE(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ ${CMAKE_CURRENT_SOURCE_DIR}/*)
FOREACH(child ${children})
IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/${child})
ENDIF()
ENDFOREACH()