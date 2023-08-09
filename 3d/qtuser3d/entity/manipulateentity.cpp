#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent, bool alpha, bool pickable, bool depthTest, bool overlay)
		:PickXEntity(parent)
	{
		XRenderPass* showPass = new XRenderPass("phong", this);
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
		m_changeColor = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
		m_colorParameter = setParameter("color", m_color);

		m_isChangeColorEnabled = true;

		m_lightEnableParameter = setParameter("lightEnable", 0);

		connect(this, &PickXEntity::signalStateChanged, this, &ManipulateEntity::slotStateChanged);

	}

	ManipulateEntity::~ManipulateEntity()
	{

	}

	void ManipulateEntity::setColor(const QVector4D& color)
	{
		m_color = color;
		resetColor();
	}

	void ManipulateEntity::setChangeColor(const QVector4D& color)
	{
		m_changeColor = color;
		changeColor();
	}

	void ManipulateEntity::setChangeColorEnabled(bool enable)
	{
		m_isChangeColorEnabled = enable;
		changeColor();
	}

	void ManipulateEntity::setLightEnable(bool flag)
	{
		m_lightEnableParameter->setValue(flag);
	}

	void ManipulateEntity::resetColor()
	{
		m_colorParameter->setValue(m_color);
	}

	void ManipulateEntity::changeColor()
	{
		if (m_isChangeColorEnabled)
			m_colorParameter->setValue(m_changeColor);
	}

	void ManipulateEntity::slotStateChanged(ControlState newState)
	{
		if ((bool)newState)
			resetColor();
		else
			changeColor();
	}
}
