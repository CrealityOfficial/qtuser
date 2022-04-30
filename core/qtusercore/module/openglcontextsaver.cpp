#include "openglcontextsaver.h"
#include <QtCore/QDebug>

namespace qtuser_core
{
	OpenGLContextSaver::OpenGLContextSaver(QOpenGLContext* context)
		: m_context(context)
		, m_surface(context ? static_cast<QOffscreenSurface*>(context->surface()) : nullptr)
	{

	}

	OpenGLContextSaver::~OpenGLContextSaver()
	{
		if (m_context)
			m_context->makeCurrent(m_surface);
	}

	QOpenGLContext* OpenGLContextSaver::context()
	{
		return m_context;
	}

	QOffscreenSurface* OpenGLContextSaver::surface()
	{
		return m_surface;
	}

	RawOGL::RawOGL(QObject* parent)
		:QObject(parent)
		, m_renderContext(nullptr)
		, m_surface(nullptr)
	{
		m_renderContext = new QOpenGLContext(this);
		m_surface = new QOffscreenSurface(nullptr, this);
		m_surface->create();
	}

	RawOGL::~RawOGL()
	{
		qDebug() << "Raw OGL Ctr~";
	}

	void RawOGL::init(QOpenGLContext* context)
	{
		m_renderContext = context;
		m_surface->setFormat(context->format());

		initializeOpenGLFunctions();
	}

	QOpenGLContext* RawOGL::sharedContext()
	{
		return m_renderContext;
	}

	QOffscreenSurface* RawOGL::surface()
	{
		return m_surface;
	}
}