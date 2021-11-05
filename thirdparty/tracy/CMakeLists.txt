if(OPENGUI_USE_TRACY)
    download_and_extract(
        ${FILE_SERVER_DIR}/code/tracy-0.7.4.zip
        ${OPEN_GUI_ROOT_DIR}/Build/Code/tracy.zip
        ${CMAKE_CURRENT_SOURCE_DIR}/tracy-0.7.4
    )

    download_and_extract(
        ${FILE_SERVER_DIR}/binaries/${SAKURA_PLATFORM}/${SAKURA_ARCH}/common/tracy-0.7.4-app.zip
        ${OPEN_GUI_ROOT_DIR}/Build/PreBuilt/Common/tracy-0.7.4-app.zip
        ${OPEN_GUI_ROOT_DIR}/build/Binaries/Tools/
    )
    
    set(TRACY_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tracy-0.7.4 CACHE STRING INTERNAL FORCE)
    set(TRACY_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/TracyClient.cpp CACHE STRING INTERNAL FORCE)
endif()
