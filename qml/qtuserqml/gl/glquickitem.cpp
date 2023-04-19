#include "qtuserqml/gl/glquickitem.h"
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QOpenGLFramebufferObjectFormat>
#include <QtGui/QOffscreenSurface>
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
#include "qtuser3d/framegraph/xrendergraph.h"
#include "qtuserqml/gl/rawogl.h"

#include "qtuserqml/macro.h"

QTUSER_QML_REG(GLQuickItem)

#ifdef __APPLE__
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
	FrameBufferObjectRenderer(GLQuickItem* item)
		: m_item(item)
	{
		m_item->setupGL();
	}

	virtual ~FrameBufferObjectRenderer()
	{
        m_item->clearScene();
        qDebug() << "APPLE FrameBufferObjectRenderer ~~";
	}

	void render() Q_DECL_OVERRIDE
	{
		ContextSaver saver;

		m_item->paintGL();

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

#if _DEBUG
		//qDebug() << "FrameBufferObjectRenderer Render ";
#endif
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
};

GLQuickItem::GLQuickItem(QQuickItem* parent)
	: QQuickFramebufferObject(parent)
	, m_rootEntity(nullptr)
	, m_renderSettings(nullptr)
	, m_inputSettings(nullptr)
	, m_shotTimes(0)
	, m_always(false)
	, m_sharedContext(nullptr)
	, m_ratio(1)
{
	setFlag(ItemHasContents, true);

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

	setFocus(true, Qt::ActiveWindowFocusReason);

	qDebug() << "mac GLQuickItem -->" << QThread::currentThread();
}

GLQuickItem::~GLQuickItem()
{
	m_eventSubdivide->closeHandlers();
	
	qDebug() << "mac GLQuickItem ~~ -->" << QThread::currentThread();
}

void GLQuickItem::setupGL()
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
	setSharedContext(QOpenGLContext::currentContext());

	qDebug() << "mac setupGL -->" << QThread::currentThread();
}

void GLQuickItem::paintGL()
{
	static_cast<Qt3DRender::QRenderAspectPrivate*>(
		Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderSynchronous();
}

void GLQuickItem::clearScene()
{
	m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr());
}

void GLQuickItem::releaseGL()
{

	qDebug() << "mac releaseGL -->" << QThread::currentThread();

    qDebug() << "releaseGL Start ~~ ";
    delete m_aspectEngine;
    qDebug() << "releaseGL End ~~ ";

	delete m_rawOGL;
	m_rawOGL = nullptr;
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
    if(win)
    {
        m_ratio = win->devicePixelRatio();
        qDebug() << "handleWindowChanged = " << m_ratio;
        update();
    }
}

void GLQuickItem::applyRootEntity()
{
	m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(m_rootEntity));

	update();
}

QQuickFramebufferObject::Renderer* GLQuickItem::createRenderer() const
{
	QQuickFramebufferObject::Renderer* render = new FrameBufferObjectRenderer((GLQuickItem*)this);
	return render;
}

void GLQuickItem::setSharedContext(QOpenGLContext* context)
{
	m_sharedContext = context;
    m_rawOGL = new qtuser_qml::RawOGL();
	m_rawOGL->init(m_sharedContext);

	QObject::connect(m_sharedContext, &QOpenGLContext::aboutToBeDestroyed,
		[this] { releaseGL(); });

	assert(m_sharedContext);
}
#else
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

		qDebug() << "windows FrameBufferObjectRenderer  -->" << QThread::currentThread();
	}

	virtual ~FrameBufferObjectRenderer()
	{
		qDebug() << "windows FrameBufferObjectRenderer ~~ -->" << QThread::currentThread();
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
	, m_shotTimes(0)
	, m_always(false)
	, m_sharedContext(nullptr)
	, m_ratio(1.0f)
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
	m_emptyFrameGraphNode = new Qt3DRender::QFrameGraphNode(m_renderSettings);
	m_renderSettings->setActiveFrameGraph(m_emptyFrameGraphNode);

	m_eventSubdivide = new qtuser_3d::EventSubdivide(this);

	m_rawOGL = new qtuser_qml::RawOGL(this);
    qDebug() << "windows GLQuickItem  -->" << QThread::currentThread();
}

