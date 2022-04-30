#include "quicknativerenderitem.h"
#include "qtusercore/module/openglcontextsaver.h"

#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QtCore/QDebug>
#include <QtCore/QThread>

#include <QSGSimpleTextureNode>
#include <Qt3DRender/private/qrenderaspect_p.h>
#include <QtQuick/QQuickWindow>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DInput/QInputAspect>
#include <Qt3DInput/QInputSettings>
#include <Qt3DLogic/QLogicAspect>

#include <Qt3DRender/QRenderSettings>

#include "qtuser3d/event/eventsubdivide.h"
#include "qtuser3d/event/eventhandlers.h"
#include "qtuser3d/framegraph/rendergraph.h"

#include "ccglobal/quick/macro.h"

QTUSER_QUICK_REG(QuickNativeRenderItem)
class NativeFrameBufferObjectRenderer : public QQuickFramebufferObject::Renderer
{
public:
	NativeFrameBufferObjectRenderer(QuickNativeRenderItem* item, Qt3DRender::QRenderAspect* renderAspect
		, Qt3DCore::QAspectEngine* aspectEngine)
		:m_renderAspect(renderAspect), m_item(item)
		, m_aspectEngine(aspectEngine)
	{
		static_cast<Qt3DRender::QRenderAspectPrivate*>(
			Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderInitialize(QOpenGLContext::currentContext());

		qDebug() << "NativeFrameBufferObjectRenderer Ctr. thread " << QThread::currentThreadId();
	}

	virtual ~NativeFrameBufferObjectRenderer()
	{
		qDebug() << "NativeFrameBufferObjectRenderer Ctr~. thread " << QThread::currentThreadId();
	}

	void render() Q_DECL_OVERRIDE
	{
		qtuser_core::OpenGLContextSaver saver;

		static_cast<Qt3DRender::QRenderAspectPrivate*>(
			Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderSynchronous();

		// We may have called doneCurrent() so restore the context.
		saver.context()->makeCurrent(saver.surface());

		m_item->window()->resetOpenGLState();

		if (m_item->always())
		{
			update();
		}
		else
		{
			if (m_item->shotTimes() > 0)
			{
				update();
				m_item->decShotTimes();
			}
		}
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
	Qt3DRender::QRenderAspect* m_renderAspect;
	Qt3DCore::QAspectEngine* m_aspectEngine;
};

QuickNativeRenderItem::QuickNativeRenderItem(QQuickItem* parent)
	: QQuickFramebufferObject(parent)
	, m_aspectEngine(new Qt3DCore::QAspectEngine(this))
	, m_renderAspect(new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Synchronous))
	, m_inputAspect(new Qt3DInput::QInputAspect)
	, m_logicAspect(new Qt3DLogic::QLogicAspect)
	, m_rootEntity(nullptr)
	, m_renderSettings(nullptr)
	, m_inputSettings(nullptr)
	, m_shotTimes(0)
	, m_renderGraph(nullptr)
	, m_always(false)
	, m_ratio(1.0f)
{
	setFlag(ItemHasContents, true);
	m_aspectEngine->registerAspect(m_renderAspect);
	m_aspectEngine->registerAspect(m_inputAspect);
	m_aspectEngine->registerAspect(m_logicAspect);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::AllButtons);
	forceActiveFocus(Qt::MouseFocusReason);

	connect(this, &QQuickItem::windowChanged, this, &QuickNativeRenderItem::handleWindowChanged);

	m_rootEntity = new Qt3DCore::QEntity();
	m_renderSettings = new Qt3DRender::QRenderSettings(m_rootEntity);
	m_inputSettings = new Qt3DInput::QInputSettings(m_rootEntity);
	m_rootEntity->addComponent(m_renderSettings);
	m_rootEntity->addComponent(m_inputSettings);

	m_eventSubdivide = new qtuser_3d::EventSubdivide(this);

	m_rootFrameGraph = new Qt3DRender::QFrameGraphNode(m_renderSettings);
	m_renderSettings->setActiveFrameGraph(m_rootFrameGraph);

	qDebug() << "QuickNativeRenderItem Ctr. thread " << QThread::currentThreadId();
}

QuickNativeRenderItem::~QuickNativeRenderItem()
{
	m_eventSubdivide->closeHandlers();
	m_renderGraph = nullptr;

	delete m_aspectEngine;

	qDebug() << "QuickNativeRenderItem Ctr. thread " << QThread::currentThreadId();
}

void QuickNativeRenderItem::setupGL()
{
	m_aspectEngine = new Qt3DCore::QAspectEngine();
	m_renderAspect = new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Synchronous);
	m_inputAspect = new Qt3DInput::QInputAspect;
	m_logicAspect = new Qt3DLogic::QLogicAspect;

