set(SRCS qtuserquick/interface.h
		 qtuserquick/main/entry.h
		 qtuserquick/main/entry.cpp
		 qtuserquick/main/quicknativerenderitem.h
		 qtuserquick/main/quicknativerenderitem.cpp
		 )
		 
set(ILIBS Qt5::Quick Qt5::Widgets qtuser_core Qt5::3DExtras Qt5::3DInput Qt5::3DLogic Qt5::3DRenderPrivate qtuser_core qtuser_3d)
set(DEFS QTUSER_QUICK_DLL)

__add_real_target(qtuser_quick dll SOURCE ${SRCS}
								   ILIB ${ILIBS}
								   DEF ${DEFS}
								   INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
								   FOLDER qtuser
								   SOURCE_FOLDER
								   )