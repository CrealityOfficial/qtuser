#ifndef _QTUSER_3D_EULARMOUSEMANIPULATOR_1590807908153_H
#define _QTUSER_3D_EULARMOUSEMANIPULATOR_1590807908153_H
#include "qtuser3d/camera/cameramousemanipulator.h"

namespace qtuser_3d
{
	class QTUSER_3D_API EularMouseManipulator: public CameraMouseManipulator
	{
	public:
		EularMouseManipulator(QObject* parent = nullptr);
		virtual ~EularMouseManipulator();

	protected:
		void onRightMouseButtonPress(QMouseEvent* event) override;
		void onRightMouseButtonMove(QMouseEvent* event) override;

		void onMidMouseButtonPress(QMouseEvent* event) override;
		void onMidMouseButtonMove(QMouseEvent* event) override;

		void performTranslate(const QPoint& pos);
		void performRotate(const QPoint& pos);
	protected:
	};
}
#endif // _QTUSER_3D_EULARMOUSEMANIPULATOR_1590807908153_H
