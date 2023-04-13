#include "qtuser3d/entity/piefadeentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>

namespace qtuser_3d
{
	PieFadeEntity::PieFadeEntity(Qt3DCore::QNode* parent, bool alpha, bool depthTest, bool pickable)
		:PickableEntity(parent)
	{
		QString showPassName = "piefade";
		QString pickPassName = "";
		if (alpha)
		{
			showPassName += ".alpha";
		}

		if (pickable)
		{
			pickPassName = "pickFace.pick";
		}

		QString shader_type = showPassName + "_" + pickPassName;

		qtuser_3d::UEffect* effect = (qtuser_3d::UEffect*)EFFECTCREATE(shader_type, m_material);
		setEffect(effect);

		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
		
		m_rotModeParameter = createParameter("rotMode", 0);
		m_rotRadiansParameter = createParameter("rotRadians", 0.0f);
		m_rotCenterParameter = createParameter("rotCenter", QVector3D(0.0, 0.0, 0.0));
		m_rotInitDirParameter = createParameter("rotInitDir", QVector3D(0.0, 1.0, 0.0));
		m_rotAxisParameter = createParameter("rotAxis", QVector3D(0.0, 0.0, 1.0));

		m_lightEnableParameter = createParameter("lightEnable", 0);

		if (!depthTest)
		{
			effect->setPassDepthTest(showPassName, Qt3DRender::QDepthTest::Always);
			if (pickable)
				effect->setPassDepthTest(pickPassName, Qt3DRender::QDepthTest::Always);
		}
		else
		{
			effect->setPassDepthTest(showPassName, Qt3DRender::QDepthTest::Less);
			if (pickable)
				effect->setPassDepthTest(pickPassName, Qt3DRender::QDepthTest::Less);
		}

		setPassCullFace(showPassName, Qt3DRender::QCullFace::CullingMode::Back);
		if (pickable)
			setPassCullFace(pickPassName, Qt3DRender::QCullFace::CullingMode::Back);
	}

	PieFadeEntity::~PieFadeEntity()
	{

	}

	void PieFadeEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
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

	void PieFadeEntity::setLigthEnable(bool enable)
	{
		m_lightEnableParameter->setValue(enable);
	}
}
