#include "pointcloudentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/pointscreatehelper.h"
#include <Qt3DRender/QRenderPass>

namespace qtuser_3d
{
	PointCloudEntity::PointCloudEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
		, m_usedEffect(nullptr)
		, m_pointSizeState(nullptr)
		, m_pointSize(1.0f)
	{
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		selectEffect();
	}

	PointCloudEntity::~PointCloudEntity()
	{
	}

	void PointCloudEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}

	void PointCloudEntity::setPointSize(float pointSize)
	{
		if (pointSize != m_pointSize)
		{
			m_pointSize = pointSize;

			selectEffect();
		}
	}

	void PointCloudEntity::selectEffect()
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

	void PointCloudEntity::updateGeometry(QVector<QVector3D>& positions)
	{
		(positions.size() == 0) ? updateGeometry(nullptr) : updateGeometry((int)positions.size(), (float*)&positions.at(0));
	}

	void PointCloudEntity::updateGeometry(int pointsNum, float* positions)
	{
		updateGeometry(PointsCreateHelper::create(positions, pointsNum));
	}

	void PointCloudEntity::updateGeometry(Qt3DRender::QGeometry* geometry)
	{
		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Points;
		setGeometry(geometry, type);
	}
}
