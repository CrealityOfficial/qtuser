#ifndef _Q3DNATIVERENDERITEM_1650183679751_H
#define _Q3DNATIVERENDERITEM_1650183679751_H
#include "qtuserqml/qtuserqmlexport.h"
#include "qtuserquick/main/quicknativerenderitem.h"
#include "qtusercore/event/eventdispacher.h"
#include "qtuser3d/module/q3drendersystem.h"
#include "qtuser3d/event/eventsubdivide.h"

class QTUSER_QML_API Q3DNativeRenderItem : public QuickNativeRenderItem
	, public qtuser_core::QuickNativeEventDispacher
{
	Q_OBJECT
public:
	Q3DNativeRenderItem(QQuickItem* parent = 0);
	virtual ~Q3DNativeRenderItem();

	void uninitialize();
	qtuser_3d::EventSubdivide* eventSubdivide();
	qtuser_3d::Q3DRenderSystem* renderSystem();
protected:
	void _geometryChanged(const QSize& size) override;
	void _mousePressEvent(QMouseEvent* event) override;
	void _mouseMoveEvent(QMouseEvent* event) override;
	void _mouseReleaseEvent(QMouseEvent* event) override;
	void _wheelEvent(QWheelEvent* event) override;
	void _hoverEnterEvent(QHoverEvent* event) override;
	void _hoverMoveEvent(QHoverEvent* event) override;
	void _hoverLeaveEvent(QHoverEvent* event) override;
	void _keyPressEvent(QKeyEvent* event) override;
	void _keyReleaseEvent(QKeyEvent* event) override;
protected:
	qtuser_3d::EventSubdivide* m_eventSubdivide;
	qtuser_3d::Q3DRenderSystem* m_q3dRenderSystem;
};

#endif // _Q3DNATIVERENDERITEM_1650183679751_H