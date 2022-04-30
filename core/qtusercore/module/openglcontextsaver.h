#ifndef QTUSER_CORE_OPENGLCONTEXTSAVER_1651303374025_H
#define QTUSER_CORE_OPENGLCONTEXTSAVER_1651303374025_H
#include "qtusercore/qtusercoreexport.h"
#include <QtGui/QOpenGLContext>
#include <QtGui/QOffscreenSurface>
#include <QtGui/QOpenGLExtraFunctions>

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

	class QTUSER_CORE_API RawOGL : public QObject, public QOpenGLExtraFunctions
	{
	public:
		RawOGL(QObject* parent = nullptr);
		virtual ~RawOGL();

		void init(QOpenGLContext* context);
		QOpenGLContext* sharedContext();
		QOffscreenSurface* surface();
	protected:
		QOpenGLContext* m_renderContext;
		QOffscreenSurface* m_surface;
	};
}

#endif // QTUSER_CORE_OPENGLCONTEXTSAVER_1651303374025_H