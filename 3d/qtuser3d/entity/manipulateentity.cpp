#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent, bool lightEnable, bool alpha, bool pickable, bool depthTest, bool overlay)
		:PickXEntity(parent)
	{
		XRenderPass* showPass;
		if (lightEnable)
			showPass = new XRenderPass("phong", this);
		else
			showPass = new XRenderPass("pure", this);
		
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

		m_color = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);
		m_triggeredColor = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
		m_isTriggerible = false;

		setParameter("color", m_color);
	}

	ManipulateEntity::~ManipulateEntity()
	{

	}

	void ManipulateEntity::setColor(const QVector4D& color)
	{
		m_color = color;
		setParameter("color", m_color);
	}

	void ManipulateEntity::setTriggeredColor(const QVector4D& color)
	{
		m_triggeredColor = color;
	}

	void ManipulateEntity::setTriggerible(bool enable)
	{
		m_isTriggerible = enable;
	}

	void ManipulateEntity::setVisualState(ControlState state)
	{
		if (m_isTriggerible)
		{
			if ((int)state)
				setParameter("color", m_color);
			else
				setParameter("color", m_triggeredColor);
		}
	}
}
