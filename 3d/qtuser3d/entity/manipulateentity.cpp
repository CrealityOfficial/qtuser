#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent, bool alpha, bool pickable, bool depthTest, int alphalayer)
		:PickableEntity(parent)
	{
		QString showPassName = "manipulate";
		QString pickPassName = "";
		if (alpha)
		{
			if (alphalayer == 0)
			{
				showPassName += ".alpha";
			}
			else if (alphalayer == 1)
			{
				showPassName += ".alpha2nd";
			}
		}
		
		if (pickable)
		{
			pickPassName = "pickFace.pick2nd";
		}

		QString shader_type = showPassName + "_" + pickPassName;

		qtuser_3d::UEffect* effect = (qtuser_3d::UEffect*)EFFECTCREATE(shader_type, m_material);
		setEffect(effect);

		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
		m_changeColorParameter = createParameter("changecolor", QVector4D(0.0f, 0.0f, 0.0f, 0.0f));
		m_methodParameter = createParameter("mt", 0);
		m_lightEnableParameter = createParameter("lightEnable", 0);

		if (!depthTest)
		{
			effect->setPassDepthTest(showPassName, Qt3DRender::QDepthTest::Always);
			if (pickable)
				effect->setPassDepthTest(pickPassName, Qt3DRender::QDepthTest::Always);
		}
		else
		{
			effect->setPassDepthTest(showPassName, Qt3DRender::QDepthTest::DepthFunction::Less);
			if (pickable)
				effect->setPassDepthTest(pickPassName, Qt3DRender::QDepthTest::DepthFunction::Less);
		}

		setPassCullFace(showPassName, Qt3DRender::QCullFace::CullingMode::NoCulling);
		if (pickable) {
			setPassCullFace(pickPassName, Qt3DRender::QCullFace::CullingMode::NoCulling);
		}
		
		if (alpha) {
			setPassBlend(showPassName);
		}
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

	void ManipulateEntity::setLightEnable(bool flag)
	{
		m_lightEnableParameter->setValue(flag);
	}
}
