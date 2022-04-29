#ifndef _QUICKNATIVERENDERITEM_1650183679751_H
#define _QUICKNATIVERENDERITEM_1650183679751_H
#include "qtuserquick/interface.h"
#include <QtQuick/QQuickFramebufferObject>

class QuickNativeRenderItem : public QQuickFramebufferObject 
{
	friend class NativeFrameBufferObjectRenderer;
	Q_OBJECT
public:
	QuickNativeRenderItem(QQuickItem* parent = 0);
	virtual ~QuickNativeRenderItem();

signals:
	void sigRender();
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
};

#endif // _QUICKNATIVERENDERITEM_1650183679751_H