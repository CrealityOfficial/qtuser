#include "printergrid.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/gridcreatehelper.h"
#include "qtuser3d/effect/ueffect.h"
#include <Qt3DRender/QLineWidth>

namespace qtuser_3d
{
	PrinterGrid::PrinterGrid(Qt3DCore::QNode* parent, float lw)
		: BasicEntity(parent)
		, m_lineColor(0.32f, 0.32f, 0.32f, 1.0f)
		, m_xshowColor(0.65f, 0.23f, 0.23f, 1.0f)
		, m_yshowColor(0.65f, 0.23f, 0.23f, 1.0f)
		, m_gap(10.0f)
	{
		UEffect* useEffect = qobject_cast<UEffect*>( EFFECTCREATE("printergrid", m_material));
		m_xshowColorParam = useEffect->createParameter("xshowcolor", m_xshowColor);
		m_yshowColorParam = useEffect->createParameter("yshowcolor", m_yshowColor);
		m_lineColorParam = useEffect->createParameter("linecolor", m_lineColor);
		m_visible = createParameter("visible", 0.0f);

		setObjectName("PrinterGrid");

		Qt3DRender::QLineWidth* lineWidth = new Qt3DRender::QLineWidth(this);
		lineWidth->setSmooth(true);
		lineWidth->setValue(lw);
		QList<Qt3DRender::QRenderPass*> passes = useEffect->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
		for (Qt3DRender::QRenderPass* pass : passes)
		{
			pass->addRenderState(lineWidth);
		}
		setEffect(useEffect);
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

	void PrinterGrid::setShowColor(QVector4D clr, int xy)
	{
		if (xy == 0)
		{
			m_xshowColor = clr;
			m_xshowColorParam->setValue(clr);
		}
		else if (xy == 1)
		{
			m_yshowColor = clr;
			m_yshowColorParam->setValue(clr);
		}
	}

	void PrinterGrid::setLineColor(QVector4D clr)
	{
		m_lineColor = clr;
		m_lineColorParam->setValue(clr);
	}
}
