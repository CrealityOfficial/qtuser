#include "qtuser3d/entity/piefadeentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>

namespace qtuser_3d
{
	PieFadeEntity::PieFadeEntity(Qt3DCore::QNode* parent, int type)
		:PickableEntity(parent)
	{
		QString shader_type = "piefade.alpha_pickFace.pick";
		if (type == 1)
		{
			shader_type = "piefade.alpha";
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

		m_rotModeParameter = createParameter("rotMode", 0);
		m_rotRadiansParameter = createParameter("rotRadians", 0.0f);
		m_rotCenterParameter = createParameter("rotCenter", QVector3D(0.0, 0.0, 0.0));
		m_rotInitDirParameter = createParameter("rotInitDir", QVector3D(0.0, 1.0, 0.0));
		m_rotAxisParameter = createParameter("rotAxis", QVector3D(0.0, 0.0, 1.0));

		effect->setPassDepthTest("piefade.alpha", Qt3DRender::QDepthTest::Always);
		effect->setPassDepthTest("pickFace.pick", Qt3DRender::QDepthTest::Always);
	}

	PieFadeEntity::~PieFadeEntity()
	{

	}

	void PieFadeEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}

	void PieFadeEntity::setChangeColor(const QVector4D& color)
	{
		m_changeColorParameter->setValue(color);
	}

	void PieFadeEntity::setMethod(int mt)
	{
		m_methodParameter->setValue(mt);
	}

	void PieFadeEntity::setRotMode(int mode)
	{
		m_rotModeParameter->setValue(mode);
	}

	void PieFadeEntity::setRotRadians(float radians)
	{
		m_rotRadiansParameter->setValue(radians);
	}

	void PieFadeEntity::setRotCenter(QVector3D center)
	{
		m_rotCenterParameter->setValue(center);
	}

	void PieFadeEntity::setRotInitDir(QVector3D dir)
	{
		m_rotInitDirParameter->setValue(dir);
	}
	void PieFadeEntity::setRotAxis(QVector3D axis)
	{
		m_rotAxisParameter->setValue(axis);
	}
}
