#include "lineentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/linecreatehelper.h"
namespace qtuser_3d
{
	LineEntity::LineEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
		, m_usePure(true)
	{
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));

		setEffect(EFFECT("pure"));
	}
	
	LineEntity::~LineEntity()
	{
	}

	void LineEntity::updateGeometry(QVector<QVector3D>& positions, QVector<QVector4D>& colors, bool loop)
	{
		int pointsNum = (int)positions.size();
		if (pointsNum <= 0)
			updateGeometry(pointsNum, nullptr, nullptr);
		else
		{
			updateGeometry(pointsNum, (float*)&positions.at(0), (colors.size() > 0 ? (float*)&colors.at(0) : nullptr));
		}
	}

	void LineEntity::updateGeometry(QVector<QVector3D>& positions, bool loop)
	{
		updateGeometry(positions.size(), positions.size() > 0 ? (float*)&positions.at(0) : nullptr, nullptr);
	}

	void LineEntity::updateGeometry(int pointsNum, float* positions, float* colors, bool loop)
	{
		if (m_usePure && colors)
		{
			setEffect(EFFECT("color"));
			m_usePure = false;
		}
		else if (!m_usePure && !colors)
		{
			setEffect(EFFECT("pure"));
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
			setEffect(EFFECT("color"));
			m_usePure = false;
		}
		else if (!m_usePure && !color)
		{
			setEffect(EFFECT("pure"));
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