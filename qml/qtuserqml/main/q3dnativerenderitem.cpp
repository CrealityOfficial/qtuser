#include "q3dnativerenderitem.h"
#include "qtuserqml/macro.h"

#include "qtuser3d/utils/shaderprogrammanager.h"

#include <QtCore/QThread>

QTUSER_QML_REG(Q3DNativeRenderItem)
Q3DNativeRenderItem::Q3DNativeRenderItem(QQuickItem* parent)
	: QuickNativeRenderItem(parent)
{
	m_eventSubdivide = new qtuser_3d::EventSubdivide(this);
	m_q3dRenderSystem = new qtuser_3d::Q3DRenderSystem(this);

	m_q3dRenderSystem->registerResidentNode(SHADERROOT);

	setQuickNativeEventDispacher(this);
	setQuickNativeRenderSystem(m_q3dRenderSystem);

	qDebug() << "Q3DNativeRenderItem Ctr. thread " << QThread::currentThread();
}

Q3DNativeRenderItem::~Q3DNativeRenderItem()
{
	qDebug() << "Q3DNativeRenderItem Ctr. thread " << QThread::currentThread();
}

qtuser_3d::EventSubdivide* Q3DNativeRenderItem::eventSubdivide()
{
	return m_eventSubdivide;
}

qtuser_3d::Q3DRenderSystem* Q3DNativeRenderItem::renderSystem()
{
	return m_q3dRenderSystem;
}

void Q3DNativeRenderItem::uninitialize()
{
	qDebug() << "Q3DNativeRenderItem uninitialize " << QThread::currentThread();

	m_q3dRenderSystem->renderRenderGraph(nullptr);
	setQuickNativeEventDispacher(nullptr);

	m_q3dRenderSystem->unRegisterResidentNode(SHADERROOT);

	m_q3dRenderSystem->unRegisterAll();
	m_eventSubdivide->closeHandlers();
}

void Q3DNativeRenderItem::_geometryChanged(const QSize& size)
{
	m_eventSubdivide->geometryChanged(size);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::_mousePressEvent(QMouseEvent* event)
{
	m_eventSubdivide->mousePressEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::_mouseMoveEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseMoveEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::_mouseReleaseEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseReleaseEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::_wheelEvent(QWheelEvent* event)
{
	m_eventSubdivide->wheelEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::_hoverEnterEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverEnterEvent(event);
}

void Q3DNativeRenderItem::_hoverMoveEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverMoveEvent(event);
}

void Q3DNativeRenderItem::_hoverLeaveEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverLeaveEvent(event);
}

void Q3DNativeRenderItem::_keyPressEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyPressEvent(event);
}

void Q3DNativeRenderItem::_keyReleaseEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyReleaseEvent(event);
}