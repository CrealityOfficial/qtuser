#include "purecolorentity.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	PureColorEntity::PureColorEntity(Qt3DCore::QNode* parent)
		:XEntity(parent)
	{
		XRenderPass* pass = new XRenderPass("pure", this);
		pass->addFilterKeyMask("view", 0);
		XEffect* effect = new XEffect(this);
		effect->addRenderPass(pass);
		setEffect(effect);

		m_colorParameter = setParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
	}

	PureColorEntity::~PureColorEntity()
	{
	}

	void PureColorEntity::setColor(QVector4D color)
	{
		m_colorParameter->setValue(color);
	}
}
