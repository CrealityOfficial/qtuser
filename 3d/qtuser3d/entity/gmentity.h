#ifndef QTUSER_3D_GMENTITY_1592184697882_H
#define QTUSER_3D_GMENTITY_1592184697882_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/math/box3d.h"
#include <QtGui/QMatrix4x4>
#include <Qt3DCore/QTransform>

namespace qtuser_3d
{
	class QTUSER_3D_API GMEntity : public PickableEntity
	{
		Q_OBJECT
	public:
		GMEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~GMEntity();

		QVector3D center();
		void setCenter(const QVector3D& center, bool update = true);

		void setLocalScale(const QVector3D& scale, bool update = true);
		void resetLocalScale(bool update = true);
		QVector3D localScale();

		QQuaternion localQuaternion();
		void setLocalQuaternion(const QQuaternion& q, bool update = true);
		void resetLocalQuaternion(bool update = true);

		void setLocalPosition(const QVector3D& position, bool update = true);
		void resetLocalPosition(bool update = true);
		QVector3D localPosition();

		void mirrorX();
		void mirrorY();
		void mirrorZ();
		void mirrorSet(const QMatrix4x4& m);
		QMatrix4x4 mirrorMatrix();

		void updateMatrix();

		qtuser_3d::Box3D localBox();
		qtuser_3d::Box3D globalSpaceBox();

		QMatrix4x4 parent2Global();
		QMatrix4x4 globalMatrix();
		QMatrix4x4 localMatrix();

		void setParent2Global(const QMatrix4x4& matrix);

		QVector3D mapGlobal2Local(QVector3D position);  // map global bottom center to position
	protected:
		void parentGlobalChanged(const QMatrix4x4& parent2Global);
		void notifyGlobalChanged(const QMatrix4x4& globalMatrix);

		virtual void onGlobalMatrixChanged(const QMatrix4x4& globalMatrix);
		virtual void onLocalMatrixChanged(const QMatrix4x4& localMatrix);

		void mirror(const QMatrix4x4& matrix, bool apply = true);
	protected:
		virtual qtuser_3d::Box3D calculateGlobalSpaceBox();
		void updateGlobalSpaceBox();
	protected:
		QVector3D m_localCenter;

		QVector3D m_localPosition;
		QVector3D m_localScale;
		QQuaternion m_localRotate;

		qtuser_3d::Box3D m_globalSpaceBox;    // parent space box
		qtuser_3d::Box3D m_localBox;

		QMatrix4x4 m_local2Parent;
		QMatrix4x4 m_parent2Global;
		bool m_localMatrixDirty;
		bool m_parentMatrixDirty;
		QMatrix4x4 m_globalMatrix;

		QMatrix4x4 m_mirrorMatrix;
	};
}
#endif // QTUSER_3D_GMENTITY_1592184697882_H