set(SRCS)
set(ILIBS Qt5::Core Qt5::Gui Qt5::Widgets)
set(DEFS QTUSER_CORE_DLL)
set(UIS)

__cc_find(Quazip)
__cc_find(zlib)
__assert_target(quazip)
__assert_target(zlib)

set(LIBS quazip zlib)

include(auto.module.cmake)
include(module.module.cmake)
include(string.module.cmake)
include(util.module.cmake)
include(widget.module.cmake)

__add_real_target(qtuser_core dll SOURCE ${SRCS}
								  ILIB ${ILIBS}
								  LIB ${LIBS}
								  DEF ${DEFS}
								  QTUI ${UIS}
								  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								  FOLDER qtuser
								  SOURCE_FOLDER
								  )

if(CC_INSTALL_QTUSER)
	__install_directory_specif(${CMAKE_CURRENT_SOURCE_DIR}/ qtuser_core)
endif()