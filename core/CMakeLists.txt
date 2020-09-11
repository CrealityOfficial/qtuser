__files_group(${CMAKE_CURRENT_SOURCE_DIR}/qtusercore/ SRCS)
set(LIBS Qt5::Core)
set(DEFS QTUSER_CORE_DLL)

__add_real_target(qtuser_core dll SOURCE ${SRCS} LIB ${LIBS} DEF ${DEFS})

INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/qtusercore DESTINATION include/ FILES_MATCHING PATTERN "*.h")