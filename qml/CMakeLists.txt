__tree_add_current(SRCS)
qt5_add_resources(RCC_SRC qtuserqml.qrc)

set(ILIBS Qt5::Widgets Qt5::Quick Qt5::3DExtras Qt5::3DInput Qt5::3DLogic Qt5::3DRenderPrivate qtuser_core qtuser_3d qtuser_quick)
set(DEFS QTUSER_QML_DLL)
set(INCS)

__add_real_target(qtuser_qml dll SOURCE ${SRCS} ${RCC_SRC} 
								 INC ${INCS}
								 ILIB ${ILIBS}
								 DEF ${DEFS}
								 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								 )

if(CC_INSTALL_QTUSER)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/ qtuser_qml)
endif()
