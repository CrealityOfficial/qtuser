#ifndef _QTUSER_3D_TRACKBALLCAMERAMANIPULATOR_1588840518365_H
#define _QTUSER_3D_TRACKBALLCAMERAMANIPULATOR_1588840518365_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/camera/cameramousemanipulator.h"

namespace qtuser_3d
{
	class QTUSER_3D_API TrackballCameraManipulator: public CameraMouseManipulator
	{
		Q_OBJECT
	public:
		TrackballCameraManipulator(QObject* parent = nullptr);
		virtual ~TrackballCameraManipulator();

		void setRightHorizontal(bool horizontal);
	protected:
		void performRotate(const QPoint& p);
	protected:
		void onCameraChanged() override;

		void onRightMouseButtonPress(QMouseEvent* event) override;
		void onRightMouseButtonMove(QMouseEvent* event) override;

	protected:
		bool m_RightHorizontal;
	};
}
#endif // _QTUSER_3D_TRACKBALLCAMERAMANIPULATOR_1588840518365_H
