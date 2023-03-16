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
		, m_aspectEngine(new Qt3DCore::QAspectEngine())
		, m_renderAspect(new Qt3DRender::QRenderAspect(Qt3DRender::QRenderAspect::Synchronous))
		, m_inputAspect(new Qt3DInput::QInputAspect)
		, m_logicAspect(new Qt3DLogic::QLogicAspect)
		, m_rootEntity(nullptr)
		, m_renderSettings(nullptr)
		, m_inputSettings(nullptr)
		, m_renderGraph(nullptr)
		, m_sharedContext(nullptr)
		, m_times(0)
		, m_continous(false)
	{
		m_aspectEngine->registerAspect(m_renderAspect);
		m_aspectEngine->registerAspect(m_inputAspect);
		m_aspectEngine->registerAspect(m_logicAspect);

		m_rootEntity = new Qt3DCore::QEntity();
		m_renderSettings = new Qt3DRender::QRenderSettings(m_rootEntity);
		m_inputSettings = new Qt3DInput::QInputSettings(m_rootEntity);
		m_rootEntity->addComponent(m_renderSettings);
		m_rootEntity->addComponent(m_inputSettings);

		m_rootFrameGraph = new Qt3DRender::QFrameGraphNode(m_renderSettings);
		m_renderSettings->setActiveFrameGraph(m_rootFrameGraph);

		m_raw = new qtuser_core::RawOGL(this);
		qDebug() << "Q3DRenderSystem Ctr. thread " << QThread::currentThreadId();
	}

	Q3DRenderSystem::~Q3DRenderSystem()
	{
		m_renderGraph = nullptr;
		delete m_aspectEngine;
		qDebug() << "Q3DRenderSystem Ctr~. thread " << QThread::currentThreadId();
	}

	void Q3DRenderSystem::renderNode(Qt3DRender::QFrameGraphNode* node, Qt3DCore::QEntity* entity)
	{
		if (node)
			node->setParent(m_rootFrameGraph);
		if (entity)
			entity->setParent(m_rootEntity);

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

	void Q3DRenderSystem::requestUpdate()
	{
		if (m_continous)
			return;

		m_times = 10;
		invokeUpdate();
	}

	void Q3DRenderSystem::renderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_renderGraph == graph)
			return;


		if (m_renderGraph)
		{
			Qt3DCore::QEntity* sceneGraph = m_renderGraph->sceneGraph();

			if (m_renderGraph)
			{
				m_renderGraph->setParent((Qt3DCore::QNode*)nullptr);
			}
			if (sceneGraph)
				sceneGraph->setParent((Qt3DCore::QNode*)nullptr);
			m_renderGraph->endRender();

			disconnect(m_renderGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));
		}

		m_renderGraph = graph;

		bindRenderGraph();
	}

	void Q3DRenderSystem::unRegisterAll()
	{
		while (m_residentNodes.size())
		{
			unRegisterResidentNode(m_residentNodes.front());
		}

		renderRenderGraph(nullptr);

		m_aspectEngine->setRootEntity(nullptr);
	}

	void Q3DRenderSystem::initializeFromRenderThread()
	{
		QOpenGLContext* context = QOpenGLContext::currentContext();
		static_cast<Qt3DRender::QRenderAspectPrivate*>(
			Qt3DRender::QRenderAspectPrivate::get(m_renderAspect))->renderInitialize(context);

		m_raw->init(context);
		m_sharedContext = context;
		QMetaObject::invokeMethod(this, "applyRootEntity", Qt::QueuedConnection);
	}

	void Q3DRenderSystem::unitializeFromRenderThread()
	{
		//delete m_aspectEngine;
	}

	void Q3DRenderSystem::applyRootEntity()
	{
		m_aspectEngine->setRootEntity(Qt3DCore::QEntityPtr(m_rootEntity));

		bindRenderGraph();
	}

	bool Q3DRenderSystem::render()
	{
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

		if (m_renderGraph)
			m_renderGraph->updateRenderSize(m_size);
	}

	void Q3DRenderSystem::bindRenderGraph()
	{
		if (m_renderGraph && m_aspectEngine->rootEntity())
		{
			m_renderGraph->begineRender();
			Qt3DCore::QEntity* sceneGraph = m_renderGraph->sceneGraph();

			m_renderGraph->setParent(m_rootFrameGraph);
			if (sceneGraph)
				sceneGraph->setParent(m_rootEntity);

			m_renderGraph->updateRenderSize(m_size);
			connect(m_renderGraph, SIGNAL(signalUpdate()), this, SLOT(requestUpdate()));
			requestUpdate();
		}
	}
}