__files_group(${CMAKE_CURRENT_SOURCE_DIR}/qtuser3d/ SRCS)

__cc_find(glew)
__assert_target(glew)
#qt5_add_resources(RCC_SRC 3dresources.qrc)
#list(APPEND SRCS ${RCC_SRC})
find_package(Qt5 COMPONENTS SerialPort  REQUIRED)
set(ILIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::OpenGL Qt5::Xml Qt5::SerialPort Qt5::3DRender Qt5::3DExtras Qt5::3DRenderPrivate Qt5::Concurrent qtuser_core glew )
set(DEFS QTUSER_3D_DLL)

__add_real_target(qtuser_3d dll SOURCE ${SRCS}
								ILIB ${ILIBS}
								DEF ${DEFS}
								INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								)

if(CC_INSTALL_QTUSER)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/ qtuser_3d)
endif()

if(CC_BC_WIN AND BUILD_SHADER_TOOLS)
	add_subdirectory(tools)
endif()
