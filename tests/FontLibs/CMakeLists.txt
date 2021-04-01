add_executable(FontLibsTest FontLibs.cpp FreeTypeLib.cpp)
target_link_libraries(FontLibsTest PRIVATE OpenGUI freetype harfbuzz icucommon)
catch_discover_tests(FontLibsTest)

download_and_extract(
    ${FILE_SERVER_DIR}/fonts/fireflysung.zip
    ${OPEN_GUI_ROOT_DIR}/build/PreBuilt/fireflysung.zip
    ${OPEN_GUI_ROOT_DIR}/build/Binaries/Debug/
)
extract_file(${OPEN_GUI_ROOT_DIR}/build/PreBuilt/fireflysung.zip ${OPEN_GUI_ROOT_DIR}/build/Binaries/Release/)