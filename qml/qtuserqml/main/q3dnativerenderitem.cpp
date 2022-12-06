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
	m_renderSystem = new qtuser_3d::Q3DRenderSystem(this);

	m_renderSystem->registerResidentNode(SHADERROOT);
	m_renderSystem->registerResidentNode(RENDERPASSROOT);
	m_renderSystem->registerResidentNode(EFFECTROOT);

	setQuickNativeEventDispacher(this);
	setQuickNativeRenderSystem(m_renderSystem);
}

Q3DNativeRenderItem::~Q3DNativeRenderItem()
{
}

qtuser_3d::EventSubdivide* Q3DNativeRenderItem::eventSubdivide()
{
	return m_eventSubdivide;
}

qtuser_3d::Q3DRenderSystem* Q3DNativeRenderItem::renderSystem()
{
	return m_renderSystem;
}

void Q3DNativeRenderItem::uninitialize()
{
	qDebug() << "Q3DNativeRenderItem uninitialize " << QThread::currentThread();

	m_renderSystem->renderRenderGraph(nullptr);
	setQuickNativeEventDispacher(nullptr);
	setQuickNativeRenderSystem(nullptr);

	m_renderSystem->unRegisterResidentNode(SHADERROOT);
	m_renderSystem->unRegisterResidentNode(RENDERPASSROOT);
	m_renderSystem->unRegisterResidentNode(EFFECTROOT);

	m_renderSystem->unRegisterAll();
	m_eventSubdivide->closeHandlers();
}

void Q3DNativeRenderItem::geometryChanged(const QSize& size)
{
	m_eventSubdivide->geometryChanged(size);
	m_renderSystem->requestUpdate();
}

void Q3DNativeRenderItem::mousePressEvent(QMouseEvent* event)
{
	m_eventSubdivide->mousePressEvent(event);
	m_renderSystem->requestUpdate();
}

void Q3DNativeRenderItem::mouseMoveEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseMoveEvent(event);
	m_renderSystem->requestUpdate();
}

void Q3DNativeRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseReleaseEvent(event);
	m_renderSystem->requestUpdate();
}

void Q3DNativeRenderItem::wheelEvent(QWheelEvent* event)
{
	m_eventSubdivide->wheelEvent(event);
	m_renderSystem->requestUpdate();
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