GLQuickItem::~GLQuickItem()
{
	qDebug() << "windows GLQuickItem destruction -->" << QThread::currentThread();

	m_eventSubdivide->closeHandlers();
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
	if (win)
	{
		m_ratio = win->devicePixelRatio();
		update();
	}
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
#endif


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
	if (m_ratio < 0.01)
	{
		m_ratio = 1;
	}
	size *= m_ratio;
	m_eventSubdivide->geometryChanged(size);

	RenderGraph* graph = currentRenderGraph();
	if (graph) 
		graph->updateRenderSize(size);

	requestUpdate();
}

void GLQuickItem::mousePressEvent(QMouseEvent* event)
{
    QPoint pt = event->pos();
    pt.rx() *= m_ratio;
    pt.ry() *= m_ratio;
    event->setLocalPos(pt);
	m_eventSubdivide->mousePressEvent(event);
	setFocus(true);
}

void GLQuickItem::mouseMoveEvent(QMouseEvent* event)
{
    QPoint pt = event->pos();
    pt.rx() *= m_ratio;
    pt.ry() *= m_ratio;
    event->setLocalPos(pt);
	m_eventSubdivide->mouseMoveEvent(event);
}

void GLQuickItem::mouseReleaseEvent(QMouseEvent* event)
{
    QPoint pt = event->pos();
    pt.rx() *= m_ratio;
    pt.ry() *= m_ratio;
    event->setLocalPos(pt);
	m_eventSubdivide->mouseReleaseEvent(event);
}

void GLQuickItem::wheelEvent(QWheelEvent* event)
{
    QWheelEvent new_event(event->posF() * m_ratio, event->globalPosF(), event->delta() * m_ratio, event->buttons(), event->modifiers(), event->orientation());
    m_eventSubdivide->wheelEvent(&new_event);
}

void GLQuickItem::hoverEnterEvent(QHoverEvent* event)
{
    QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
    m_eventSubdivide->hoverEnterEvent(&new_event);
}

void GLQuickItem::hoverMoveEvent(QHoverEvent* event)
{
    QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
    m_eventSubdivide->hoverMoveEvent(&new_event);
}

void GLQuickItem::hoverLeaveEvent(QHoverEvent* event)
{
    QHoverEvent new_event(event->type(), event->posF() * m_ratio, event->oldPosF() * m_ratio, event->modifiers());
    m_eventSubdivide->hoverLeaveEvent(&new_event);
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

RenderGraph* GLQuickItem::currentRenderGraph()
{
	return qobject_cast<RenderGraph*>(m_renderSettings->activeFrameGraph());
}

bool GLQuickItem::isRenderRenderGraph(RenderGraph* graph)
{
	return graph && (currentRenderGraph() == graph);
}

void GLQuickItem::renderRenderGraph(RenderGraph* graph)
{
	if (isRenderRenderGraph(graph))
		return;

	RenderGraph* currentGraph = currentRenderGraph();
	if (currentGraph)
	{
		Qt3DCore::QEntity* sceneGraph = currentGraph->sceneGraph();
		if(sceneGraph)
			sceneGraph->setParent((Qt3DCore::QNode*)nullptr);
		currentGraph->endRender();
		currentGraph->setEnabled(false);

		disconnect(currentGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));

		XRenderGraph* xRender = qobject_cast<XRenderGraph*>(currentGraph);
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
		Qt3DCore::QEntity* sceneGraph = graph->sceneGraph();
		if (sceneGraph) 
			sceneGraph->setParent(m_rootEntity);

		QSize itemSize = size().toSize();
		if (m_ratio > 0)
		{
			itemSize *= m_ratio;
		}
		graph->updateRenderSize(itemSize);
		graph->setEnabled(true);

		XRenderGraph* xRender = qobject_cast<XRenderGraph*>(graph);
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
	requestUpdate();
}

void GLQuickItem::registerRenderGraph(RenderGraph* graph)
{
	if (graph && (m_registerRenderGraph.indexOf(graph) == -1))
	{
		Qt3DCore::QEntity* sceneGraph = graph->sceneGraph();
		graph->setParent(m_renderSettings);
		
		if (sceneGraph)
		{
			//sceneGraph->setParent(m_rootEntity);
			//sceneGraph->setEnabled(false);
		}

		m_registerRenderGraph.push_back(graph);
	}
}

void GLQuickItem::unRegisterRenderGraph(RenderGraph* graph)
{
	int index = m_registerRenderGraph.indexOf(graph);
	if (index != -1)
	{
		if (isRenderRenderGraph(graph))
			renderRenderGraph(nullptr);

		Qt3DCore::QEntity* sceneGraph = graph->sceneGraph();
		graph->setParent((Qt3DCore::QNode*)nullptr);
		graph->setEnabled(true);

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
