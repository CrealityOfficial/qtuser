#ifndef QTUSER_CORE_GLCOMPATIBILITY_1654748123930_H
#define QTUSER_CORE_GLCOMPATIBILITY_1654748123930_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QCoreApplication>

#define QT_USE_GLES 1

namespace qtuser_core
{
	struct GlCompatibility
	{
		Qt::ApplicationAttribute glAttribute = Qt::AA_UseDesktopOpenGL;  // Qt::AA_UseDesktopOpenGL
																		 // Qt::AA_UseOpenGLES
																		 // Qt::AA_UseSoftwareOpenGL
	};

	QTUSER_CORE_API GlCompatibility createCompatibility();
}

#endif // QTUSER_CORE_GLCOMPATIBILITY_1654748123930_H