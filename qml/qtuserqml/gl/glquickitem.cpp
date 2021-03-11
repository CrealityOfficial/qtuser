#include "qtuserqml/gl/glquickitem.h"
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QtGui/QOffscreenSurface>

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
#include "qtuserqml/gl/rawogl.h"
class ContextSaver
{
public:
	explicit ContextSaver(QOpenGLContext* context = QOpenGLContext::currentContext())
		: m_context(context)
		, m_surface(context ? static_cast<QOffscreenSurface*>(context->surface()) : nullptr)
	{
		//qDebug() << m_context->format();
	}

	~ContextSaver()
	{
		if (m_context)
			m_context->makeCurrent(m_surface);
	}

	QOpenGLContext* context() const { return m_context; }
	QOffscreenSurface* surface() const { return m_surface; }

private:
	QOpenGLContext* const m_context;
	QOffscreenSurface* const m_surface;
};

class FrameBufferObjectRenderer : public QQuickFramebufferObject::Renderer
{
public:
	FrameBufferObjectRenderer(GLQuickItem* item, Qt3DRender::QRenderAspect* renderAspect
		, Qt3DCore::QAspectEngine* aspectEngine)
		:m_renderAspect(renderAspect), m_item(item)
		, m_aspectEngine(aspectEngine)
	{
		static_cast<Qt3DRender::QRenderAspectPrivate*>(
			Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderInitialize(QOpenGLContext::currentContext());
		m_item->setSharedContext(QOpenGLContext::currentContext());
	}

	virtual ~FrameBufferObjectRenderer()
	{

	}

	void render() Q_DECL_OVERRIDE
	{
		ContextSaver saver;

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
	GLQuickItem* m_item;
	Qt3DRender::QRenderAspect* m_renderAspect;
	Qt3DCore::QAspectEngine* m_aspectEngine;
};

GLQuickItem::GLQuickItem(QQuickItem* parent)
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
	, m_sharedContext(nullptr)
{
	setFlag(ItemHasContents, true);
	m_aspectEngine->registerAspect(m_renderAspect);
	m_aspectEngine->registerAspect(m_inputAspect);
	m_aspectEngine->registerAspect(m_logicAspect);

	setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::AllButtons);
	forceActiveFocus(Qt::MouseFocusReason);

	connect(this, &QQuickItem::windowChanged, this, &GLQuickItem::handleWindowChanged);

	m_rootEntity = new Qt3DCore::QEntity();
	m_renderSettings = new Qt3DRender::QRenderSettings(m_rootEntity);
	m_inputSettings = new Qt3DInput::QInputSettings(m_rootEntity);
	m_rootEntity->addComponent(m_renderSettings);
	m_rootEntity->addComponent(m_inputSettings);

	m_eventSubdivide = new qtuser_3d::EventSubdivide(this);

	m_rootFrameGraph = new Qt3DRender::QFrameGraphNode(m_renderSettings);
	m_renderSettings->setActiveFrameGraph(m_rootFrameGraph);

	m_rawOGL = new qtuser_qml::RawOGL(this);
}

GLQuickItem::~GLQuickItem()
{
	m_eventSubdivide->closeHandlers();
	m_renderGraph = nullptr;
	
	delete m_aspectEngine;
}

void GLQuickItem::requestUpdate()
{
	m_shotTimes = 20;
	update();
}

int GLQuickItem::shotTimes()
{
	return m_shotTimes;
}

void GLQuickItem::decShotTimes()
{
	--m_shotTimes;
}

void GLQuickItem::setAlways(bool always)
{
	m_always = always;
	update();
}

bool GLQuickItem::always()
{
	return m_always;
}

void GLQuickItem::handleWindowChanged(QQuickWindow* win)
{
	update();
}

void GLQuickItem::applyRootEntity()
{
	m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(m_rootEntity));

	update();
}

QQuickFramebufferObject::Renderer* GLQuickItem::createRenderer() const
{
	QQuickFramebufferObject::Renderer* render = new FrameBufferObjectRenderer((GLQuickItem*)this, m_renderAspect, m_aspectEngine);
	QMetaObject::invokeMethod((GLQuickItem*)(this), "applyRootEntity", Qt::QueuedConnection);
	return render;
}

void GLQuickItem::setSharedContext(QOpenGLContext* context)
{
	m_sharedContext = context;
	m_rawOGL->init(m_sharedContext);
	assert(m_sharedContext);
}

QOpenGLContext* GLQuickItem::sharedContext()
{
	return m_sharedContext;
}

qtuser_qml::RawOGL* GLQuickItem::rawOGL()
{
	return m_rawOGL;
}

QSGNode* GLQuickItem::updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* nodeData)
{
	node = QQuickFramebufferObject::updatePaintNode(node, nodeData);
	QSGSimpleTextureNode* textureNode = static_cast<QSGSimpleTextureNode*>(node);
	if (textureNode)
		textureNode->setTextureCoordinatesTransform(QSGSimpleTextureNode::MirrorVertically);
	return node;
}

void GLQuickItem::geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry)
{
	QQuickFramebufferObject::geometryChanged(newGeometry, oldGeometry);

	QSize size = newGeometry.size().toSize();
	m_eventSubdivide->geometryChanged(size);
	if (m_renderGraph) m_renderGraph->updateRenderSize(size);
	requestUpdate();
}

void GLQuickItem::mousePressEvent(QMouseEvent* event)
{
	m_eventSubdivide->mousePressEvent(event);
	setFocus(true);
}

void GLQuickItem::mouseMoveEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseMoveEvent(event);
}

void GLQuickItem::mouseReleaseEvent(QMouseEvent* event)
{
	m_eventSubdivide->mouseReleaseEvent(event);
}

void GLQuickItem::wheelEvent(QWheelEvent* event)
{
    m_eventSubdivide->wheelEvent(event);
}

void GLQuickItem::hoverEnterEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverEnterEvent(event);
}

void GLQuickItem::hoverMoveEvent(QHoverEvent* event)
{
	//qDebug() << event->pos();
	m_eventSubdivide->hoverMoveEvent(event);
}

void GLQuickItem::hoverLeaveEvent(QHoverEvent* event)
{
	m_eventSubdivide->hoverLeaveEvent(event);
}

void GLQuickItem::keyPressEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyPressEvent(event);
}

void GLQuickItem::keyReleaseEvent(QKeyEvent* event)
{
	m_eventSubdivide->keyReleaseEvent(event);
}

EventSubdivide* GLQuickItem::eventSubdivide()
{
	return m_eventSubdivide;
}

void GLQuickItem::registerResidentNode(Qt3DCore::QNode* node)
{
	if (node && (m_residentNodes.indexOf(node) == -1))
	{
		node->setParent(m_rootEntity);
		m_residentNodes.push_back(node);
	}
}

void GLQuickItem::unRegisterResidentNode(Qt3DCore::QNode* node)
{
	int index = m_residentNodes.indexOf(node);
	if (index != -1)
	{
		node->setParent((Qt3DCore::QNode*)nullptr);
		m_residentNodes.removeAt(index);
	}
}

bool GLQuickItem::isRenderRenderGraph(RenderGraph* graph)
{
	return m_renderGraph == graph;
}

void GLQuickItem::renderRenderGraph(RenderGraph* graph)
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
		if(sceneGraph) sceneGraph->setEnabled(false);
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
		m_renderGraph->updateRenderSize(itemSize);
	}

	requestUpdate();
}

void GLQuickItem::registerRenderGraph(RenderGraph* graph)
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

void GLQuickItem::unRegisterRenderGraph(RenderGraph* graph)
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

void GLQuickItem::unRegisterAll()
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
