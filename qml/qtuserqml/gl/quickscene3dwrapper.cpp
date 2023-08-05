#include "quickscene3dwrapper.h"
#include "qtuser3d/framegraph/xrendergraph.h"

namespace qtuser_qml
{
	QuickScene3DWrapper::QuickScene3DWrapper(QObject* parent)
		: QObject(parent)
		, m_scene3D(nullptr)
	{
		m_root = new Qt3DCore::QEntity();
		m_renderSettings = new Qt3DRender::QRenderSettings(m_root);
		m_defaultFG = new Qt3DRender::QFrameGraphNode(m_root);

		m_root->addComponent(m_renderSettings);
		m_renderSettings->setActiveFrameGraph(m_defaultFG);

		m_eventSubdivide = new qtuser_3d::EventSubdivide(this);
	}

	QuickScene3DWrapper::~QuickScene3DWrapper()
	{
		m_eventSubdivide->closeHandlers();
	}

	void QuickScene3DWrapper::bindScene3D(QObject* scene3d)
	{
		m_scene3D = scene3d;
		if (m_scene3D)
		{
			QMetaObject::invokeMethod(m_scene3D, "setEntity", Qt::AutoConnection, Q_ARG(Qt3DCore::QEntity*, m_root));
		}
	}

	void QuickScene3DWrapper::renderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (isRenderRenderGraph(graph))
			return;

		qtuser_3d::RenderGraph* currentGraph = currentRenderGraph();
		if (currentGraph)
		{
			currentGraph->endRender();
			currentGraph->setEnabled(false);
			currentGraph->setParent((Qt3DCore::QNode*)nullptr);

			disconnect(currentGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));

			qtuser_3d::XRenderGraph* xRender = qobject_cast<qtuser_3d::XRenderGraph*>(currentGraph);
			if (xRender)
			{
				m_eventSubdivide->removeHoverEventHandler(xRender);
				m_eventSubdivide->removeKeyEventHandler(xRender);
				m_eventSubdivide->removeLeftMouseEventHandler(xRender);
				m_eventSubdivide->removeRightMouseEventHandler(xRender);
				m_eventSubdivide->removeMidMouseEventHandler(xRender);
				m_eventSubdivide->removeResizeEventHandler(xRender);
				m_eventSubdivide->removeWheelEventHandler(xRender);
			}
		}

		if (graph)
		{
			connect(graph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));

			graph->begineRender();
			//QSize itemSize = size().toSize();
			//if (m_ratio > 0)
			//{
			//	itemSize *= m_ratio;
			//}
			//graph->updateRenderSize(itemSize);
			graph->setEnabled(true);
			graph->setParent(m_renderSettings);

			qtuser_3d::XRenderGraph* xRender = qobject_cast<qtuser_3d::XRenderGraph*>(graph);
			if (xRender)
			{
				m_eventSubdivide->addHoverEventHandler(xRender);
				m_eventSubdivide->addKeyEventHandler(xRender);
				m_eventSubdivide->addLeftMouseEventHandler(xRender);
				m_eventSubdivide->addRightMouseEventHandler(xRender);
				m_eventSubdivide->addMidMouseEventHandler(xRender);
				m_eventSubdivide->addResizeEventHandler(xRender);
				m_eventSubdivide->addWheelEventHandler(xRender);
			}
		}

		m_renderSettings->setActiveFrameGraph(graph);
	}

	void QuickScene3DWrapper::registerRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (graph && (m_registerRenderGraph.indexOf(graph) == -1))
		{
			m_registerRenderGraph.push_back(graph);
		}
	}

	void QuickScene3DWrapper::unRegisterRenderGraph(qtuser_3d::RenderGraph* graph)
	{
	}

	void QuickScene3DWrapper::unRegisterAll()
	{
		for (qtuser_3d::RenderGraph* graph : m_registerRenderGraph)
			graph->setParent(m_renderSettings);
	}

	bool QuickScene3DWrapper::isRenderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		return graph && (currentRenderGraph() == graph);
	}

	qtuser_3d::RenderGraph* QuickScene3DWrapper::currentRenderGraph()
	{
		return qobject_cast<qtuser_3d::RenderGraph*>(m_renderSettings->activeFrameGraph());
	}

	qtuser_3d::EventSubdivide* QuickScene3DWrapper::eventSubdivide()
	{
		return m_eventSubdivide;
	}

	void QuickScene3DWrapper::requestUpdate()
	{
		if (m_scene3D)
		{
			QMetaObject::invokeMethod(m_scene3D, "requestUpdate", Qt::AutoConnection);
		}
	}

	void QuickScene3DWrapper::_geometry(int width, int height)
	{
		QSize size(width, height);
		m_eventSubdivide->geometryChanged(size);

		qtuser_3d::RenderGraph* graph = currentRenderGraph();
		if (graph)
			graph->updateRenderSize(size);

		requestUpdate();
	}

	//void GLQuickItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
	//{
	//	QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);
	//
	//	QSize size = newGeometry.size().toSize();
	//	if (m_ratio < 0.01)
	//	{
	//		m_ratio = 1;
	//	}
	//	size *= m_ratio;
	//	m_eventSubdivide->geometryChanged(size);
	//
	//	RenderGraph* graph = currentRenderGraph();
	//	if (graph)
	//		graph->updateRenderSize(size);
	//
	//	requestUpdate();
	//}
	//
	//void GLQuickItem::mousePressEvent(QMouseEvent* event)
	//{
	//	QPoint pt = event->pos();
	//	pt.rx() *= m_ratio;
	//	pt.ry() *= m_ratio;
	//	event->setLocalPos(pt);
	//	m_eventSubdivide->mousePressEvent(event);
	//	setFocus(true);
	//}
	//
	//void GLQuickItem::mouseMoveEvent(QMouseEvent* event)
	//{
	//	QPoint pt = event->pos();
	//	pt.rx() *= m_ratio;
	//	pt.ry() *= m_ratio;
	//	event->setLocalPos(pt);
	//	m_eventSubdivide->mouseMoveEvent(event);
	//}
	//
	//void GLQuickItem::mouseReleaseEvent(QMouseEvent* event)
	//{
	//	QPoint pt = event->pos();
	//	pt.rx() *= m_ratio;
	//	pt.ry() *= m_ratio;
	//	event->setLocalPos(pt);
	//	m_eventSubdivide->mouseReleaseEvent(event);
	//}
	//
	//void GLQuickItem::wheelEvent(QWheelEvent* event)
	//{
	//	QWheelEvent new_event(event->posF() * m_ratio, event->globalPosF(), event->delta() * m_ratio, event->buttons(), event->modifiers(), event->orientation());
	//	m_eventSubdivide->wheelEvent(&new_event);
	//}
	//
	//void GLQuickItem::hoverEnterEvent(QHoverEvent* event)
	//{
	//	QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
	//	m_eventSubdivide->hoverEnterEvent(&new_event);
	//}
	//
	//void GLQuickItem::hoverMoveEvent(QHoverEvent* event)
	//{
	//	QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
	//	m_eventSubdivide->hoverMoveEvent(&new_event);
	//}
	//
	//void GLQuickItem::hoverLeaveEvent(QHoverEvent* event)
	//{
	//	QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
	//	m_eventSubdivide->hoverLeaveEvent(&new_event);
	//}
}