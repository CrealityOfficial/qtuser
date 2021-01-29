#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/math/rectutil.h"
#include "qtuser3d/math/angles.h"
#include "qtuser3d/math/space3d.h"

#include <QtMath>

namespace qtuser_3d
{
	ScreenCamera::ScreenCamera(QObject* parent)
		:QObject(parent)
		,m_camera(nullptr)
		, m_minDistance(1.0f)
		, m_maxDistance(4000.0f)
	{
		m_camera = new Qt3DRender::QCamera();
		m_camera->lens()->setPerspectiveProjection(30.0f, 16.0f / 9.0f, 0.1f, 3000.0f);
		m_camera->setPosition(QVector3D(0.0f, -40.0f, 0.0f));
		m_camera->setViewCenter(QVector3D(0.0f, 0.0f, 0.0f));
		m_camera->setUpVector(QVector3D(0.0f, 0.0f, 1.0f));
	}

	ScreenCamera::~ScreenCamera()
	{
		if (!m_camera->parent())
		{
			delete m_camera;
			m_camera = nullptr;
		}
	}

	Qt3DRender::QCamera* ScreenCamera::camera()
	{
		return m_camera;
	}

	void ScreenCamera::setScreenSize(const QSize& size)
	{
		m_size = size;

		if (m_size.height() != 0)
		{
			float aspectRatio = (float)m_size.width() / (float)m_size.height();
			m_camera->setAspectRatio(aspectRatio);
		}
	}

	QSize ScreenCamera::size()
	{
		return m_size;
	}

	void ScreenCamera::fittingBoundingBox(const qtuser_3d::Box3D& box)
	{
		QVector3D bsize = box.size();
        float len = qMax(bsize.x(), qMax(bsize.y(), bsize.z()));
		QVector3D center = box.center();
		center.setZ(center.z() - box.size().z() * 0.4);

		//float nearPlane = 0.2f * len;
		//float farPlane = 10.0f * len;

		//osg::Vec3f delta = m_position - m_focus;
		float newDistance = 2.0f * len;

		QVector3D cameraPosition = m_camera->position();
		QVector3D cameraCenter = m_camera->viewCenter();
		QVector3D cameraView = cameraCenter - cameraPosition;
		cameraView.normalize();

		QVector3D newPosition = center - cameraView * newDistance;
		m_camera->setPosition(newPosition);
		m_camera->setViewCenter(center);
		//m_camera->setNearPlane(nearPlane);
		//m_camera->setFarPlane(farPlane);

		m_box = box;
		float dmax = viewAllLen(m_box.size().length() / 2.0f) * 1.5;
		setMaxLimitDistance(dmax);

		_updateNearFar(m_box);
	}

	void ScreenCamera::updateNearFar(const qtuser_3d::Box3D& box)
	{
		m_box = box;

		_updateNearFar(m_box);
	}

	void ScreenCamera::updateNearFar()
	{
		_updateNearFar(m_box);
	}

	void ScreenCamera::_updateNearFar(const qtuser_3d::Box3D& box)
	{
		QVector3D cameraPosition = m_camera->position();
		QVector3D cameraCenter = m_camera->viewCenter();
		QVector3D cameraView = cameraCenter - cameraPosition;
		cameraView.normalize();

		QVector3D center = box.center();
		float r = box.size().length() / 2.0f;
		float d = QVector3D::dotProduct(cameraView, center - cameraPosition);
		float dmin = d - 1.2f * r;
		float dmax = d + 1.2f * r;

		float fieldofview = m_camera->lens()->fieldOfView();
		float aspect = m_camera->lens()->aspectRatio();

		float nearpos = dmin < 1.0f ? 1.0f : dmin;
		float farpos = dmax > 0.0f ? dmax : 3000.0f;

		m_camera->lens()->setPerspectiveProjection(fieldofview, aspect, nearpos, farpos);
	}

	qtuser_3d::Ray ScreenCamera::screenRay(const QPoint& point)
	{
		QPointF p = qtuser_3d::viewportRatio(point, m_size);
		return screenRay(p);
	}

