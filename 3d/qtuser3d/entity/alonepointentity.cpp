#include "alonepointentity.h"

#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
namespace qtuser_3d
{
	AlonePointEntity::AlonePointEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
		, m_usedEffect(nullptr)
		, m_pointSizeState(nullptr)
		, m_pointSize(1.0f)
	{
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));

		setEffect(EFFECT("pure"));
		setGeometry(PRIMITIVESHAPE("point"), Qt3DRender::QGeometryRenderer::Points);
		selectEffect();
	}
	
	AlonePointEntity::~AlonePointEntity()
	{
	}

	void AlonePointEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}

	void AlonePointEntity::updateGlobal(QVector3D& point)
	{
		QMatrix4x4 m;

		m.translate(point);
		setPose(m);
	}

	void AlonePointEntity::updateLocal(QVector3D& point, const QMatrix4x4& parentMatrix)
	{
		QVector3D globalPoint = parentMatrix * point;
		updateGlobal(globalPoint);
	}

	void AlonePointEntity::setPointSize(float pointSize)
	{
		if (pointSize != m_pointSize)
		{
			m_pointSize = pointSize;

			selectEffect();
		}
	}

	void AlonePointEntity::selectEffect()
	{
		if (m_pointSize == 1.0f)
		{
			setEffect(EFFECT("pure"));
		}
		else
		{
			if (!m_pointSizeState || !m_usedEffect)
			{
				m_usedEffect = EFFECTCREATE("pure", m_material);
				QList<Qt3DRender::QRenderPass*> renderPasses = m_usedEffect->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
				m_pointSizeState = new Qt3DRender::QPointSize(m_usedEffect);
				m_pointSizeState->setSizeMode(Qt3DRender::QPointSize::Fixed);
				if (renderPasses.size() > 0) renderPasses.at(0)->addRenderState(m_pointSizeState);
			}

			m_pointSizeState->setValue(m_pointSize);
			setEffect(m_usedEffect);
		}
	}

	void AlonePointEntity::setShader(const QString& name)
	{
		setEffect(EFFECTCREATE(name, m_material));

		QList<Qt3DRender::QRenderPass*> renderPasses = m_material->findChildren<Qt3DRender::QRenderPass*>(name, Qt::FindChildrenRecursively);
		for (Qt3DRender::QRenderPass* pass : renderPasses)
		{
			if (m_pointSizeState)
			{
				pass->addRenderState(m_pointSizeState);
			}
		}
	}
}