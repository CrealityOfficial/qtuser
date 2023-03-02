#include "q3dnativerenderitem.h"
#include "qtuserqml/macro.h"

#include "qtuser3d/utils/shaderprogrammanager.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/renderpass/renderpassmanager.h"

#include <QtCore/QThread>

QTUSER_QML_REG(Q3DNativeRenderItem)
Q3DNativeRenderItem::Q3DNativeRenderItem(QQuickItem* parent)
	: QuickNativeRenderItem(parent)
{
	m_eventSubdivide = new qtuser_3d::EventSubdivide(this);
	m_q3dRenderSystem = new qtuser_3d::Q3DRenderSystem(this);

	m_q3dRenderSystem->registerResidentNode(SHADERROOT);
	m_q3dRenderSystem->registerResidentNode(RENDERPASSROOT);
	m_q3dRenderSystem->registerResidentNode(EFFECTROOT);

	setQuickNativeEventDispacher(this);
	setQuickNativeRenderSystem(m_q3dRenderSystem);

	qDebug() << "Q3DNativeRenderItem Ctr. thread " << QThread::currentThreadId();
}

Q3DNativeRenderItem::~Q3DNativeRenderItem()
{
	qDebug() << "Q3DNativeRenderItem Ctr. thread " << QThread::currentThreadId();
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
	m_q3dRenderSystem->unRegisterResidentNode(RENDERPASSROOT);
	m_q3dRenderSystem->unRegisterResidentNode(EFFECTROOT);

	m_q3dRenderSystem->unRegisterAll();
	m_eventSubdivide->closeHandlers();
}

void Q3DNativeRenderItem::geometryChanged(const QSize& size)
{
	m_eventSubdivide->geometryChanged(size);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::mousePressEvent(QMouseEvent* event)
{
	m_eventSubdivide->mousePressEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::mouseMoveEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseMoveEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseReleaseEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::wheelEvent(QWheelEvent* event)
{
	m_eventSubdivide->wheelEvent(event);
	m_q3dRenderSystem->requestUpdate();
}

void Q3DNativeRenderItem::hoverEnterEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverEnterEvent(event);
}

void Q3DNativeRenderItem::hoverMoveEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverMoveEvent(event);
}

void Q3DNativeRenderItem::hoverLeaveEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverLeaveEvent(event);
}

void Q3DNativeRenderItem::keyPressEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyPressEvent(event);
}

void Q3DNativeRenderItem::keyReleaseEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyReleaseEvent(event);
}