	qtuser_3d::Ray ScreenCamera::screenRay(const QPointF& point)
	{
		QVector3D position = m_camera->position();
		QVector3D center = m_camera->viewCenter();
		QVector3D dir = (center - position).normalized();
		QVector3D nearCenter = position + dir * m_camera->nearPlane();
		QVector3D left = QVector3D::crossProduct(dir, m_camera->upVector());
		left.normalize();
        float near_height = 2.0f * m_camera->nearPlane() * qTan(m_camera->fieldOfView() * 3.1415926f / 180.0f / 2.0f);
		float near_width = near_height * m_camera->aspectRatio();
		QVector3D nearPoint = nearCenter + m_camera->upVector() * point.y() * near_height + left * point.x() * near_width;

		qtuser_3d::Ray ray;
		ray.start = m_camera->position();
		ray.dir = nearPoint - ray.start;
		ray.dir.normalize();
		return ray;
	}

	float ScreenCamera::screenSpaceRatio(const QVector3D& position)
	{
		float ratio = 1.0f;
		if (m_camera)
		{
			float nearPlane = m_camera->nearPlane();
			float positionPlane = nearPlane;

			float h = 2.0f * nearPlane * tanf(m_camera->fieldOfView() * M_PI / 2.0f / 180.0f);
			QVector3D cameraCenter = m_camera->position();
			QVector3D cameraView = m_camera->viewCenter() - cameraCenter;
			cameraView.normalize();
			positionPlane = QVector3D::dotProduct(position - cameraCenter, cameraView);

			ratio = positionPlane * h / nearPlane / (float)m_size.height();
		}

		return ratio;
	}

	float ScreenCamera::viewAllLen(float r)
	{
		float len = r / (qSin(m_camera->fieldOfView() * 3.1415926f / 180.0f / 2.0f));
		return len;
	}

	void ScreenCamera::setMaxLimitDistance(float dmax)
	{
		m_maxDistance = dmax;
	}

	void ScreenCamera::setMinLimitDistance(float dmin)
	{
		m_minDistance = dmin;
	}

	bool ScreenCamera::zoom(float scale)
	{
		float factor = scale;

		QVector3D cameraPosition = m_camera->position();
		QVector3D cameraCenter = m_camera->viewCenter();
		QVector3D cameraView = cameraCenter - cameraPosition;
		float distance = cameraView.length();
		float newDistance = factor * distance;

		cameraView.normalize();
		if ((newDistance > m_minDistance) && (newDistance < m_maxDistance))
		{
			QVector3D newPosition = cameraCenter - cameraView * newDistance;
			m_camera->setPosition(newPosition);

			notifyCameraChanged();

			_updateNearFar(m_box);
			return true;
		}
		return false;
	}

	bool ScreenCamera::translate(const QVector3D& trans)
	{
		QVector3D cameraPosition = m_camera->position();
		QVector3D viewCenter = m_camera->viewCenter();
		cameraPosition += trans;
		viewCenter += trans;

		m_camera->setPosition(cameraPosition);
		m_camera->setViewCenter(viewCenter);

		_updateNearFar(m_box);
		return true;
	}

	bool ScreenCamera::rotate(const QVector3D& axis, float angle)
	{
		QQuaternion q = QQuaternion::fromAxisAndAngle(axis, angle);
		return rotate(q);
	}

	bool ScreenCamera::rotate(const QQuaternion& q)
	{
		QVector3D up = m_camera->upVector();
		QVector3D viewCenter = m_camera->viewCenter();
		QVector3D position = m_camera->position();
		QVector3D dir = viewCenter - position;
		float distance = dir.length();

		dir.normalize();
		QVector3D newDir = q * dir;
		newDir.normalize();
		QVector3D newPosition = viewCenter - newDir * distance;

		QVector3D newUp = q * up;
		newUp.normalize();
		m_camera->setUpVector(newUp);
		m_camera->setPosition(newPosition);

		_updateNearFar(m_box);
		return true;
	}

	bool ScreenCamera::testCameraValid()
	{
		return (m_size.width() != 0) && (m_size.height() != 0);
	}

	QPoint ScreenCamera::flipY(const QPoint pos)
	{
		QPoint point(-1, -1);
		if (testCameraValid())
		{
			point.setX(pos.x());
			point.setY(qtuser_3d::adjustY(pos.y(), m_size));
		}
		return point;
	}

