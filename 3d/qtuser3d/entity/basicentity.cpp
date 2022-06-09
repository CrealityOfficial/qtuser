#include "basicentity.h"
#include <QThread>
#include <Qt3DRender/QAttribute>
#include <Qt3DRender/QBuffer>

namespace qtuser_3d
{
	BasicEntity::BasicEntity(Qt3DCore::QNode* parent)
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

	BasicEntity::~BasicEntity()
	{
	}

	Qt3DRender::QParameter* BasicEntity::createParameter(const QString& name, const QVariant& value)
	{
		Qt3DRender::QParameter* parameter = new Qt3DRender::QParameter(name, value, m_material);
		m_material->addParameter(parameter);
		return parameter;
	}

	void BasicEntity::setParameter(const QString& name, const QVariant& value)
	{
		QVector<Qt3DRender::QParameter*> parameters = m_material->parameters();

		bool have = false;
		for (Qt3DRender::QParameter* parameter : parameters)
		{
			if (name == parameter->name())
			{
				parameter->setValue(value);
				have = true;
				break;
			}
		}

		if (!have)
			createParameter(name, value);
	}

	void BasicEntity::destroyParameter(Qt3DRender::QParameter* parameter)
	{
		if (parameter)
		{
			m_material->removeParameter(parameter);
			delete parameter;
		}
	}

	void BasicEntity::setEffect(Qt3DRender::QEffect* effect)
	{
		Qt3DRender::QEffect* oldEffect = m_material->effect();
		m_material->setEffect(effect);
		if (oldEffect && oldEffect->parent() == this)
			delete oldEffect;
	}

	Qt3DRender::QEffect* BasicEntity::selectEffect(Qt3DRender::QEffect* effect)
	{
		Qt3DRender::QEffect* oldEffect = m_material->effect();
		m_material->setEffect(effect);
		return oldEffect;
	}

	void BasicEntity::setPose(const QMatrix4x4& matrix)
	{
		m_transform->setMatrix(matrix);
		//setParameter("modelMatrix", m_matrix);
	}

	QMatrix4x4 BasicEntity::pose() const
	{
		return m_transform->matrix();
	}

	void BasicEntity::setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type, int vCountPerPatch)
	{
		Qt3DRender::QGeometry* oldGeometry = m_geometryRenderer->geometry();
		if (oldGeometry && (oldGeometry->parent() == m_geometryRenderer))
		{
			oldGeometry->setParent((Qt3DCore::QNode*)nullptr);
			delete oldGeometry;
		}

		m_geometryRenderer->setGeometry(geometry);

		if (type == Qt3DRender::QGeometryRenderer::PrimitiveType::Patches)
			m_geometryRenderer->setVerticesPerPatch(vCountPerPatch);

		if(geometry)
			m_geometryRenderer->setPrimitiveType(type);
	}

	Qt3DRender::QGeometry* BasicEntity::geometry()
	{
		return m_geometryRenderer->geometry();
	}

	void BasicEntity::replaceGeometryRenderer(Qt3DRender::QGeometryRenderer* geometryRenderer)
	{
		qDebug() << "replaceGeometryRenderer from " << m_geometryRenderer << " to " << geometryRenderer;
		removeComponent(m_geometryRenderer);
		m_geometryRenderer->setParent((Qt3DCore::QNode*)nullptr);

		delete m_geometryRenderer;
		m_geometryRenderer = geometryRenderer;

		if (m_geometryRenderer)
		{
			addComponent(m_geometryRenderer);
		}
	}

	Qt3DRender::QBlendEquationArguments* BasicEntity::blendArguments()
	{
		QList<Qt3DRender::QBlendEquationArguments*> argumnets
			= findChildren<Qt3DRender::QBlendEquationArguments*>(QString(), Qt::FindChildrenRecursively);
		if (argumnets.size() > 0)
			return argumnets.at(0);

		return nullptr;
	}

	Qt3DRender::QAttribute* BasicEntity::positionAttribute()
	{
		Qt3DRender::QGeometry* geom = geometry();
		if (!geom)
			return nullptr;

		QVector<Qt3DRender::QAttribute*> attributes = geom->attributes();
		if (attributes.size() == 0 || attributes.at(0)->name() != Qt3DRender::QAttribute::defaultPositionAttributeName())
			return nullptr;

		Qt3DRender::QAttribute* attribute = attributes.at(0);
		if (attribute->vertexSize() != 3)
			return nullptr;

		return attribute;
	}
	int BasicEntity::traitPositionCount()
	{
		Qt3DRender::QAttribute* attribute = BasicEntity::positionAttribute();
		if (!attribute)
			return 0;

		return (int)attribute->count();
	}

	void BasicEntity::fillPositionData(char* buffer, int len)
	{
		Qt3DRender::QAttribute* attribute = BasicEntity::positionAttribute();
		if (!attribute)
			return;

		int l = attribute->count() * attribute->vertexSize() * sizeof(float);
		if (len < l)
			return;

		memcpy(buffer, attribute->buffer()->data(), l);
	}
}