#include "qtuserqml/gl/rawogl.h"
#include <QtGui/QOffscreenSurface>

namespace qtuser_qml
{
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