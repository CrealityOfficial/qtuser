#include "xentity.h"
#include "qtuser3d/refactor/xentity.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	XEntity::XEntity(Qt3DCore::QNode* parent)
		:Qt3DCore::QEntity(parent)
		, m_geometryRenderer(nullptr)
	{
		m_material = new Qt3DRender::QMaterial(this);
		m_transform = new Qt3DCore::QTransform(this);
		addComponent(m_transform);
		addComponent(m_material);

		m_geometryRenderer = new Qt3DRender::QGeometryRenderer(this);
		addComponent(m_geometryRenderer);
	}

	XEntity::~XEntity()
	{
	}

	Qt3DRender::QParameter* XEntity::setParameter(const QString& name, const QVariant& value)
	{
		Qt3DRender::QParameter* parameter = nullptr;
		QVector<Qt3DRender::QParameter*> parameters = m_material->parameters();

		for (Qt3DRender::QParameter* param : parameters)
		{
			if (name == param->name())
			{
				param->setValue(value);
				parameter = param;
				break;
			}
		}

		if (!parameter)
		{
			parameter = new Qt3DRender::QParameter(name, value, m_material);
			m_material->addParameter(parameter);
		}

		return parameter;
	}

	void XEntity::setEffect(XEffect* effect)
	{
		m_material->setEffect(effect);
	}

	void XEntity::setPose(const QMatrix4x4& matrix)
	{
		m_transform->setMatrix(matrix);
	}

	void XEntity::setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type)
	{
		m_geometryRenderer->setGeometry(geometry);
		m_geometryRenderer->setPrimitiveType(type);
	}
}