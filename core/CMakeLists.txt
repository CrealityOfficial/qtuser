__tree_add_current(SRCS)
set(LIBS Qt5::Core Qt5::Gui Qt5::Widgets Qt5::3DRender)
set(DEFS QTUSER_CORE_DLL)

__add_real_target(qtuser_core dll SOURCE ${SRCS} LIB ${LIBS} DEF ${DEFS})
set_property(TARGET qtuser_core PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
add_definitions(-DHAVE_CONFIG_H)

if(INSTALL_QTUSER)
	INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qtusercore DESTINATION include/ FILES_MATCHING PATTERN "*.h")
endif()