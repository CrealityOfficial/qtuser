#ifndef TRANSLATE_HELPER_ENTITY_H
#define TRANSLATE_HELPER_ENTITY_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QEntity>

#include "qtuser3d/math/box3d.h"
namespace qtuser_3d
{
	class Pickable;
	class SimplePickable;
	class ManipulateEntity;
	class QTUSER_3D_API TranslateHelperEntity : public Qt3DCore::QEntity
	{
		Q_OBJECT
	public:
		TranslateHelperEntity(Qt3DCore::QNode* parent = nullptr, int type = 7, int shapetype = 0, QVector3D dir = QVector3D(1, 1, 1));
		virtual ~TranslateHelperEntity();

		Pickable* xPickable();
		Pickable* yPickable();
		Pickable* zPickable();

		void setXVisibility(bool visibility);
		void setYVisibility(bool visibility);
		void setZVisibility(bool visibility);

		void setScale(float scaleRate);
		void updateBox(const Box3D& box);
		void setFixSize(int fixSize);
		QVector3D center();
		void setDirColor(QVector4D v4, int nDir);
	protected:
		Qt3DCore::QTransform* m_transform;

		ManipulateEntity* m_xArrowEntity;
		ManipulateEntity* m_yArrowEntity;
		ManipulateEntity* m_zArrowEntity;
		SimplePickable* m_xPickable;
		SimplePickable* m_yPickable;
		SimplePickable* m_zPickable;

		int m_fixSize;
		QVector3D m_center;
	};
}

#endif // TRANSLATE_HELPER_ENTITY_H