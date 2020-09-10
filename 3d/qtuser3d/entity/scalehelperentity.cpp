#include "qtuser3d/entity/scalehelperentity.h"
#include "qtuser3d/entity/manipulateentity.h"

#include "qtuser3d/utils/primitiveshapecache.h"

namespace qtuser_3d
{
	ScaleHelperEntity::ScaleHelperEntity(Qt3DCore::QNode* parent)
		:Qt3DCore::QEntity(parent)
	{
		m_transform = new Qt3DCore::QTransform(this);
		addComponent(m_transform);

		m_xArrowEntity = new qtuser_3d::ManipulateEntity(this);
		m_yArrowEntity = new qtuser_3d::ManipulateEntity(this);
		m_zArrowEntity = new qtuser_3d::ManipulateEntity(this);
		//m_xArrowEntity->setNoPrimitive(true);
		//m_yArrowEntity->setNoPrimitive(true);
		//m_zArrowEntity->setNoPrimitive(true);

		QMatrix4x4 xMatrix;
		xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
		xMatrix.scale(100.0f, 200.0f, 100.0f);
		QMatrix4x4 yMatrix;
		yMatrix.scale(100.0f, 200.0f, 100.0f);
		QMatrix4x4 zMatrix;
		zMatrix.rotate(90.0f, 1.0f, 0.0f, 0.0f);
		zMatrix.scale(100.0f, 200.0f, 100.0f);
		m_xArrowEntity->setPose(xMatrix);
		m_yArrowEntity->setPose(yMatrix);
		m_zArrowEntity->setPose(zMatrix);

		m_xArrowEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		m_yArrowEntity->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
		m_zArrowEntity->setColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));

		Qt3DRender::QGeometry* geometry = PRIMITIVESHAPE("arrow");
		m_xArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
		m_yArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
		m_zArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
	}

	ScaleHelperEntity::~ScaleHelperEntity()
	{

	}

	qtuser_3d::PickableEntity* ScaleHelperEntity::xEntity()
	{
		return m_xArrowEntity;
	}

	qtuser_3d::PickableEntity* ScaleHelperEntity::yEntity()
	{
		return m_yArrowEntity;
	}

	qtuser_3d::PickableEntity* ScaleHelperEntity::zEntity()
	{
		return m_zArrowEntity;
	}
}