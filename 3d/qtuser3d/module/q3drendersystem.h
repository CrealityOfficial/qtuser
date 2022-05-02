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

		void registerResidentNode(Qt3DCore::QNode* node);
		void unRegisterResidentNode(Qt3DCore::QNode* node);
		void renderRenderGraph(qtuser_3d::RenderGraph* graph);
		void registerRenderGraph(qtuser_3d::RenderGraph* graph);
		void unRegisterRenderGraph(qtuser_3d::RenderGraph* graph);
		void unRegisterAll();

		bool isRenderRenderGraph(qtuser_3d::RenderGraph* graph);

		QOpenGLContext* sharedContext();
		qtuser_core::RawOGL* rawOGL();

		void setContinousRender();
		void setCommandRender();
		void requestUpdate();
	protected:
		void initializeFromRenderThread() override;
		void render() override;
		void synchronize() override;
		void updateContextSize(const QSize& size) override;

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

		qtuser_3d::RenderGraph* m_renderGraph;
		QList<qtuser_3d::RenderGraph*> m_registerRenderGraph;
		QList<Qt3DCore::QNode*> m_residentNodes;
		Qt3DRender::QFrameGraphNode* m_rootFrameGraph;

		QSize m_size;

		qtuser_core::RawOGL* m_raw;
		QOpenGLContext* m_sharedContext;
		int m_times;
	};
}

#endif // QTUSER_3D_Q3DRENDERSYSTEM_1651431043764_H