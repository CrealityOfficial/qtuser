#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent, int type)
		:PickableEntity(parent)
	{
		QString shader_type = "manipulate.alpha_pickFace.pick";
		if (type == 1)
		{
			shader_type = "manipulate.alpha";
		}
		else if (type == 2)
		{
			shader_type = "pickFace.pick";
		}

		qtuser_3d::UEffect* effect = (qtuser_3d::UEffect*)EFFECTCREATE(shader_type, m_material);
		setEffect(effect);

		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
		m_changeColorParameter = createParameter("changecolor", QVector4D(0.0f, 0.0f, 0.0f, 0.0f));
		m_methodParameter = createParameter("mt", 0);

		effect->setPassDepthTest("manipulate.alpha", Qt3DRender::QDepthTest::Always);
		effect->setPassDepthTest("pickFace.pick", Qt3DRender::QDepthTest::Always);

		setPassCullFace("manipulate.alpha", Qt3DRender::QCullFace::NoCulling);
		setPassCullFace("pickFace.pick", Qt3DRender::QCullFace::NoCulling);
	}

	ManipulateEntity::~ManipulateEntity()
	{

	}

	void ManipulateEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}

	void ManipulateEntity::setChangeColor(const QVector4D& color)
	{
		m_changeColorParameter->setValue(color);
	}

	void ManipulateEntity::setMethod(int mt)
	{
		m_methodParameter->setValue(mt);
	}
}
