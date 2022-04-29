#include "quicknativerenderitem.h"

#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtGui/QOpenGLFramebufferObject>
#include <QtGui/QOpenGLFramebufferObjectFormat>

#include "ccglobal/quick/macro.h"

QTUSER_QUICK_REG(QuickNativeRenderItem)
class NativeFrameBufferObjectRenderer : public QQuickFramebufferObject::Renderer
{
public:
	NativeFrameBufferObjectRenderer(QuickNativeRenderItem* item)
		:QQuickFramebufferObject::Renderer()
		, m_item(item)
	{
		qDebug() << "NativeFrameBufferObjectRenderer Ctr. thread " << QThread::currentThreadId();
	}

	virtual ~NativeFrameBufferObjectRenderer()
	{
		qDebug() << "NativeFrameBufferObjectRenderer Ctr~, thread " << QThread::currentThreadId();
	}

	void render() Q_DECL_OVERRIDE
	{
		emit m_item->sigRender();
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
{
	setFlag(ItemHasContents, true);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::AllButtons);
	forceActiveFocus(Qt::MouseFocusReason);

	qDebug() << QString("QuickNativeRenderItem Ctr. thread ") << QThread::currentThreadId();
}

QuickNativeRenderItem::~QuickNativeRenderItem()
{
	qDebug() << QString("QuickNativeRenderItem Ctr~. thread ") << QThread::currentThreadId();
}

QQuickFramebufferObject::Renderer* QuickNativeRenderItem::createRenderer() const
{
	return new NativeFrameBufferObjectRenderer((QuickNativeRenderItem*)this);
}

QSGNode* QuickNativeRenderItem::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* nodeData)
{
	node = QQuickFramebufferObject::updatePaintNode(node, nodeData);
	//QSGSimpleTextureNode* textureNode = static_cast<QSGSimpleTextureNode*>(node);
	//if (textureNode)
	//	textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
	return node;
}

void QuickNativeRenderItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
}

void QuickNativeRenderItem::mousePressEvent(QMouseEvent* event)
{
}

void QuickNativeRenderItem::mouseMoveEvent(QMouseEvent* event)
{
}

void QuickNativeRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
}

void QuickNativeRenderItem::wheelEvent(QWheelEvent* event)
{
}

void QuickNativeRenderItem::hoverEnterEvent(QHoverEvent* event)
{
}

void QuickNativeRenderItem::hoverMoveEvent(QHoverEvent* event)
{
}

void QuickNativeRenderItem::hoverLeaveEvent(QHoverEvent* event)
{
}

void QuickNativeRenderItem::keyPressEvent(QKeyEvent* event)
{
}

void QuickNativeRenderItem::keyReleaseEvent(QKeyEvent* event)
{
}