	m_aspectEngine->registerAspect(m_renderAspect);
	m_aspectEngine->registerAspect(m_inputAspect);
	m_aspectEngine->registerAspect(m_logicAspect);

	static_cast<Qt3DRender::QRenderAspectPrivate*>(
		Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderInitialize(QOpenGLContext::currentContext());

	applyRootEntity();
	qDebug() << "Mac setupGL thread. " << QThread::currentThread();
}

void QuickNativeRenderItem::paintGL()
{
	static_cast<Qt3DRender::QRenderAspectPrivate*>(
		Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderSynchronous();
}

void QuickNativeRenderItem::clearScene()
{
	m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr());
}

void QuickNativeRenderItem::releaseGL()
{
	qDebug() << "Mac releaseGL thread " << QThread::currentThreadId();
	qDebug() << "releaseGL Start ~~ ";
	delete m_aspectEngine;
	qDebug() << "releaseGL End ~~ ";
}

void QuickNativeRenderItem::requestUpdate()
{
	m_shotTimes = 20;
	update();
}

int QuickNativeRenderItem::shotTimes()
{
	return m_shotTimes;
}

void QuickNativeRenderItem::decShotTimes()
{
	--m_shotTimes;
}

void QuickNativeRenderItem::setAlways(bool always)
{
	m_always = always;
	update();
}

bool QuickNativeRenderItem::always()
{
	return m_always;
}

void QuickNativeRenderItem::handleWindowChanged(QQuickWindow* win)
{
	if (win)
	{
		m_ratio = win->devicePixelRatio();
		update();
	}
}

void QuickNativeRenderItem::applyRootEntity()
{
	m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(m_rootEntity));

	update();
}

QQuickFramebufferObject::Renderer* QuickNativeRenderItem::createRenderer() const
{
	QQuickFramebufferObject::Renderer* render = new NativeFrameBufferObjectRenderer((QuickNativeRenderItem*)this, m_renderAspect, m_aspectEngine);
	QMetaObject::invokeMethod((QuickNativeRenderItem*)(this), "applyRootEntity", Qt::QueuedConnection);
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
	if (m_ratio < 0.01)
	{
		m_ratio = 1;
	}
	size *= m_ratio;
	m_eventSubdivide->geometryChanged(size);
	if (m_renderGraph) m_renderGraph->updateRenderSize(size);
	requestUpdate();
}

void QuickNativeRenderItem::mousePressEvent(QMouseEvent* event)
{
	QPoint pt = event->pos();
	pt.rx() *= m_ratio;
	pt.ry() *= m_ratio;
	event->setLocalPos(pt);
	m_eventSubdivide->mousePressEvent(event);
	setFocus(true);
}

void QuickNativeRenderItem::mouseMoveEvent(QMouseEvent* event)
{
	QPoint pt = event->pos();
	pt.rx() *= m_ratio;
	pt.ry() *= m_ratio;
	event->setLocalPos(pt);
	m_eventSubdivide->mouseMoveEvent(event);
}

void QuickNativeRenderItem::mouseReleaseEvent(QMouseEvent* event)
{
	QPoint pt = event->pos();
	pt.rx() *= m_ratio;
	pt.ry() *= m_ratio;
	event->setLocalPos(pt);
	m_eventSubdivide->mouseReleaseEvent(event);
}

void QuickNativeRenderItem::wheelEvent(QWheelEvent* event)
{
	QWheelEvent new_event(event->posF() * m_ratio, event->globalPosF(), event->delta() * m_ratio, event->buttons(), event->modifiers(), event->orientation());
	m_eventSubdivide->wheelEvent(&new_event);
}

void QuickNativeRenderItem::hoverEnterEvent(QHoverEvent* event)
{
	QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
	m_eventSubdivide->hoverEnterEvent(&new_event);
}

void QuickNativeRenderItem::hoverMoveEvent(QHoverEvent* event)
{
	QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
	m_eventSubdivide->hoverMoveEvent(&new_event);
}