	void ScreenCamera::home(const qtuser_3d::Box3D& box, int type)
	{
		QVector3D size = box.size();
		QVector3D center = box.center();
		center.setZ(center.z() - box.size().z() * 0.4);

		QVector3D dir = QVector3D(1.0f, 1.0f, -1.0f);
		dir.normalize();
		QVector3D right = QVector3D(1.0f, -1.0f, 0.0f);
		right.normalize();
		QVector3D up = QVector3D::crossProduct(right, dir);

		if (type == 1)
		{
			dir = QVector3D(0.0f, 1.0f, -1.0f);
			dir.normalize();
			right = QVector3D(1.0f, 0.0f, 0.0f);
			right.normalize();
			up = QVector3D::crossProduct(right, dir);
		}

		float len = viewAllLen(size.length() / 2.0f);
		float dmax = len*1.5;
		QVector3D position = center - len * dir;
		m_camera->setViewCenter(center);
		m_camera->setUpVector(up);
		m_camera->setPosition(position);

		m_box = box;
		setMaxLimitDistance(dmax);
		_updateNearFar(m_box);

		notifyCameraChanged();
	}

	void ScreenCamera::showSelf() const
	{
		if (m_camera != nullptr)
		{
			qDebug() << "show ScreenCamera:";
			qDebug() << m_camera->position() << " * " << m_camera->viewCenter() << " * " << m_camera->upVector();
		}
	}

	void ScreenCamera::viewFrom(const QVector3D& dir, const QVector3D& right)
	{
		QVector3D newUp = QVector3D::crossProduct(right, dir);
		newUp.normalize();

		QVector3D viewCenter = m_camera->viewCenter();
		QVector3D position = m_camera->position();
		float d = (viewCenter - position).length();
		QVector3D newPosition = viewCenter - dir.normalized() * d;

		m_camera->setUpVector(newUp);
		m_camera->setPosition(newPosition);
	}

	QVector3D ScreenCamera::horizontal()
	{
		QVector3D viewCenter = m_camera->viewCenter();
		QVector3D position = m_camera->position();
		QVector3D dir = viewCenter - position;
		dir.normalize();
		QVector3D up = m_camera->upVector();

		QVector3D h = QVector3D::crossProduct(dir, up);
		h.setZ(0.0f);
		if (h.length() > 0.0f)
		{
			h.normalize();
		}
		if (h.length() == 0.0f)
		{
			qDebug() << "error";
		}
		return h;
	}

	QVector3D ScreenCamera::vertical()
	{
		QVector3D v = QVector3D(0.0f, 1.0f, 0.0f);
		QVector3D viewCenter = m_camera->viewCenter();
		QVector3D position = m_camera->position();
		QVector3D dir = viewCenter - position;
		float y = QVector3D::dotProduct(v, dir);
		float z = qSqrt(dir.lengthSquared() - y * y);
		QVector3D d(0.0f, y, z);
		if (d.lengthSquared() > 0.0f)
			v = d.normalized();
		return v;
	}

	float ScreenCamera::verticalAngle()
	{
		QVector3D v = QVector3D(0.0f, 0.0f, 1.0f);
		QVector3D viewCenter = m_camera->viewCenter();
		QVector3D position = m_camera->position();
		QVector3D dir = viewCenter - position;
		dir.normalize();
		float angle = acosf(QVector3D::dotProduct(v, dir));
		return angle;
	}

	void ScreenCamera::addCameraObserver(ScreenCameraObserver* observer)
	{
		if (observer)
		{
			m_cameraObservers.push_back(observer);
		}
	}

	void ScreenCamera::removeCameraObserver(ScreenCameraObserver* observer)
	{
		m_cameraObservers.removeOne(observer);
	}

	void ScreenCamera::clearCameraObservers()
	{
		m_cameraObservers.clear();
	}

	void ScreenCamera::notifyCameraChanged()
	{
		for (ScreenCameraObserver* observer : m_cameraObservers)
			observer->onCameraChanged(this);
	}

	bool cameraRayPoint(ScreenCamera* camera, QPoint point, QVector3D& planePosition, QVector3D& planeNormal, QVector3D& collide)
	{
		if (camera)
		{
			Ray ray = camera->screenRay(point);
			return lineCollidePlane(planePosition, planeNormal, ray, collide);
		}

		return false;
	}
}
