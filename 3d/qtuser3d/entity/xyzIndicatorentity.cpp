#include "xyzIndicatorentity.h"

#include "qtuser3d/effect/effectmanager.h"
namespace qtuser_3d
{
	XyzIndicatorEntity::XyzIndicatorEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		setEffect(EFFECT("xyzIndicator"));
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
	}

	XyzIndicatorEntity::~XyzIndicatorEntity()
	{
	}

	void XyzIndicatorEntity::setColor(QVector4D color)
	{
		m_colorParameter->setValue(color);
	}
}
