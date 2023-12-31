#ifndef QTUSER_3D_Q3DRENDERSYSTEM_1651431043764_H
#define QTUSER_3D_Q3DRENDERSYSTEM_1651431043764_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtusercore/module/quicknativerendersystem.h"

#include <Qt3DCore/QAspectEngine>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QFrameGraphNode>
#include <Qt3DInput/QInputAspect>
#include <Qt3DInput/QInputSettings>
#include <Qt3DLogic/QLogicAspect>
#include <Qt3DCore/QEntity>
#include <QtCore/QTimer>

#include "qtuser3d/framegraph/rendergraph.h"

namespace qtuser_core
{
	class RawOGL;
}

namespace qtuser_3d
{
	class QTUSER_3D_API Q3DRenderSystem : public qtuser_core::QuickNativeRenderSystem
	{
		Q_OBJECT
	public:
		Q3DRenderSystem(QObject* parent = nullptr);
		virtual ~Q3DRenderSystem();

		void renderNode(Qt3DRender::QFrameGraphNode* node, Qt3DCore::QEntity* entity);
		void registerResidentNode(Qt3DCore::QNode* node);
		void unRegisterResidentNode(Qt3DCore::QNode* node);
		void renderRenderGraph(qtuser_3d::RenderGraph* graph);
		void unRegisterAll();

		QOpenGLContext* sharedContext();
		qtuser_core::RawOGL* rawOGL();

		void setContinousRender();
		void setCommandRender();
		void pauseUpdate();
		bool inRenderingState();

		bool isRenderRenderGraph(RenderGraph* graph);
	public slots:
		void requestUpdate();
	protected:
		void initializeFromRenderThread() override;
		void unitializeFromRenderThread() override;
		bool render() override;
		void synchronize() override;
		void updateContextSize(const QSize& size) override;

		void createRenderSystem();
		void releaseGL();
		RenderGraph* currentRenderGraph();
	public slots:
		void applyRootEntity();
	protected:
		Qt3DCore::QAspectEngine* m_aspectEngine;
		Qt3DRender::QRenderAspect* m_renderAspect;
		Qt3DInput::QInputAspect* m_inputAspect;
		Qt3DLogic::QLogicAspect* m_logicAspect;

		Qt3DCore::QEntity* m_rootEntity;

		Qt3DRender::QRenderSettings* m_renderSettings;
		Qt3DInput::QInputSettings* m_inputSettings;

		QList<qtuser_3d::RenderGraph*> m_registerRenderGraph;
		Qt3DRender::QFrameGraphNode* m_emptyFrameGraphNode;
		QList<Qt3DCore::QNode*> m_residentNodes;

		QSize m_size;

		qtuser_core::RawOGL* m_raw;
		QOpenGLContext* m_sharedContext;
		int m_times;
		volatile bool m_continous;
	};
}

#endif // QTUSER_3D_Q3DRENDERSYSTEM_1651431043764_H