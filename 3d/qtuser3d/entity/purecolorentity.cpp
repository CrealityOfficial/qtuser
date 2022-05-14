#include "purecolorentity.h"

#include "qtuser3d/effect/effectmanager.h"
namespace qtuser_3d
{
	PureColorEntity::PureColorEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		setEffect(EFFECT("pure"));
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
	}

	PureColorEntity::~PureColorEntity()
	{
	}

	void PureColorEntity::setColor(QVector4D color)
	{
		m_colorParameter->setValue(color);
	}
}