void QuickNativeRenderItem::hoverLeaveEvent(QHoverEvent* event)
{
	QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
	m_eventSubdivide->hoverLeaveEvent(&new_event);
}

void QuickNativeRenderItem::keyPressEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyPressEvent(event);
}

void QuickNativeRenderItem::keyReleaseEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyReleaseEvent(event);
}

qtuser_3d::EventSubdivide* QuickNativeRenderItem::eventSubdivide()
{
	return m_eventSubdivide;
}

void QuickNativeRenderItem::registerResidentNode(Qt3DCore::QNode* node)
{
	if (node && (m_residentNodes.indexOf(node) == -1))
	{
		node->setParent(m_rootEntity);
		m_residentNodes.push_back(node);
	}
}

void QuickNativeRenderItem::unRegisterResidentNode(Qt3DCore::QNode* node)
{
	int index = m_residentNodes.indexOf(node);
	if (index != -1)
	{
		node->setParent((Qt3DCore::QNode*)nullptr);
		m_residentNodes.removeAt(index);
	}
}

bool QuickNativeRenderItem::isRenderRenderGraph(qtuser_3d::RenderGraph* graph)
{
	return m_renderGraph == graph;
}

void QuickNativeRenderItem::renderRenderGraph(qtuser_3d::RenderGraph* graph)
{
	if (m_renderGraph == graph) return;

	if (m_renderGraph)
	{
		Qt3DRender::QFrameGraphNode* frameGraph = m_renderGraph->frameGraph();
		Qt3DCore::QEntity* sceneGraph = m_renderGraph->sceneGraph();

		if (frameGraph)
		{
			frameGraph->setParent((Qt3DCore::QNode*)nullptr);
		}
		if (sceneGraph) sceneGraph->setEnabled(false);
		m_renderGraph->endRender();

		disconnect(m_renderGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));
	}

	m_renderGraph = graph;

	if (m_renderGraph)
	{
		connect(m_renderGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));

		m_renderGraph->begineRender();
		Qt3DRender::QFrameGraphNode* frameGraph = graph->frameGraph();
		Qt3DCore::QEntity* sceneGraph = graph->sceneGraph();

		if (frameGraph)
		{
			frameGraph->setParent(m_rootFrameGraph);
		}
		if (sceneGraph) sceneGraph->setEnabled(true);

		QSize itemSize = size().toSize();
		if (m_ratio > 0)
		{
			itemSize *= m_ratio;
		}
		m_renderGraph->updateRenderSize(itemSize);
	}

	requestUpdate();
}

void QuickNativeRenderItem::registerRenderGraph(qtuser_3d::RenderGraph* graph)
{
	if (graph && (m_registerRenderGraph.indexOf(graph) == -1))
	{
		Qt3DRender::QFrameGraphNode* frameGraph = graph->frameGraph();
		Qt3DCore::QEntity* sceneGraph = graph->sceneGraph();

		if (frameGraph)
		{
			//frameGraph->setParent(m_rootFrameGraph);
			//frameGraph->setEnabled(false);
		}
		if (sceneGraph)
		{
			sceneGraph->setParent(m_rootEntity);
			sceneGraph->setEnabled(false);
		}

		m_registerRenderGraph.push_back(graph);
	}
}

void QuickNativeRenderItem::unRegisterRenderGraph(qtuser_3d::RenderGraph* graph)
{
	int index = m_registerRenderGraph.indexOf(graph);
	if (index != -1)
	{
		if (m_renderGraph == graph)
		{
			renderRenderGraph(nullptr);
		}

		Qt3DRender::QFrameGraphNode* frameGraph = graph->frameGraph();
		Qt3DCore::QEntity* sceneGraph = graph->sceneGraph();

		if (frameGraph)
		{
			frameGraph->setParent((Qt3DCore::QNode*)nullptr);
			frameGraph->setEnabled(true);
		}
		if (sceneGraph)
		{
			sceneGraph->setParent((Qt3DCore::QNode*)nullptr);
			sceneGraph->setEnabled(true);
		}

		m_registerRenderGraph.removeAt(index);
	}
}

void QuickNativeRenderItem::unRegisterAll()
{
	while (m_registerRenderGraph.size())
	{
		unRegisterRenderGraph(m_registerRenderGraph.front());
	}

	while (m_residentNodes.size())
	{
		unRegisterResidentNode(m_residentNodes.front());
	}
}