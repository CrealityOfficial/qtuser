#ifndef _creative_kernel_NODE3D_1590453972562_H
#define _creative_kernel_NODE3D_1590453972562_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/module/pickable.h"
#include "qtuser3d/math/box3d.h"
#include <QtGui/QMatrix4x4>

namespace qtuser_3d
{
	class QTUSER_3D_API Node3D : public Pickable
	{
		Q_OBJECT
	public:
		Node3D(QObject* parent = nullptr);
		Node3D(const Node3D& node);

		virtual ~Node3D();

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
		void _mirrorZ();
		void _mirrorSet(const QMatrix4x4& m);

		//for dlp
		void mirrorZ();
		int mirrorZ_count;
		void mirrorSet(const QMatrix4x4& m);

		QMatrix4x4 mirrorMatrix();

		void updateMatrix();

		Box3D localBox();
		Box3D globalSpaceBox();

		QMatrix4x4 parent2Global();
		QMatrix4x4 globalMatrix();
		QMatrix4x4 localMatrix();
		QMatrix4x4 objectMatrix();

		bool isFanZhuan();

		void setParent2Global(const QMatrix4x4& matrix);

		QVector3D mapGlobal2Local(QVector3D position);  // map global bottom center to position
		QVector3D mapGlobal2LocalAD(QVector3D position);  // map global bottom center to position

		void liftZUp(float z);  // z is global value
	protected:
		void parentGlobalChanged(const QMatrix4x4& parent2Global);
		void notifyGlobalChanged(const QMatrix4x4& globalMatrix);

		virtual void onGlobalMatrixChanged(const QMatrix4x4& globalMatrix);
		virtual void onLocalMatrixChanged(const QMatrix4x4& localMatrix);

		virtual void mirror(const QMatrix4x4& matrix, bool apply = true);
	protected:
		virtual Box3D calculateGlobalSpaceBox();
		void updateGlobalSpaceBox();
	protected:
		QVector3D m_localCenter;

		QVector3D m_localPosition;
		QVector3D m_localScale;
		QQuaternion m_localRotate;

		Box3D m_globalSpaceBox;    // parent space box
		Box3D m_localBox;

		QMatrix4x4 m_local2Parent;
		QMatrix4x4 m_parent2Global;
		bool m_localMatrixDirty;
		bool m_parentMatrixDirty;
		QMatrix4x4 m_globalMatrix;
		QMatrix4x4 m_objectMatrix;

		QMatrix4x4 m_mirrorMatrix;
	};
}
#endif // _creative_kernel_NODE3D_1590453972562_H
