#ifndef TRANSLATE_HELPER_ENTITY_H
#define TRANSLATE_HELPER_ENTITY_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QEntity>

namespace qtuser_3d
{
	class PickableEntity;
	class ManipulateEntity;
	class QTUSER_3D_API ScaleHelperEntity : public Qt3DCore::QEntity
	{
	public:
		ScaleHelperEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~ScaleHelperEntity();

		PickableEntity* xEntity();
		PickableEntity* yEntity();
		PickableEntity* zEntity();
	protected:
		Qt3DCore::QTransform* m_transform;

		ManipulateEntity* m_xArrowEntity;
		ManipulateEntity* m_yArrowEntity;
		ManipulateEntity* m_zArrowEntity;
	};
}

#endif // TRANSLATE_HELPER_ENTITY_H