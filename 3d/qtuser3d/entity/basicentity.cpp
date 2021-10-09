#include "basicentity.h"
#include <QThread>

namespace qtuser_3d
{
	BasicEntity::BasicEntity(Qt3DCore::QNode* parent)
		:Qt3DCore::QEntity(parent)
		, m_geometryRenderer(nullptr)
	{
		if (parent != nullptr)
		{
			int test = 1;
		}

		m_material = new Qt3DRender::QMaterial(this);
		addComponent(m_material);

		m_transform = new Qt3DCore::QTransform(this);
		addComponent(m_transform);

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
	}

	QMatrix4x4 BasicEntity::pose() const
	{
		return m_transform->matrix();
	}

	void BasicEntity::setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type)
	{
		qDebug() << objectName() << " setGeometry";
		QNode* pNode = (QNode*)parent();
		setParent((QNode*)nullptr);
		QThread::usleep(20);

		Qt3DRender::QGeometry* oldGeometry = m_geometryRenderer->geometry();
		if (oldGeometry && (oldGeometry->parent() == m_geometryRenderer))
		{
			oldGeometry->setParent((Qt3DCore::QNode*)nullptr);
			delete oldGeometry;
		}

		m_geometryRenderer->setGeometry(geometry);

		if(geometry)
			m_geometryRenderer->setPrimitiveType(type);
		setParent(pNode);

		qDebug() << objectName() << " setGeometry over";
	}

	Qt3DRender::QGeometry* BasicEntity::geometry()
	{
		return m_geometryRenderer->geometry();
	}

	void BasicEntity::replaceGeometryRenderer(Qt3DRender::QGeometryRenderer* geometryRenderer)
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

	Qt3DRender::QBlendEquationArguments* BasicEntity::blendArguments()
	{
		QList<Qt3DRender::QBlendEquationArguments*> argumnets
			= findChildren<Qt3DRender::QBlendEquationArguments*>(QString(), Qt::FindChildrenRecursively);
		if (argumnets.size() > 0)
			return argumnets.at(0);

		return nullptr;
	}
}