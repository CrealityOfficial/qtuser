#ifndef _NULLSPACE_CAMERACONTROLLER_1588862127357_H
#define _NULLSPACE_CAMERACONTROLLER_1588862127357_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/event/eventhandlers.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class ScreenCamera;
	class CameraMouseManipulator;
}

class QTUSER_3D_API CameraController: public QObject
	, public qtuser_3d::WheelEventHandler
	, public qtuser_3d::RightMouseEventHandler
	, public qtuser_3d::MidMouseEventHandler
	, public qtuser_3d::ResizeEventHandler
{
	Q_OBJECT
public:
	CameraController(QObject* parent = nullptr);
	virtual ~CameraController();

	void setScreenCamera(qtuser_3d::ScreenCamera* camera);

	Q_INVOKABLE void viewFromBottom();
	Q_INVOKABLE void viewFromTop();
	Q_INVOKABLE void viewFromLeft();
	Q_INVOKABLE void viewFromRight();
	Q_INVOKABLE void viewFromFront();
	Q_INVOKABLE void viewFromBack();

	Q_INVOKABLE void viewFromOrthographic();
	Q_INVOKABLE void viewFromPerspective();

	void uninitialize();
	void fittingBoundingBox(const qtuser_3d::Box3D& box);
	void home(const qtuser_3d::Box3D& box);
protected:
	void onResize(const QSize& size) override;

	void onRightMouseButtonPress(QMouseEvent* event) override;
	void onRightMouseButtonRelease(QMouseEvent* event) override;
	void onRightMouseButtonMove(QMouseEvent* event) override;
	void onRightMouseButtonClick(QMouseEvent* event) override;

	void onMidMouseButtonPress(QMouseEvent* event) override;
	void onMidMouseButtonRelease(QMouseEvent* event) override;
	void onMidMouseButtonMove(QMouseEvent* event) override;
	void onMidMouseButtonClick(QMouseEvent* event) override;

	void onWheelEvent(QWheelEvent* event) override;

protected:
	void view(const QVector3D& dir, const QVector3D& right);
signals:
	void signalViewChanged(bool capture);
protected:
	qtuser_3d::CameraMouseManipulator* m_cameraManipulator;
	qtuser_3d::ScreenCamera* m_screenCamera;
};
#endif // _NULLSPACE_CAMERACONTROLLER_1588862127357_H
