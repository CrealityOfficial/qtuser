#ifndef _qtuser_3d_EVENTHANDLERS_1588745383269_H
#define _qtuser_3d_EVENTHANDLERS_1588745383269_H
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QHoverEvent>
#include <QtCore/QSize>

namespace qtuser_3d
{
	class ResizeEventHandler
	{
	public:
		virtual ~ResizeEventHandler() {}

		virtual void onResize(const QSize& size) = 0;
	};

	class HoverEventHandler
	{
	public:
		virtual ~HoverEventHandler() {}

		virtual void onHoverEnter(QHoverEvent* event) = 0;
		virtual void onHoverLeave(QHoverEvent* event) = 0;
		virtual void onHoverMove(QHoverEvent* event) = 0;
	};

	class WheelEventHandler
	{
	public:
		virtual ~WheelEventHandler() {}

		virtual void onWheelEvent(QWheelEvent* event) = 0;
	};

	class LeftMouseEventHandler
	{
	public:
		virtual ~LeftMouseEventHandler() {}

		virtual void onLeftMouseButtonPress(QMouseEvent* event) = 0;
		virtual void onLeftMouseButtonRelease(QMouseEvent* event) = 0;
		virtual void onLeftMouseButtonMove(QMouseEvent* event) = 0;
		virtual void onLeftMouseButtonClick(QMouseEvent* event) = 0;
	};

	class MidMouseEventHandler
	{
	public:
		virtual ~MidMouseEventHandler() {}

		virtual void onMidMouseButtonPress(QMouseEvent* event) = 0;
		virtual void onMidMouseButtonRelease(QMouseEvent* event) = 0;
		virtual void onMidMouseButtonMove(QMouseEvent* event) = 0;
		virtual void onMidMouseButtonClick(QMouseEvent* event) = 0;
	};

	class RightMouseEventHandler
	{
	public:
		virtual ~RightMouseEventHandler() {}

		virtual void onRightMouseButtonPress(QMouseEvent* event) = 0;
		virtual void onRightMouseButtonRelease(QMouseEvent* event) = 0;
		virtual void onRightMouseButtonMove(QMouseEvent* event) = 0;
		virtual void onRightMouseButtonClick(QMouseEvent* event) = 0;
	};

	class EventLogic : public HoverEventHandler
		, public WheelEventHandler
		, public RightMouseEventHandler
		, public MidMouseEventHandler
		, public LeftMouseEventHandler
		, public ResizeEventHandler
	{
	public:
		virtual ~EventLogic() {}
	};

	class KeyEventHandler
	{
	public:
		virtual ~KeyEventHandler() {}
		virtual void onKeyPress(QKeyEvent* event) = 0;
		virtual void onKeyRelease(QKeyEvent* event) = 0;
	};
}
#endif // _qtuser_3d_EVENTHANDLERS_1588745383269_H
