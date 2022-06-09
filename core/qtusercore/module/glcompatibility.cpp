#include "glcompatibility.h"
#include <QtCore/QDebug>

namespace qtuser_core
{
	GlCompatibility createCompatibility()
	{
		GlCompatibility bility;
		
		if (QCoreApplication::testAttribute(Qt::AA_UseDesktopOpenGL))
			qDebug() << "GlCompatibility AA_UseDesktopOpenGL.";
		return bility;
	}
}