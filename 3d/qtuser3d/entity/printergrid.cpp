#include "printergrid.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/gridcreatehelper.h"
#include "qtuser3d/effect/ueffect.h"

namespace qtuser_3d
{
	PrinterGrid::PrinterGrid(Qt3DCore::QNode* parent)
		: BasicEntity(parent)
		, m_lineColor(0.8, 0.8, 0.8, 1.0)
		, m_showColor(1.0, 0.1, 0.1, 1.0)
		, m_gap(10.0)
	{
		UEffect* useEffect = qobject_cast<UEffect*>( EFFECTCREATE("printergrid", m_material));
		setEffect(useEffect);
		m_showColorParam = useEffect->createParameter("showcolor", m_showColor);
		m_lineColorParam = useEffect->createParameter("linecolor", m_lineColor);
		m_visible = createParameter("visible", 0.0f);
	}

	PrinterGrid::~PrinterGrid()
	{
	}

	void PrinterGrid::updateBounding(Box3D& box, int createtype)
	{
		if (createtype == 1)
		{
			setGeometry(GridCreateHelper::createMid(box, m_gap), Qt3DRender::QGeometryRenderer::Lines);
		}
		else
		{
			setGeometry(GridCreateHelper::create(box, m_gap), Qt3DRender::QGeometryRenderer::Lines);
		}
	}

	void PrinterGrid::visibleSubGrid(bool visible)
	{
		m_visible->setValue(visible ? 1.0f : 0.0f);
	}

	void PrinterGrid::setGap(float gap)
	{
		m_gap = gap;
	}

	void PrinterGrid::setShowColor(QVector4D clr)
	{
		m_showColor = clr;
		m_showColorParam->setValue(clr);
	}

	void PrinterGrid::setLineColor(QVector4D clr)
	{
		m_lineColor = clr;
		m_lineColorParam->setValue(clr);
	}
}
