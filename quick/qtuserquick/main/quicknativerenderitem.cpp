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
		m_item->_uninitialize();
		qDebug() << "NativeRenderSystemWrapper Ctr~. thread " << QThread::currentThreadId();
	}

	void render() Q_DECL_OVERRIDE
	{
		{
			qtuser_core::OpenGLContextSaver saver;
			if (m_item->_render())
				update();
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
	if (m_renderSystem)
		m_renderSystem->setQuickNativeUpdater(nullptr);
	m_renderSystem = renderSystem;
	if (m_renderSystem)
		m_renderSystem->setQuickNativeUpdater(this);
}

void QuickNativeRenderItem::handleWindowChanged(QQuickWindow* win)
{
	if (win)
	{
		m_ratio = win->devicePixelRatio();
		if (m_ratio < 0.01f)
			m_ratio = 1.0f;
		
		qDebug() << QString("QuickNativeRenderItem::handleWindowChanged ratio [%1]").arg(m_ratio);
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
		m_dispacher->_geometryChanged(size);
}

void QuickNativeRenderItem::mousePressEvent(QMouseEvent* event)
{
	if (m_dispacher)
	{
		QPoint pt = event->pos();
		pt.rx() *= m_ratio;
		pt.ry() *= m_ratio;
		event->setLocalPos(pt);

		m_dispacher->_mousePressEvent(event);
	}
	setFocus(true);
}

void QuickNativeRenderItem::mouseMoveEvent(QMouseEvent* event)
{
	if (m_dispacher)
	{
		QPoint pt = event->pos();
		pt.rx() *= m_ratio;
		pt.ry() *= m_ratio;
		event->setLocalPos(pt);

		m_dispacher->_mouseMoveEvent(event);
	}
}

void QuickNativeRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_dispacher)
	{
		QPoint pt = event->pos();
		pt.rx() *= m_ratio;
		pt.ry() *= m_ratio;
		event->setLocalPos(pt);

		m_dispacher->_mouseReleaseEvent(event);
	}
}

void QuickNativeRenderItem::wheelEvent(QWheelEvent* event)
{
	if (m_dispacher)
	{
		QWheelEvent newEvent(event->posF() * m_ratio, event->globalPosF() * m_ratio, event->delta() * m_ratio, event->buttons(), event->modifiers(), event->orientation());
		newEvent.setTimestamp(event->timestamp());
		m_dispacher->_wheelEvent(&newEvent);
	}
}

void QuickNativeRenderItem::hoverEnterEvent(QHoverEvent* event)
{
	if (m_dispacher)
	{
		QHoverEvent newEvent(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
		newEvent.setTimestamp(event->timestamp());
		m_dispacher->_hoverEnterEvent(&newEvent);
	}
}

void QuickNativeRenderItem::hoverMoveEvent(QHoverEvent* event)
{
	if (m_dispacher)
	{
		QHoverEvent newEvent(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
		newEvent.setTimestamp(event->timestamp());
		m_dispacher->_hoverMoveEvent(&newEvent);
	}
}

void QuickNativeRenderItem::hoverLeaveEvent(QHoverEvent* event)
{
	if (m_dispacher)
	{
		QHoverEvent newEvent(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
		newEvent.setTimestamp(event->timestamp());
		m_dispacher->_hoverLeaveEvent(&newEvent);
	}
}

void QuickNativeRenderItem::keyPressEvent(QKeyEvent* event)
{
	if(m_dispacher)
		m_dispacher->_keyPressEvent(event);
}

void QuickNativeRenderItem::keyReleaseEvent(QKeyEvent* event)
{
	if(m_dispacher)
		m_dispacher->_keyReleaseEvent(event);
}

void QuickNativeRenderItem::invokeUpdate()
{
	update();
}

void QuickNativeRenderItem::_initialize()
{
	if (m_renderSystem)
		m_renderSystem->initializeFromRenderThread();
}

void QuickNativeRenderItem::_uninitialize()
{
	if (m_renderSystem)
		m_renderSystem->unitializeFromRenderThread();
}

bool QuickNativeRenderItem::_render()
{
	if (m_renderSystem)
		return m_renderSystem->render();
	return false;
}

void QuickNativeRenderItem::_synchronize()
{
	if (m_renderSystem)
		m_renderSystem->synchronize();
}