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
	void geometryChanged(const QSize& size) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;
	void hoverEnterEvent(QHoverEvent* event) override;
	void hoverMoveEvent(QHoverEvent* event) override;
	void hoverLeaveEvent(QHoverEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
protected:
	qtuser_3d::EventSubdivide* m_eventSubdivide;
	qtuser_3d::Q3DRenderSystem* m_renderSystem;
};

#endif // _Q3DNATIVERENDERITEM_1650183679751_H