#include "printergrid.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/gridcreatehelper.h"
#include "qtuser3d/effect/ueffect.h"
#include <Qt3DRender/QLineWidth>

namespace qtuser_3d
{
	PrinterGrid::PrinterGrid(Qt3DCore::QNode* parent, float lw)
		: BasicEntity(parent)
		, m_lineColor(0.32f, 0.32f, 0.32f, 1.0)
		, m_xshowColor(0.65, 0.23, 0.23, 1.0)
		, m_yshowColor(0.65, 0.23, 0.23, 1.0)
		, m_gap(10.0)
	{
		UEffect* useEffect = qobject_cast<UEffect*>( EFFECTCREATE("printergrid", m_material));
		m_xshowColorParam = useEffect->createParameter("xshowcolor", m_xshowColor);
		m_yshowColorParam = useEffect->createParameter("yshowcolor", m_yshowColor);
		m_lineColorParam = useEffect->createParameter("linecolor", m_lineColor);
		m_visible = createParameter("visible", 0.0f);
		m_xIndexParam = createParameter("highlight_index_x", 30.0f);
		m_yIndexParam = createParameter("highlight_index_y", 30.0f);
		m_yIndexColorParam = createParameter("xyIndexcolor", QVector4D(0.3412f,0.4118f,0.4706f,1.0f));
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

	void PrinterGrid::updateXYIndex(Box3D& box)
	{
		QVector3D size = box.size();
		if (size.x() == 0 || size.y() == 0) return ;

		float minX = box.min.x();
		float maxX = box.max.x();
		float minY = box.min.y();
		float maxY = box.max.y();

		float midX = (minX + maxX) / 2.0;
		float midY = (minY + maxY) / 2.0;

		float x_index =int( int(maxX - midX) / m_gap) * m_gap;
		float y_index = int( int(maxY - midY) / m_gap) * m_gap;
		m_xIndexParam->setValue(x_index);
		m_yIndexParam->setValue(y_index);
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
	void PrinterGrid::setXYIndexColor(QVector4D clr)
	{
		m_yIndexColorParam->setValue(clr);
	}
}
