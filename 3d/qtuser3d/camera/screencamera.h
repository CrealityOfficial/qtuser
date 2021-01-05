#ifndef _QTUSER_3D_SCREENCAMERA_1588839492087_H
#define _QTUSER_3D_SCREENCAMERA_1588839492087_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/math/ray.h"
#include <Qt3DRender/QCamera>

namespace qtuser_3d
{
	class ScreenCamera;
	class ScreenCameraObserver
	{
	public:
		virtual ~ScreenCameraObserver() {}

		virtual void onCameraChanged(ScreenCamera* camera) = 0;
	};

	class QTUSER_3D_API ScreenCamera: public QObject
	{
	public:
		ScreenCamera(QObject* parent = nullptr);
		virtual ~ScreenCamera();

		Qt3DRender::QCamera* camera();
		void setScreenSize(const QSize& size);
		QSize size();

		void fittingBoundingBox(const qtuser_3d::Box3D& box);
		void updateNearFar(const qtuser_3d::Box3D& box);
		void updateNearFar();

		qtuser_3d::Ray screenRay(const QPoint& point);
		qtuser_3d::Ray screenRay(const QPointF& point);
		float screenSpaceRatio(const QVector3D& position);
		float viewAllLen(float r);

		void setMaxLimitDistance(float dmax);
		void setMinLimitDistance(float dmin);

		bool zoom(float scale);
		bool translate(const QVector3D& trans);
		bool rotate(const QVector3D& axis, float angle);
		bool rotate(const QQuaternion& q);

		void home(const qtuser_3d::Box3D& box, int type = 0);

		void showSelf() const;

		bool testCameraValid();
		QPoint flipY(const QPoint pos);

		void viewFrom(const QVector3D& dir, const QVector3D& right);

		QVector3D horizontal();
		QVector3D vertical();
		float verticalAngle();

		void addCameraObserver(ScreenCameraObserver* observer);
		void removeCameraObserver(ScreenCameraObserver* observer);
		void clearCameraObservers();
		void notifyCameraChanged();
	protected:
		void _updateNearFar(const qtuser_3d::Box3D& box);
	protected:
		Qt3DRender::QCamera* m_camera;
		QSize m_size;

		float m_minDistance;
		float m_maxDistance;
		qtuser_3d::Box3D m_box;

		QList<ScreenCameraObserver*> m_cameraObservers;
	};

	QTUSER_3D_API bool cameraRayPoint(ScreenCamera* camera, QPoint point, QVector3D& planePosition, QVector3D& planeNormal, QVector3D& collide);
}
#endif // _QTUSER_3D_SCREENCAMERA_1588839492087_H
