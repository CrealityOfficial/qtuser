__files_group(${CMAKE_CURRENT_SOURCE_DIR}/qtuser3d/ SRCS)

#qt5_add_resources(RCC_SRC 3dresources.qrc)
#list(APPEND SRCS ${RCC_SRC})
find_package(Qt5 COMPONENTS SerialPort  REQUIRED)
set(LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Xml Qt5::SerialPort Qt5::3DRender Qt5::3DExtras Qt5::3DRenderPrivate qtuser_core)
set(DEFS QTUSER_3D_DLL)

__add_real_target(qtuser_3d dll SOURCE ${SRCS}
								LIB ${LIBS}
								DEF ${DEFS}
								INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								)

if(INSTALL_QTUSER)
	INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qtuser3d DESTINATION include/ FILES_MATCHING PATTERN "*.h")
endif()

add_subdirectory(tools)
