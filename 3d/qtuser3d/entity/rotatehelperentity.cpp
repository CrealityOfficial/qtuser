#include "qtuser3d/entity/rotatehelperentity.h"

#include "qtuser3d/module/manipulatecallback.h"

#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/module/manipulatepickable.h"

#include "qtuser3d/module/facepicker.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/math/angles.h"

#include <Qt3DExtras/QTorusMesh>
#include <QtCore/qmath.h>

namespace qtuser_3d
{
	RotateHelperEntity::RotateHelperEntity(Qt3DCore::QNode* parent)
		:Qt3DCore::QEntity(parent)
		, m_pickSource(nullptr)
		, m_screenCamera(nullptr)
		, m_rotateMode(RotateMode::null)
		, m_rotateCallback(nullptr)
	{
		m_transform = new Qt3DCore::QTransform(this);
		addComponent(m_transform);

		m_xRingEntity = new ManipulateEntity(this);
		m_yRingEntity = new ManipulateEntity(this);
		m_zRingEntity = new ManipulateEntity(this);

		QMatrix4x4 xMatrix;
		xMatrix.rotate(-90.0f, 0.0f, 1.0f, 0.0f);
        xMatrix.scale(25.0f, 25.0f, 25.0f);
		QMatrix4x4 yMatrix;
		yMatrix.rotate(-90.0f, 1.0f, 0.0f, 0.0f);
        yMatrix.scale(25.0f, 25.0f, 25.0f);
		QMatrix4x4 zMatrix;
        zMatrix.scale(25.0f, 25.0f, 25.0f);
		m_xRingEntity->setPose(xMatrix);
		m_yRingEntity->setPose(yMatrix);
		m_zRingEntity->setPose(zMatrix);

		m_xRingEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		m_yRingEntity->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
		m_zRingEntity->setColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));

		{
			Qt3DExtras::QTorusMesh* torusMesh = new Qt3DExtras::QTorusMesh(this);
			torusMesh->setRadius(2);
			torusMesh->setMinorRadius(0.02f);
			torusMesh->setRings(100);
			m_xRingEntity->replaceGeometryRenderer(torusMesh);
		}
		{
			Qt3DExtras::QTorusMesh* torusMesh = new Qt3DExtras::QTorusMesh(this);
			torusMesh->setRadius(2);
			torusMesh->setMinorRadius(0.02f);
			torusMesh->setRings(100);
			m_yRingEntity->replaceGeometryRenderer(torusMesh);
		}
		{
			Qt3DExtras::QTorusMesh* torusMesh = new Qt3DExtras::QTorusMesh(this);
			torusMesh->setRadius(2);
			torusMesh->setMinorRadius(0.02f);
			torusMesh->setRings(100);
			m_zRingEntity->replaceGeometryRenderer(torusMesh);
		}

		m_xPickable = new ManipulatePickable(this);
		m_yPickable = new ManipulatePickable(this);
		m_zPickable = new ManipulatePickable(this);
		m_xPickable->setPickableEntity(m_xRingEntity);
		m_yPickable->setPickableEntity(m_yRingEntity);
		m_zPickable->setPickableEntity(m_zRingEntity);
	}

	RotateHelperEntity::~RotateHelperEntity()
	{

	}

	Pickable* RotateHelperEntity::xPickable()
	{
		return m_xPickable;
	}

	Pickable* RotateHelperEntity::yPickable()
	{
		return m_yPickable;
	}

	Pickable* RotateHelperEntity::zPickable()
	{
		return m_zPickable;
	}

	QList<Pickable*> RotateHelperEntity::pickables()
	{
		QList<Pickable*> results;
		results << m_xPickable << m_yPickable << m_zPickable;
		return results;
	}

	void RotateHelperEntity::setPickSource(FacePicker* pickSource)
	{
		m_pickSource = pickSource;
	}

	void RotateHelperEntity::setScreenCamera(ScreenCamera* camera)
	{
		m_screenCamera = camera;
	}

	void RotateHelperEntity::setRotateCallback(RotateCallback* callback)
	{
		m_rotateCallback = callback;
	}

	void RotateHelperEntity::onBoxChanged(Box3D box)
	{
		QVector3D center = box.center();
		m_center = center;
		QMatrix4x4 matrix;
		matrix.translate(center);
		m_transform->setMatrix(matrix);
	}

	void RotateHelperEntity::onLeftMouseButtonPress(QMouseEvent* event)
	{
		m_rotateMode = RotateMode::null;
		QList<Pickable*> list = pickables();
		Pickable* pickable = checkPickableFromList(m_pickSource, event->pos(), list, nullptr);
		if (pickable == m_xPickable) m_rotateMode = RotateMode::x;
		if (pickable == m_yPickable) m_rotateMode = RotateMode::y;
		if (pickable == m_zPickable) m_rotateMode = RotateMode::z;

		m_spacePoint = calculateSpacePoint(event->pos());

		if (m_rotateCallback && m_rotateMode != RotateMode::null) m_rotateCallback->onStartRotate();
	}

	void RotateHelperEntity::onLeftMouseButtonRelease(QMouseEvent* event)
	{
		if (m_rotateMode != RotateMode::null)
		{
			perform(event->pos(), true);
		}
	}

	void RotateHelperEntity::onLeftMouseButtonMove(QMouseEvent* event)
	{
		if (m_rotateMode != RotateMode::null)
		{
			perform(event->pos(), false);
		}
	}

	void RotateHelperEntity::onHoverMove(QHoverEvent* event)
	{

	}

	QVector3D RotateHelperEntity::calculateSpacePoint(QPoint point)
	{
		QVector3D collide;
		QVector3D planeNormal;
		QVector3D planePosition = m_center;

		if (m_rotateMode == RotateMode::x)  // x
		{
			planeNormal = QVector3D(1.0f, 0.0f, 0.0f);
		}
		else if (m_rotateMode == RotateMode::y)
		{
			planeNormal = QVector3D(0.0f, 1.0f, 0.0f);
		}
		else if (m_rotateMode == RotateMode::z)
		{
			planeNormal = QVector3D(0.0f, 0.0f, 1.0f);
		}

		cameraRayPoint(m_screenCamera, point, planePosition, planeNormal, collide);
		return collide;
	}

	QQuaternion RotateHelperEntity::process(QPoint point)
	{
		QVector3D p = calculateSpacePoint(point);

		QVector3D axis = QVector3D(0.0f, 0.0f, 1.0f);
		float angle = 0.0f;

		QVector3D delta;
		QVector3D oc0 = m_spacePoint - m_center;
		QVector3D oc1 = p - m_center;
		if (oc0.length() != 0.0f && oc1.length() != 0.0f)
		{
			oc0.normalize();
			oc1.normalize();

			if (oc0 == oc1)
			{
				angle = 0.0f;
			}
			else if (oc0 == -oc1)
			{
				angle = 180.0f;
			}
			else
			{
				axis = QVector3D::crossProduct(oc0, oc1);
				axis.normalize();
				angle = angleOfVector3D2(oc0, oc1);
			}
		}

		return QQuaternion::fromAxisAndAngle(axis, angle);
	}

	void RotateHelperEntity::perform(QPoint point, bool release)
	{
		QQuaternion q = process(point);

		if (m_rotateCallback)
		{
			if (release) m_rotateCallback->onEndRotate(q);
			else m_rotateCallback->onRotate(q);
		}
	}
}
