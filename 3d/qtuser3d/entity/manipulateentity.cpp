#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent, bool alpha, bool pickable, bool depthTest, bool overlay)
		:PickXEntity(parent)
	{
		XRenderPass* showPass = new XRenderPass("manipulate", this);
		Qt3DRender::QFilterKey* showFilterKey = new Qt3DRender::QFilterKey(showPass);
		showFilterKey->setValue(0);
		showFilterKey->setName("view");
		if (alpha)
		{
			showFilterKey->setName("alpha");
		}
		if (overlay)
		{
			showFilterKey->setName("overlay");
		}
		showPass->addFilterKey(showFilterKey);
		showPass->setPassCullFace(Qt3DRender::QCullFace::CullingMode::NoCulling);
		showPass->setPassDepthTest(depthTest ? Qt3DRender::QDepthTest::Less : Qt3DRender::QDepthTest::Always);
		if (alpha)
		{
			showPass->setPassBlend();
		}

		XRenderPass* pickPass = new XRenderPass("pickFace_pwd", this);
		pickPass->addFilterKeyMask("pick2nd", 0);
		pickPass->setPassCullFace(Qt3DRender::QCullFace::CullingMode::NoCulling);
		pickPass->setPassDepthTest(Qt3DRender::QDepthTest::Always);
		pickPass->setEnabled(pickable);
		XEffect* effect = new XEffect(this);
		effect->addRenderPass(showPass);
		effect->addRenderPass(pickPass);
		setEffect(effect);

		m_colorParameter = setParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
		m_changeColorParameter = setParameter("changecolor", QVector4D(0.0f, 0.0f, 0.0f, 0.0f));
		m_methodParameter = setParameter("mt", 0);
		m_lightEnableParameter = setParameter("lightEnable", 0);
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
