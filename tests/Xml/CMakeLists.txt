add_executable(XmlTest XmlParser.cpp)
target_link_libraries(XmlTest PUBLIC OpenGUI)
catch_discover_tests(XmlTest)

add_custom_command(TARGET XmlTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/Xml/XmlParserTest.xml"
        "${PROJECT_SOURCE_DIR}/Xml/XmlParserTest2.xml"
        "${PROJECT_SOURCE_DIR}/Xml/XmlParserTest3.xml"
        $<TARGET_FILE_DIR:XmlTest>)