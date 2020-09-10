#include "gmentity.h"

namespace qtuser_3d
{
	GMEntity::GMEntity(Qt3DCore::QNode* parent)
		:PickableEntity(parent)
		, m_localMatrixDirty(true)
		, m_parentMatrixDirty(true)
	{
		m_localScale = QVector3D(1.0f, 1.0f, 1.0f);
	}
	
	GMEntity::~GMEntity()
	{
	}

	void GMEntity::parentGlobalChanged(const QMatrix4x4& parent2Global)
	{
		m_parent2Global = parent2Global;
		m_parentMatrixDirty = true;

		updateMatrix();
	}

	void GMEntity::notifyGlobalChanged(const QMatrix4x4& globalMatrix)
	{
		updateGlobalSpaceBox();
		onGlobalMatrixChanged(globalMatrix);

		QList<GMEntity*> entities = findChildren<GMEntity*>(QString(), Qt::FindDirectChildrenOnly);
		for (GMEntity* entity : entities)
		{
			entity->parentGlobalChanged(m_globalMatrix);
		}
	}

	void GMEntity::onGlobalMatrixChanged(const QMatrix4x4& globalMatrix)
	{

	}

	void GMEntity::onLocalMatrixChanged(const QMatrix4x4& localMatrix)
	{

	}

	QVector3D GMEntity::center()
	{
		return m_localCenter;
	}

	void GMEntity::setCenter(const QVector3D& center, bool update)
	{
		m_localCenter = center;

		if (update) updateMatrix();
	}

	void GMEntity::setLocalScale(const QVector3D& scale, bool update)
	{
		m_localScale = scale;
		m_localMatrixDirty = true;

		if (update) updateMatrix();
	}

	void GMEntity::resetLocalScale(bool update)
	{
		setLocalScale(QVector3D(1.0f, 1.0f, 1.0f), update);
	}

	QVector3D GMEntity::localScale()
	{
		return m_localScale;
	}

	QQuaternion GMEntity::localQuaternion()
	{
		return m_localRotate;
	}

	void GMEntity::setLocalQuaternion(const QQuaternion& q, bool update)
	{
		m_localRotate = q;
		m_localMatrixDirty = true;

		if (update) updateMatrix();
	}

	void GMEntity::resetLocalQuaternion(bool update)
	{
		setLocalQuaternion(QQuaternion(), update);
	}

	void GMEntity::setLocalPosition(const QVector3D& position, bool update)
	{
		m_localPosition = position;
		m_localMatrixDirty = true;

		if (update) updateMatrix();
	}

	void GMEntity::resetLocalPosition(bool update)
	{
		setLocalPosition(QVector3D(0.0f, 0.0f, 0.0f), update);
	}

	QVector3D GMEntity::localPosition()
	{
		return m_localPosition;
	}

	void GMEntity::updateMatrix()
	{
		if (!m_localMatrixDirty && !m_parentMatrixDirty)
			return;

		m_local2Parent.setToIdentity();
		m_local2Parent.translate(m_localPosition + m_localCenter);

		m_local2Parent *= m_mirrorMatrix;

		m_local2Parent.rotate(m_localRotate);
		m_local2Parent.scale(m_localScale);
		m_local2Parent.translate(-m_localCenter);

		m_transform->setMatrix(m_local2Parent);
		onLocalMatrixChanged(m_local2Parent);

		m_globalMatrix = m_parent2Global * m_local2Parent;
		notifyGlobalChanged(m_globalMatrix);

		m_localMatrixDirty = false;
		m_parentMatrixDirty = false;
	}

	qtuser_3d::Box3D GMEntity::localBox()
	{
		return m_localBox;
	}

	qtuser_3d::Box3D GMEntity::globalSpaceBox()
	{
		return m_globalSpaceBox;
	}

	qtuser_3d::Box3D GMEntity::calculateGlobalSpaceBox()
	{
		return qtuser_3d::Box3D();
	}

	void GMEntity::updateGlobalSpaceBox()
	{
		m_globalSpaceBox = calculateGlobalSpaceBox();
	}

	QMatrix4x4 GMEntity::parent2Global()
	{
		return m_parent2Global;
	}

	QMatrix4x4 GMEntity::globalMatrix()
	{
		return m_globalMatrix;
	}

	QMatrix4x4 GMEntity::localMatrix()
	{
		return m_local2Parent;
	}

	void GMEntity::setParent2Global(const QMatrix4x4& matrix)
	{
		if (m_parent2Global != matrix)
		{
			m_parent2Global = matrix;
			m_parentMatrixDirty = true;

			updateMatrix();
		}
	}

	void GMEntity::mirrorX()
	{
		QMatrix4x4 m;
		m(0, 0) = -1;
		mirror(m, true);
	}

	void GMEntity::mirrorY()
	{
		QMatrix4x4 m;
		m(1, 1) = -1;
		mirror(m, true);
	}

	void GMEntity::mirrorZ()
	{
		QMatrix4x4 m;
		m(2, 2) = -1;
		mirror(m, true);
	}

	void GMEntity::mirrorSet(const QMatrix4x4& m)
	{
		mirror(m, false);
	}

	QMatrix4x4 GMEntity::mirrorMatrix()
	{
		return m_mirrorMatrix;
	}

	void GMEntity::mirror(const QMatrix4x4& matrix, bool apply)
	{
		if (apply)
		{
			m_mirrorMatrix = matrix * m_mirrorMatrix;
		}
		else
		{
			m_mirrorMatrix = matrix;
		}

		m_localMatrixDirty = true;
		updateMatrix();
	}

	QVector3D GMEntity::mapGlobal2Local(QVector3D position)
	{
		qtuser_3d::Box3D box = globalSpaceBox();
		QVector3D size = box.size();
		QVector3D center = box.center();
		center.setZ(center.z() - size.z() / 2.0f);

		QVector3D offset = position - center;
		QVector3D localPos = localPosition();
		return localPos + offset;
	}
}