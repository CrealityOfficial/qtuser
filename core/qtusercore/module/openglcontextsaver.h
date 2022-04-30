#ifndef QTUSER_CORE_OPENGLCONTEXTSAVER_1651303374025_H
#define QTUSER_CORE_OPENGLCONTEXTSAVER_1651303374025_H
#include "qtusercore/qtusercoreexport.h"
#include <QtGui/QOpenGLContext>
#include <QtGui/QOffscreenSurface>

namespace qtuser_core
{
	class QTUSER_CORE_API OpenGLContextSaver
	{
	public:
		OpenGLContextSaver(QOpenGLContext* context = QOpenGLContext::currentContext());
		~OpenGLContextSaver();

		QOpenGLContext* context();
		QOffscreenSurface* surface();
	private:
		QOpenGLContext* m_context;
		QOffscreenSurface* m_surface;
	};
}

#endif // QTUSER_CORE_OPENGLCONTEXTSAVER_1651303374025_H