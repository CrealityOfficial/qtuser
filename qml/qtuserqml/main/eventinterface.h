#ifndef CREATIVE_KERNEL_EVENTINTERFACE_1594436854496_H
#define CREATIVE_KERNEL_EVENTINTERFACE_1594436854496_H
#include "qtuser3d/event/eventhandlers.h"

void addResizeEventHandler(qtuser_3d::ResizeEventHandler* handler);
void removeResizeEventHandler(qtuser_3d::ResizeEventHandler* handler);
void closeResizeEventHandlers();

void addHoverEventHandler(qtuser_3d::HoverEventHandler* handler);
void removeHoverEventHandler(qtuser_3d::HoverEventHandler* handler);
void closeHoverEventHandlers();

void addWheelEventHandler(qtuser_3d::WheelEventHandler* handler);
void removeWheelEventHandler(qtuser_3d::WheelEventHandler* handler);
void closeWheelEventHandlers();

void addRightMouseEventHandler(qtuser_3d::RightMouseEventHandler* handler);
void removeRightMouseEventHandler(qtuser_3d::RightMouseEventHandler* handler);
void closeRightMouseEventHandlers();

void addMidMouseEventHandler(qtuser_3d::MidMouseEventHandler* handler);
void removeMidMouseEventHandler(qtuser_3d::MidMouseEventHandler* handler);
void closeMidMouseEventHandlers();

void addLeftMouseEventHandler(qtuser_3d::LeftMouseEventHandler* handler);
void removeLeftMouseEventHandler(qtuser_3d::LeftMouseEventHandler* handler);
void closeLeftMouseEventHandlers();

void addKeyEventHandler(qtuser_3d::KeyEventHandler* handler);
void removeKeyEventHandler(qtuser_3d::KeyEventHandler* handler);
void closeKeyEventHandlers();

#endif // CREATIVE_KERNEL_EVENTINTERFACE_1594436854496_H