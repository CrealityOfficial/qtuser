set(SRCS qtuserquick/interface.h
		 qtuserquick/main/entry.h
		 qtuserquick/main/entry.cpp
		 qtuserquick/main/quicknativerenderitem.h
		 qtuserquick/main/quicknativerenderitem.cpp
		 )
		 
set(LIBS Qt5::Quick Qt5::Widgets qtuser_core)
set(DEFS QTUSER_QUICK_DLL)

__add_real_target(qtuser_quick dll SOURCE ${SRCS}
								   LIB ${LIBS}
								   DEF ${DEFS}
								   INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								   FOLDER qtuser
								   SOURCE_FOLDER
								   )