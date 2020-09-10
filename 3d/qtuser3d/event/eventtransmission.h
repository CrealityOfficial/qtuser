#ifndef _qtuser_3d_EVENTTRANSMISSION_1588734351555_H
#define _qtuser_3d_EVENTTRANSMISSION_1588734351555_H
#include "qtuser3d/qtuser3dexport.h"
#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>
#include <QtGui/QHoverEvent>
#include <QtGui/QWheelEvent>

namespace qtuser_3d
{
	class KeyTransmission
	{
	public:
		virtual ~KeyTransmission() {}
	};

	class WheelTransmission
	{
	public:
		virtual ~WheelTransmission() {}

		virtual void wheelEvent(QWheelEvent* event) = 0;
	};

	class HoverTransmission
	{
	public:
		virtual ~HoverTransmission() {}
		virtual void hoverEvent(QHoverEvent* event) = 0;
	};

	class MouseTransmision
	{
	public:
		virtual ~MouseTransmision() {}
	};

	class QTUSER_3D_API EventTransmission: public QObject
	{
	public:
		EventTransmission(QObject* parent = nullptr);
		virtual ~EventTransmission();

	protected:
		bool eventFilter(QObject* watched, QEvent* event) override;

	private:
		bool processKeyPressEvent(QKeyEvent* event);
		bool processKeyReleaseEvent(QKeyEvent* event);
		bool processMousePressEvent(QMouseEvent* event);
		bool processMouseReleaseEvent(QMouseEvent* event);
		bool processHoverEvent(QHoverEvent* event);
		bool processWheelEvent(QWheelEvent* event);
	};
}
#endif // _qtuser_3d_EVENTTRANSMISSION_1588734351555_H
