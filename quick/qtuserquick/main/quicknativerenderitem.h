#ifndef _QUICKNATIVERENDERITEM_1650183679751_H
#define _QUICKNATIVERENDERITEM_1650183679751_H
#include "qtuserquick/interface.h"
#include "qtusercore/event/eventdispacher.h"
#include "qtusercore/module/quicknativerendersystem.h"
#include <QtQuick/QQuickFramebufferObject>

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
	class EventSubdivide;
}

class QTUSER_QUICK_API QuickNativeRenderItem : public QQuickFramebufferObject
{
	friend class NativeFrameBufferObjectRenderer;
	friend class NativeRenderSystemWrapper;
	Q_OBJECT
public:
	QuickNativeRenderItem(QQuickItem* parent = 0);
	virtual ~QuickNativeRenderItem();

	void setQuickNativeEventDispacher(qtuser_core::QuickNativeEventDispacher* dispacher);
	void setQuickNativeRenderSystem(qtuser_core::QuickNativeRenderSystem* renderSystem);

public slots:
	void handleWindowChanged(QQuickWindow* win);
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

	void _initialize(); // called from renderer ctr
	void _render(); // called from Renderer
	void _synchronize();
protected:
	qtuser_core::QuickNativeEventDispacher* m_dispacher;
	qtuser_core::QuickNativeRenderSystem* m_renderSystem;

	float m_ratio;
};

#endif // _QUICKNATIVERENDERITEM_1650183679751_H