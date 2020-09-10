#include "eventtransmission.h"
namespace qtuser_3d
{
	EventTransmission::EventTransmission(QObject* parent)
		:QObject(parent)
	{
	}
	EventTransmission::~EventTransmission()
	{
	}

	bool EventTransmission::eventFilter(QObject* watched, QEvent* event)
	{
		bool result = true;
		switch (event->type())
		{
		case QEvent::MouseButtonPress:
			result = processMousePressEvent((QMouseEvent*)event);
			break;
		case QEvent::MouseButtonRelease:
			result = processMouseReleaseEvent((QMouseEvent*)event);
			break;
		case QEvent::KeyPress:
			result = processKeyPressEvent((QKeyEvent*)event);
			break;
		case QEvent::KeyRelease:
			result = processKeyReleaseEvent((QKeyEvent*)event);
			break;
		case QEvent::HoverMove:
			result = processHoverEvent((QHoverEvent*)event);
			break;
		case QEvent::Wheel:
			result = processWheelEvent((QWheelEvent*)event);
			break;
		default:
			break;
		}
		return result;
	}

	bool EventTransmission::processKeyPressEvent(QKeyEvent* event)
	{
		return false;
	}

	bool EventTransmission::processKeyReleaseEvent(QKeyEvent* event)
	{
		return false;
	}

	bool EventTransmission::processMousePressEvent(QMouseEvent* event)
	{
		return false;
	}

	bool EventTransmission::processMouseReleaseEvent(QMouseEvent* event)
	{
		return false;
	}

	bool EventTransmission::processHoverEvent(QHoverEvent* event)
	{
		return false;
	}

	bool EventTransmission::processWheelEvent(QWheelEvent* event)
	{
		return false;
	}
}
