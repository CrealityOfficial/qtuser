#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/effect/effectmanager.h"

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent)
		:PickableEntity(parent)
	{
		setEffect(EFFECT("manipulate_pickFace.pick"));
		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
	}

	ManipulateEntity::~ManipulateEntity()
	{

	}

	void ManipulateEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}
