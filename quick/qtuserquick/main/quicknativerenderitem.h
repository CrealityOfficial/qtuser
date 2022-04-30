#ifndef _QUICKNATIVERENDERITEM_1650183679751_H
#define _QUICKNATIVERENDERITEM_1650183679751_H
#include "qtuserquick/interface.h"
#include <QtQuick/QQuickFramebufferObject>
#include <Qt3DCore/QAspectEngine>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QRenderSettings>
#include <Qt3DRender/QClearBuffers>
#include <QtGui/QOffscreenSurface>
#include <Qt3DRender/QRenderSurfaceSelector>

namespace Qt3DInput
{
	class QInputAspect;
	class QInputSettings;
}

namespace Qt3DLogic
{
	class QLogicAspect;
}

namespace qtuser_3d
{
	class RenderGraph;
	class EventSubdivide;
}

namespace qtuser_core
{
	class RawOGL;
}

class QTUSER_QUICK_API QuickNativeRenderItem : public QQuickFramebufferObject
{
	friend class NativeFrameBufferObjectRenderer;
	Q_OBJECT
public:
	QuickNativeRenderItem(QQuickItem* parent = 0);
	virtual ~QuickNativeRenderItem();

	void setupGL();
	void clearScene();
	void paintGL();
	void releaseGL();

	int shotTimes();
	void decShotTimes();
	void setAlways(bool always);
	bool always();

	qtuser_3d::EventSubdivide* eventSubdivide();
	void registerResidentNode(Qt3DCore::QNode* node);
	void unRegisterResidentNode(Qt3DCore::QNode* node);
	void renderRenderGraph(qtuser_3d::RenderGraph* graph);
	void registerRenderGraph(qtuser_3d::RenderGraph* graph);
	void unRegisterRenderGraph(qtuser_3d::RenderGraph* graph);
	void unRegisterAll();

	bool isRenderRenderGraph(qtuser_3d::RenderGraph* graph);
	
	QOpenGLContext* sharedContext();
	qtuser_core::RawOGL* rawOGL();
public slots:
	void handleWindowChanged(QQuickWindow* win);
	void applyRootEntity();
	void requestUpdate();
protected:
	Renderer* createRenderer() const Q_DECL_OVERRIDE;
	QSGNode* updatePaintNode(QSGNode* node, UpdatePaintNodeData* nodeData) Q_DECL_OVERRIDE;
	void geometryChanged(const QRectF& newGeometry, const QRectF& oldGeometry) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent* event) Q_DECL_OVERRIDE;
	void hoverEnterEvent(QHoverEvent* event) Q_DECL_OVERRIDE;
	void hoverMoveEvent(QHoverEvent* event) Q_DECL_OVERRIDE;
	void hoverLeaveEvent(QHoverEvent* event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent* event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

	void setSharedContext(QOpenGLContext* context);
protected:
	Qt3DCore::QAspectEngine* m_aspectEngine;
	Qt3DRender::QRenderAspect* m_renderAspect;
	Qt3DInput::QInputAspect* m_inputAspect;
	Qt3DLogic::QLogicAspect* m_logicAspect;

	Qt3DCore::QEntity* m_rootEntity;

	Qt3DRender::QRenderSettings* m_renderSettings;
	Qt3DInput::QInputSettings* m_inputSettings;

	qtuser_3d::EventSubdivide* m_eventSubdivide;

	int m_shotTimes;

	qtuser_3d::RenderGraph* m_renderGraph;
	QList<qtuser_3d::RenderGraph*> m_registerRenderGraph;
	QList<Qt3DCore::QNode*> m_residentNodes;
	Qt3DRender::QFrameGraphNode* m_rootFrameGraph;

	bool m_always;
	float m_ratio;

	qtuser_core::RawOGL* m_raw;
	QOpenGLContext* m_sharedContext;
};

#endif // _QUICKNATIVERENDERITEM_1650183679751_H