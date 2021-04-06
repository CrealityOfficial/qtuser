#ifndef _QTUSER_QML3D_GLQUICKITEM_1588076188113_H
#define _QTUSER_QML3D_GLQUICKITEM_1588076188113_H
#include "qtuserqml/qtuserqmlexport.h"
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

namespace qtuser_qml
{
	class RawOGL;
}

using namespace qtuser_3d;

class QTUSER_QML_API GLQuickItem : public QQuickFramebufferObject
{
	friend class FrameBufferObjectRenderer;
	Q_OBJECT
public:
	GLQuickItem(QQuickItem* parent = 0);
	~GLQuickItem();

	void setupGL();
	void clearScene();
	void paintGL();
	void releaseGL();

	int shotTimes();
	void decShotTimes();
	void setAlways(bool always);
	bool always();

	EventSubdivide* eventSubdivide();
	void registerResidentNode(Qt3DCore::QNode* node);
	void unRegisterResidentNode(Qt3DCore::QNode* node);
	void renderRenderGraph(RenderGraph* graph);
	void registerRenderGraph(RenderGraph* graph);
	void unRegisterRenderGraph(RenderGraph* graph);
	void unRegisterAll();

	bool isRenderRenderGraph(RenderGraph* graph);

	QOpenGLContext* sharedContext();
	qtuser_qml::RawOGL* rawOGL();
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
private:
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

	QOpenGLContext* m_sharedContext;
	qtuser_qml::RawOGL* m_rawOGL;
};
#endif // _QTUSER_QML3D_GLQUICKITEM_1588076188113_H
