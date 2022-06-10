#include "glcompatibility.h"
#include <QtCore/QDebug>

namespace qtuser_core
{
	GlCompatibility createCompatibility()
	{
		GlCompatibility bility;
		
#if QT_USE_GLES
		bility.glAttribute = Qt::AA_UseOpenGLES;
#endif 
		return bility;
	}
}