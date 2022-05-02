#ifndef _EVENT_DISPACHER_1588905534361_H
#define _EVENT_DISPACHER_1588905534361_H
#include "qtusercore/qtusercoreexport.h"
#include <QtGui/qevent.h>

namespace qtuser_core
{
	class QuickNativeEventDispacher
	{
	public:
		virtual ~QuickNativeEventDispacher() {}
		virtual void geometryChanged(const QSize& size) = 0;
		virtual void mousePressEvent(QMouseEvent* event) = 0;
		virtual void mouseMoveEvent(QMouseEvent* event) = 0;
		virtual void mouseReleaseEvent(QMouseEvent* event) = 0;
		virtual void wheelEvent(QWheelEvent* event) = 0;
		virtual void hoverEnterEvent(QHoverEvent* event) = 0;
		virtual void hoverMoveEvent(QHoverEvent* event) = 0;
		virtual void hoverLeaveEvent(QHoverEvent* event) = 0;
		virtual void keyPressEvent(QKeyEvent* event) = 0;
		virtual void keyReleaseEvent(QKeyEvent* event) = 0;
	};

}
#endif // _EVENT_DISPACHER_1588905534361_H
