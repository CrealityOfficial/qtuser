#include "q3drendersystem.h"
#include <QtCore/QThread>
#include <QtCore/QDebug>

#include <QtGui/QOpenGLContext>
#include <Qt3DRender/private/qrenderaspect_p.h>

#include "qtusercore/module/openglcontextsaver.h"

namespace qtuser_3d
{
	Q3DRenderSystem::Q3DRenderSystem(QObject* parent)
		: QuickNativeRenderSystem(parent)
		, m_aspectEngine(nullptr)
		, m_renderAspect(nullptr)
		, m_inputAspect(nullptr)
		, m_logicAspect(nullptr)
		, m_rootEntity(nullptr)
		, m_renderSettings(nullptr)
		, m_inputSettings(nullptr)
		, m_sharedContext(nullptr)
		, m_times(0)
		, m_continous(false)
	{
#ifndef __APPLE__
		createRenderSystem();
#endif

		m_rootEntity = new Qt3DCore::QEntity();
		m_renderSettings = new Qt3DRender::QRenderSettings(m_rootEntity);
		m_inputSettings = new Qt3DInput::QInputSettings(m_rootEntity);
		m_rootEntity->addComponent(m_renderSettings);
		m_rootEntity->addComponent(m_inputSettings);

		m_emptyFrameGraphNode = new Qt3DRender::QFrameGraphNode(m_renderSettings);
		m_renderSettings->setActiveFrameGraph(m_emptyFrameGraphNode);

		qDebug() << "Q3DRenderSystem Ctr. thread " << QThread::currentThread();
	}

	Q3DRenderSystem::~Q3DRenderSystem()
	{
		qDebug() << "Q3DRenderSystem Ctr~. thread " << QThread::currentThread();

#ifndef __APPLE__
		releaseGL();
#endif
	}

	void Q3DRenderSystem::renderNode(Qt3DRender::QFrameGraphNode* node, Qt3DCore::QEntity* entity)
	{
		requestUpdate();
	}

	void Q3DRenderSystem::registerResidentNode(Qt3DCore::QNode* node)
	{
		if (node && (m_residentNodes.indexOf(node) == -1))
		{
			node->setParent(m_rootEntity);
			m_residentNodes.push_back(node);
		}
	}

	void Q3DRenderSystem::unRegisterResidentNode(Qt3DCore::QNode* node)
	{
		int index = m_residentNodes.indexOf(node);
		if (index != -1)
		{
			node->setParent((Qt3DCore::QNode*)nullptr);
			m_residentNodes.removeAt(index);
		}
	}

	QOpenGLContext* Q3DRenderSystem::sharedContext()
	{
		return m_sharedContext;
	}

	qtuser_core::RawOGL* Q3DRenderSystem::rawOGL()
	{
		return m_raw;
	}

	void Q3DRenderSystem::setContinousRender()
	{
		m_continous = true;
		invokeUpdate();
	}

	void Q3DRenderSystem::setCommandRender()
	{
		m_continous = false;
		m_times = 10;
		invokeUpdate();
	}

	void Q3DRenderSystem::pauseUpdate()
	{
		m_continous = false;
	}

	bool Q3DRenderSystem::inRenderingState()
	{
		return m_continous;
	}

	void Q3DRenderSystem::requestUpdate()
	{
		if (m_continous)
			return;

		m_times = 10;
		invokeUpdate();
	}

	RenderGraph* Q3DRenderSystem::currentRenderGraph()
	{
		return qobject_cast<RenderGraph*>(m_renderSettings->activeFrameGraph());
	}

	bool Q3DRenderSystem::isRenderRenderGraph(RenderGraph* graph)
	{
		return graph && (currentRenderGraph() == graph);
	}

	void Q3DRenderSystem::renderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (isRenderRenderGraph(graph))
			return;

		RenderGraph* currentGraph = currentRenderGraph();
		if (currentGraph)
		{
			currentGraph->endRender();
			currentGraph->setEnabled(false);

			disconnect(currentGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));
		}

		if (graph)
		{
			connect(graph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));

			graph->begineRender();
			graph->updateRenderSize(m_size);
			graph->setEnabled(true);
			if (graph->parent() != m_renderSettings)
				graph->setParent(m_renderSettings);
		}

		m_renderSettings->setActiveFrameGraph(graph);
		requestUpdate();
	}

	void Q3DRenderSystem::unRegisterAll()
	{
		while (m_residentNodes.size())
		{
			unRegisterResidentNode(m_residentNodes.front());
		}

		renderRenderGraph(nullptr);

		if(m_aspectEngine)
			m_aspectEngine->setRootEntity(nullptr);
	}

	void Q3DRenderSystem::initializeFromRenderThread()
	{
#ifdef __APPLE__
		createRenderSystem();
#endif
		static_cast<Qt3DRender::QRenderAspectPrivate*>(
			Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderInitialize(QOpenGLContext::currentContext());

		m_sharedContext = QOpenGLContext::currentContext();
		qDebug() << "m_sharedContext . thread " << m_sharedContext->thread();

#ifdef __APPLE__
		QObject::connect(m_sharedContext, &QOpenGLContext::aboutToBeDestroyed,
			[this] { releaseGL(); });
#endif

		m_raw->init(m_sharedContext);
		QMetaObject::invokeMethod(this, "applyRootEntity", Qt::QueuedConnection);
	}

	void Q3DRenderSystem::unitializeFromRenderThread()
	{
		if (m_aspectEngine)
			m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr());
	}

	void Q3DRenderSystem::releaseGL()
	{
		qDebug() << "Q3DRenderSystem::releaseGL ->" << QThread::currentThread();
		if (m_aspectEngine)
		{
			delete m_aspectEngine;
			m_aspectEngine = nullptr;
		}
		qDebug() << "releaseGL End ~~ ";
	}

	void Q3DRenderSystem::applyRootEntity()
	{
		if(m_aspectEngine)
			m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(m_rootEntity));
	}

	bool Q3DRenderSystem::render()
	{
		if (!m_renderAspect)
			return false;

		static_cast<Qt3DRender::QRenderAspectPrivate*>(
			Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderSynchronous();

		if (m_continous)
		{
			return true;
		}
		else
		{
			if (m_times > 0)
			{
				--m_times;
				return true;
			}
		}
		return false;
	}

	void Q3DRenderSystem::synchronize()
	{

	}

	void Q3DRenderSystem::updateContextSize(const QSize& size)
	{
		m_size = size;

		RenderGraph* graph = currentRenderGraph();
		if (graph)
			graph->updateRenderSize(m_size);
	}

	void Q3DRenderSystem::createRenderSystem()
	{
		m_aspectEngine = new Qt3DCore::QAspectEngine();
		m_renderAspect = new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Synchronous);
		m_inputAspect = new Qt3DInput::QInputAspect();
		m_logicAspect = new Qt3DLogic::QLogicAspect();

		m_aspectEngine->registerAspect(m_renderAspect);
		m_aspectEngine->registerAspect(m_inputAspect);
		m_aspectEngine->registerAspect(m_logicAspect);

		m_raw = new qtuser_core::RawOGL(this);
		qDebug() << "Q3DRenderSystem::createRenderSystem. thread " << QThread::currentThread();
	}
}