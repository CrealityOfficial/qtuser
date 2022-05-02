#include "quicknativerenderitem.h"
#include <QtQuick/QQuickWindow>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLFramebufferObjectFormat>
#include <QtCore/QDebug>
#include <QtCore/QThread>

#include <QSGSimpleTextureNode>
#include "qtuser3d/event/eventsubdivide.h"
#include "qtuser3d/event/eventhandlers.h"
#include "qtusercore/module/openglcontextsaver.h"

QTUSER_QUICK_REG(QuickNativeRenderItem)
class NativeRenderSystemWrapper : public QQuickFramebufferObject::Renderer
{
public:
	NativeRenderSystemWrapper(QuickNativeRenderItem* item)
		:m_item(item)
	{
		m_item->_initialize();
		qDebug() << "NativeRenderSystemWrapper Ctr. thread " << QThread::currentThreadId();
	}

	virtual ~NativeRenderSystemWrapper()
	{
		qDebug() << "NativeRenderSystemWrapper Ctr~. thread " << QThread::currentThreadId();
	}

	void render() Q_DECL_OVERRIDE
	{
		{
			qtuser_core::OpenGLContextSaver saver;
			m_item->_render();
			//qInfo() << "NativeRenderSystemWrapper render. thread " << QThread::currentThreadId();
		}
		m_item->window()->resetOpenGLState();
	}

	void synchronize(QQuickFramebufferObject* item) Q_DECL_OVERRIDE
	{
		//qInfo() << "NativeRenderSystemWrapper synchronize." << QThread::currentThreadId();
		m_item->_synchronize();
	}

	QOpenGLFramebufferObject* createFramebufferObject(const QSize& size) Q_DECL_OVERRIDE
	{
		QOpenGLFramebufferObjectFormat format;
		format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
		format.setSamples(4);
		return new QOpenGLFramebufferObject(size, format);
	}

protected:
	QuickNativeRenderItem* m_item;
};

QuickNativeRenderItem::QuickNativeRenderItem(QQuickItem* parent)
	: QQuickFramebufferObject(parent)
	, m_ratio(1.0f)
	, m_dispacher(nullptr)
	, m_renderSystem(nullptr)
{
	setFlag(ItemHasContents, true);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::AllButtons);
	forceActiveFocus(Qt::MouseFocusReason);

	connect(this, &QQuickItem::windowChanged, this, &QuickNativeRenderItem::handleWindowChanged);
	qDebug() << "QuickNativeRenderItem Ctr. thread " << QThread::currentThreadId();
}

QuickNativeRenderItem::~QuickNativeRenderItem()
{
	qDebug() << "QuickNativeRenderItem Ctr. thread " << QThread::currentThreadId();
}

void QuickNativeRenderItem::setQuickNativeEventDispacher(qtuser_core::QuickNativeEventDispacher* dispacher)
{
	m_dispacher = dispacher;
}

void QuickNativeRenderItem::setQuickNativeRenderSystem(qtuser_core::QuickNativeRenderSystem* renderSystem)
{
	if(m_renderSystem)
		disconnect(m_renderSystem, SIGNAL(signalUpdate()), this, SLOT(update()));
	m_renderSystem = renderSystem;
	if(m_renderSystem)
		connect(m_renderSystem, SIGNAL(signalUpdate()), this, SLOT(update()));
}

void QuickNativeRenderItem::handleWindowChanged(QQuickWindow* win)
{
	if (win)
	{
		m_ratio = win->devicePixelRatio();
		if (m_ratio < 0.01f)
			m_ratio = 1.0f;
		
		update();
	}
}

QQuickFramebufferObject::Renderer* QuickNativeRenderItem::createRenderer() const
{
	QQuickFramebufferObject::Renderer* render = new NativeRenderSystemWrapper((QuickNativeRenderItem*)this);
	return render;
}

QSGNode* QuickNativeRenderItem::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* nodeData)
{
	node = QQuickFramebufferObject::updatePaintNode(node, nodeData);
	QSGSimpleTextureNode* textureNode = static_cast<QSGSimpleTextureNode*>(node);
	if (textureNode)
		textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
	return node;
}

void QuickNativeRenderItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);

	QSize size = newGeometry.size().toSize();
	size *= m_ratio;

	if (m_renderSystem)
		m_renderSystem->updateContextSize(size);

	if (m_dispacher)
		m_dispacher->geometryChanged(size);
}

void QuickNativeRenderItem::mousePressEvent(QMouseEvent* event)
{
	if (m_dispacher)
	{
		QMouseEvent newEvent(event->type(), event->pos() * m_ratio, event->button(),
			event->buttons(), event->modifiers());
		m_dispacher->mousePressEvent(&newEvent);
	}
	setFocus(true);
}

void QuickNativeRenderItem::mouseMoveEvent(QMouseEvent* event)
{
	if (m_dispacher)
	{
		QMouseEvent newEvent(event->type(), event->pos() * m_ratio, event->button(),
			event->buttons(), event->modifiers());
		m_dispacher->mouseMoveEvent(&newEvent);
	}
}

void QuickNativeRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_dispacher)
	{
		QMouseEvent newEvent(event->type(), event->pos() * m_ratio, event->button(),
			event->buttons(), event->modifiers());
		m_dispacher->mouseReleaseEvent(&newEvent);
	}
}

void QuickNativeRenderItem::wheelEvent(QWheelEvent* event)
{
	if (m_dispacher)
	{
		QWheelEvent new_event(event->posF() * m_ratio, event->globalPosF() * m_ratio, event->delta() * m_ratio, event->buttons(), event->modifiers(), event->orientation());
		m_dispacher->wheelEvent(&new_event);
	}
}

void QuickNativeRenderItem::hoverEnterEvent(QHoverEvent* event)
{
	if (m_dispacher)
	{
		QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
		m_dispacher->hoverEnterEvent(&new_event);
	}
}

void QuickNativeRenderItem::hoverMoveEvent(QHoverEvent* event)
{
	if (m_dispacher)
	{
		QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
		m_dispacher->hoverMoveEvent(&new_event);
	}
}

void QuickNativeRenderItem::hoverLeaveEvent(QHoverEvent* event)
{
	if (m_dispacher)
	{
		QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
		m_dispacher->hoverLeaveEvent(&new_event);
	}
}

void QuickNativeRenderItem::keyPressEvent(QKeyEvent* event)
{
	if(m_dispacher)
		m_dispacher->keyPressEvent(event);
}

void QuickNativeRenderItem::keyReleaseEvent(QKeyEvent* event)
{
	if(m_dispacher)
		m_dispacher->keyReleaseEvent(event);
}

void QuickNativeRenderItem::_initialize()
{
	if (m_renderSystem)
		m_renderSystem->initializeFromRenderThread();
}

void QuickNativeRenderItem::_render()
{
	if (m_renderSystem)
		m_renderSystem->render();
}

void QuickNativeRenderItem::_synchronize()
{
	if (m_renderSystem)
		m_renderSystem->synchronize();
}