__tree_add_current(SRCS)
qt5_add_resources(RCC_SRC qtuserqml.qrc)

set(LIBS Qt5::Widgets Qt5::Quick Qt5::3DExtras Qt5::3DInput Qt5::3DLogic Qt5::3DRenderPrivate qtuser_core qtuser_3d)
set(DEFS QTUSER_QML_DLL)
set(INCS ${CMAKE_CURRENT_SOURCE_DIR}/../core/ ${CMAKE_CURRENT_SOURCE_DIR}/../3d/)

__add_real_target(qtuser_qml dll SOURCE ${SRCS} ${RCC_SRC} INC ${INCS} LIB ${LIBS} DEF ${DEFS})
set_property(TARGET qtuser_qml PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})

if(INSTALL_QTUSER)
	INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qtuserqml DESTINATION include/ FILES_MATCHING PATTERN "*.h")
endif()
