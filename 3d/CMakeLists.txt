__files_group(${CMAKE_CURRENT_SOURCE_DIR}/qtuser3d/ SRCS)

qt5_add_resources(RCC_SRC 3dresources.qrc)
list(APPEND SRCS ${RCC_SRC})

set(LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::3DRender Qt5::3DExtras qtuser_core)
set(DEFS QTUSER_3D_DLL)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/../core/)

__add_real_target(qtuser_3d dll SOURCE ${SRCS} INC ${INCS} LIB ${LIBS} DEF ${DEFS})
set_property(TARGET qtuser_3d PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

if(INSTALL_QTUSER)
	INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qtuser3d DESTINATION include/ FILES_MATCHING PATTERN "*.h")
endif()

