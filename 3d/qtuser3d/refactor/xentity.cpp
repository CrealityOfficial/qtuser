#include "xentity.h"
#include "qtuser3d/refactor/xentity.h"
#include "qtuser3d/refactor/xeffect.h"

#include <QtCore/QDebug>

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

	void XEntity::setModelMatrix(const QMatrix4x4& matrix)
	{
		setParameter("modelMatrix", matrix);
	}

	void XEntity::setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type)
	{
		m_geometryRenderer->setGeometry(geometry);
		m_geometryRenderer->setPrimitiveType(type);
	}

	void XEntity::replaceGeometryRenderer(Qt3DRender::QGeometryRenderer* geometryRenderer)
	{
		removeComponent(m_geometryRenderer);
		m_geometryRenderer->setParent((Qt3DCore::QNode*)nullptr);

		delete m_geometryRenderer;
		m_geometryRenderer = geometryRenderer;

		if (m_geometryRenderer)
		{
			addComponent(m_geometryRenderer);
		}
	}

	void XEntity::addRenderState(int index, Qt3DRender::QRenderState* state)
	{
		XEffect* effect = qobject_cast<XEffect*>(m_material->effect());
		if (!effect)
		{
			qDebug() << QString("XEffect is null.");
			return;
		}

		effect->addRenderState(index, state);
	}

	void XEntity::addPassFilter(int index, const QString& filter)
	{
		XEffect* effect = qobject_cast<XEffect*>(m_material->effect());
		if (!effect)
		{
			qDebug() << QString("XEffect is null.");
			return;
		}

		effect->addPassFilter(index, filter);
	}

	Qt3DRender::QGeometry* XEntity::geometry()
	{
		return m_geometryRenderer->geometry();
	}

	QMatrix4x4 XEntity::pose()
	{
		return m_transform->matrix();
	}
}