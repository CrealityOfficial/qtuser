#include "lineentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/linecreatehelper.h"
#include <Qt3DRender/QRenderPass>

namespace qtuser_3d
{
	LineEntity::LineEntity(Qt3DCore::QNode* parent)
		:XEntity(parent)
		, m_usePure(true)
	{
		m_colorParameter = setParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));

		// EFFECTCREATE 这种写法如果传入非空 parent 的话，会导致模型透明效果异常，原因有待研究
		//setEffect(EFFECTCREATE("pure", m_material));
		 
		setEffect(XEFFECTCREATE("pure", m_material));

		QList<Qt3DRender::QRenderPass*> renderPasses = m_material->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
		m_lineWidth = new Qt3DRender::QLineWidth(m_material);
		//m_lineWidth->setSmooth(true);
		if (renderPasses.size() > 0)
			renderPasses.at(0)->addRenderState(m_lineWidth);
	}
	
	LineEntity::~LineEntity()
	{
	}

	void LineEntity::setLineWidth(float width)
	{
		m_lineWidth->setValue(width);
	}

	void LineEntity::updateGeometry(const QVector<QVector3D>& positions, const QVector<QVector4D>& colors, bool loop)
	{
		int pointsNum = (int)positions.size();
		if (pointsNum <= 0)
			updateGeometry(pointsNum, nullptr, nullptr);
		else
		{
			updateGeometry(pointsNum, (float*)&positions.at(0), (colors.size() > 0 ? (float*)&colors.at(0) : nullptr));
		}
	}

	void LineEntity::updateGeometry(const QVector<QVector3D>& positions, bool loop)
	{
		updateGeometry(positions.size(), positions.size() > 0 ? (float*)&positions.at(0) : nullptr, nullptr);
	}

	void LineEntity::updateGeometry(int pointsNum, float* positions, float* colors, bool loop)
	{
		if (m_usePure && colors)
		{
			setEffect(XEFFECT("color"));
			m_usePure = false;
		}
		else if (!m_usePure && !colors)
		{
			setEffect(XEFFECT("pure"));
			m_usePure = true;
		}

		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Lines;
		if (loop) type = Qt3DRender::QGeometryRenderer::LineLoop;
		setGeometry(LineCreateHelper::create(pointsNum, positions, colors), type);
	}

	void LineEntity::updateGeometry(Qt3DRender::QGeometry* geometry, bool color, bool loop)
	{
		if (m_usePure && color)
		{
			setEffect(XEFFECT("color"));
			m_usePure = false;
		}
		else if (!m_usePure && !color)
		{
			setEffect(XEFFECT("pure"));
			m_usePure = true;
		}

		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Lines;
		if (loop) type = Qt3DRender::QGeometryRenderer::LineLoop;
		setGeometry(geometry, type);
	}

	void LineEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}