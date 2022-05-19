#include "qtuser3d/event/eventsubdivide.h"
#include "qtuser3d/event/eventhandlers.h"
#include "qtuser3d/event/clickeventchecker.h"
#include <qdebug.h>

namespace qtuser_3d
{
	EventSubdivide::EventSubdivide(QObject* parent)
		:QObject(parent)
	{
		m_clickEventChecker = new ClickEventChecker(this);
	}

	EventSubdivide::~EventSubdivide()
	{
	}

	void EventSubdivide::geometryChanged(const QSize& size)
	{
		m_size = size;
		for (ResizeEventHandler* handler : m_resizeEventHandlers)
		{
			handler->onResize(size);
		}
	}

	void EventSubdivide::mousePressEvent(QMouseEvent* event)
	{
		if ((event->button() == Qt::LeftButton))
		{
			for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
			{
				handler->onLeftMouseButtonPress(event);
			}
		}
		if ((event->button() == Qt::MiddleButton))
		{
			for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
			{
				handler->onMidMouseButtonPress(event);
			}
		}
		if ((event->button() == Qt::RightButton))
		{
			for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
			{
				handler->onRightMouseButtonPress(event);
			}
            m_bLastMove = false;
		}

		m_clickEventChecker->mousePressEvent(event);
	}

	void EventSubdivide::mouseMoveEvent(QMouseEvent* event)
	{
		if ((event->buttons() == Qt::RightButton))
		{
			for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
			{
				handler->onRightMouseButtonMove(event);
			}
            m_bLastMove = true;
		}
		if ((event->buttons() == Qt::MiddleButton))
		{
			for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
			{
				handler->onMidMouseButtonMove(event);
			}
		}
		if ((event->buttons() == Qt::LeftButton))
		{
			for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
			{
				handler->onLeftMouseButtonMove(event);
			}
		}

		m_clickEventChecker->mouseMoveEvent(event);
	}

	void EventSubdivide::mouseReleaseEvent(QMouseEvent* event)
	{
		if ((event->button() == Qt::RightButton))
		{
			for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
			{
				handler->onRightMouseButtonRelease(event);
			}
		}
		if ((event->button() == Qt::MiddleButton))
		{
			for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
			{
				handler->onMidMouseButtonRelease(event);
			}
		}
		if ((event->button() == Qt::LeftButton))
		{
			for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
			{
				handler->onLeftMouseButtonRelease(event);
			}
		}

		if (m_clickEventChecker->CheckRightButton(event))
		{
            if(m_bLastMove)return;
			for (RightMouseEventHandler* handler : m_rightMouseEventHandlers)
			{
				handler->onRightMouseButtonClick(event);
			}
		}
		if (m_clickEventChecker->CheckMiddleButton(event))
		{
			for (MidMouseEventHandler* handler : m_midMouseEventHandlers)
			{
				handler->onMidMouseButtonClick(event);
			}
		}
		if (m_clickEventChecker->CheckLeftButton(event))
		{
			for (LeftMouseEventHandler* handler : m_leftMouseEventHandlers)
			{
				handler->onLeftMouseButtonClick(event);
			}
		}
	}

	void EventSubdivide::wheelEvent(QWheelEvent* event)
	{
		for (WheelEventHandler* handler : m_wheelEventHandlers)
		{
			handler->onWheelEvent(event);
		}
	}

	void EventSubdivide::hoverEnterEvent(QHoverEvent* event)
	{
		for (HoverEventHandler* handler : m_hoverEventHandlers)
		{
			handler->onHoverEnter(event);
		}
	}

	void EventSubdivide::hoverMoveEvent(QHoverEvent* event)
	{
		for (HoverEventHandler* handler : m_hoverEventHandlers)
		{
			handler->onHoverMove(event);
		}
	}

	void EventSubdivide::hoverLeaveEvent(QHoverEvent* event)
	{
		for (HoverEventHandler* handler : m_hoverEventHandlers)
		{
			handler->onHoverLeave(event);
		}
	}

	void EventSubdivide::keyPressEvent(QKeyEvent* event)
	{
		for (KeyEventHandler* handler : m_KeyEventHandlers)
		{
			handler->onKeyPress(event);
		}
	}

	void EventSubdivide::keyReleaseEvent(QKeyEvent* event)
	{
		for (KeyEventHandler* handler : m_KeyEventHandlers)
		{
			handler->onKeyRelease(event);
		}
	}

	void EventSubdivide::closeHandlers()
	{
		closeLeftMouseEventHandlers();
		closeRightMouseEventHandlers();
		closeWheelEventHandlers();
		closeHoverEventHandlers();
		closeResizeEventHandlers();
		closeKeyEventHandlers();
	}

	void EventSubdivide::addResizeEventHandler(ResizeEventHandler* handler)
	{
		if (handler)
		{
			handler->onResize(m_size);
			m_resizeEventHandlers.push_back(handler);
		}
	}

	void EventSubdivide::removeResizeEventHandler(ResizeEventHandler* handler)
	{
		m_resizeEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeResizeEventHandlers()
	{
		m_resizeEventHandlers.clear();
	}

	void EventSubdivide::addHoverEventHandler(HoverEventHandler* handler)
	{
		if (handler) m_hoverEventHandlers.push_back(handler);
	}

	void EventSubdivide::removeHoverEventHandler(HoverEventHandler* handler)
	{
		m_hoverEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeHoverEventHandlers()
	{
		m_hoverEventHandlers.clear();
	}

	void EventSubdivide::addWheelEventHandler(WheelEventHandler* handler)
	{
		if (handler) m_wheelEventHandlers.push_back(handler);
	}

	void EventSubdivide::removeWheelEventHandler(WheelEventHandler* handler)
	{
		m_wheelEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeWheelEventHandlers()
	{
		m_wheelEventHandlers.clear();
	}

	void EventSubdivide::addRightMouseEventHandler(RightMouseEventHandler* handler)
	{
		if (handler) m_rightMouseEventHandlers.push_back(handler);
	}

	void EventSubdivide::removeRightMouseEventHandler(RightMouseEventHandler* handler)
	{
		m_rightMouseEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeRightMouseEventHandlers()
	{
		m_rightMouseEventHandlers.clear();
	}


	void EventSubdivide::addMidMouseEventHandler(MidMouseEventHandler* handler)
	{
		if (handler) m_midMouseEventHandlers.push_back(handler);
	}

	void EventSubdivide::removeMidMouseEventHandler(MidMouseEventHandler* handler)
	{
		m_midMouseEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeMidMouseEventHandlers()
	{
		m_midMouseEventHandlers.clear();
	}


	void EventSubdivide::addLeftMouseEventHandler(LeftMouseEventHandler* handler)
	{
		if (handler) m_leftMouseEventHandlers.push_back(handler);
	}

	void EventSubdivide::removeLeftMouseEventHandler(LeftMouseEventHandler* handler)
	{
		m_leftMouseEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeLeftMouseEventHandlers()
	{
		m_leftMouseEventHandlers.clear();
	}

	void EventSubdivide::addKeyEventHandler(KeyEventHandler* handler)
	{
		if (handler) m_KeyEventHandlers.push_back(handler);
	}

	void EventSubdivide::removeKeyEventHandler(KeyEventHandler* handler)
	{
		m_KeyEventHandlers.removeOne(handler);
	}

	void EventSubdivide::closeKeyEventHandlers()
	{
		m_KeyEventHandlers.clear();
	}
}
