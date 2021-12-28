if(OPENGUI_USE_TRACY)
    extract_file(
        ${OPEN_GUI_ROOT_DIR}/resources/source/tracy-0.7.4.zip
        ${CMAKE_CURRENT_SOURCE_DIR}/tracy-0.7.4
    )

    extract_file(
        ${OPEN_GUI_ROOT_DIR}/resources/pre-built/${SAKURA_PLATFORM}/Common/tracy-0.7.4-app.zip
        ${OPEN_GUI_ROOT_DIR}/build/Binaries/Tools/
    )
    
    set(TRACY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tracy-0.7.4 CACHE STRING INTERNAL FORCE)
    set(TRACY_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/TracyClient.cpp CACHE STRING INTERNAL FORCE)
endif()
