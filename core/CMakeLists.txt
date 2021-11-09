set(SRCS)
set(LIBS Qt5::Core Qt5::Gui Qt5::Widgets)
set(DEFS QTUSER_CORE_DLL)

include(auto.module.cmake)
include(module.module.cmake)
include(string.module.cmake)
include(util.module.cmake)

__add_real_target(qtuser_core dll SOURCE ${SRCS}
								  LIB ${LIBS}
								  DEF ${DEFS}
								  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								  FOLDER qtuser
								  SOURCE_FOLDER
								  )

if(INSTALL_QTUSER)
	INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qtusercore DESTINATION include/ FILES_MATCHING PATTERN "*.h")
endif()