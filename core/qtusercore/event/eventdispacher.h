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
		virtual void _geometryChanged(const QSize& size) = 0;
		virtual void _mousePressEvent(QMouseEvent* event) = 0;
		virtual void _mouseMoveEvent(QMouseEvent* event) = 0;
		virtual void _mouseReleaseEvent(QMouseEvent* event) = 0;
		virtual void _wheelEvent(QWheelEvent* event) = 0;
		virtual void _hoverEnterEvent(QHoverEvent* event) = 0;
		virtual void _hoverMoveEvent(QHoverEvent* event) = 0;
		virtual void _hoverLeaveEvent(QHoverEvent* event) = 0;
		virtual void _keyPressEvent(QKeyEvent* event) = 0;
		virtual void _keyReleaseEvent(QKeyEvent* event) = 0;
	};

}
#endif // _EVENT_DISPACHER_1588905534361_H
