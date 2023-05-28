#include "printergrid.h"
#include <Qt3DRender/QLineWidth>
#include "qtuser3d/geometry/gridcreatehelper.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	PrinterGrid::PrinterGrid(Qt3DCore::QNode* parent, float lw)
		: XEntity(parent)
		, m_lineColor(0.32f, 0.32f, 0.32f, 1.0f)
		, m_xshowColor(0.65f, 0.23f, 0.23f, 1.0f)
		, m_yshowColor(0.65f, 0.23f, 0.23f, 1.0f)
		, m_gap(10.0f)
	{
		setObjectName("PrinterGrid");

		m_xshowColorParam = setParameter("xshowcolor", m_xshowColor);
		m_yshowColorParam = setParameter("yshowcolor", m_yshowColor);
		m_lineColorParam = setParameter("linecolor", m_lineColor);
		m_visible = setParameter("visible", 0.0f);

		XRenderPass* pass = new XRenderPass("printergrid", this);
		pass->addFilterKeyMask("view", 0);
		pass->setPassDepthTest(Qt3DRender::QDepthTest::Less);
		Qt3DRender::QLineWidth* lineWidth = new Qt3DRender::QLineWidth(this);
		lineWidth->setSmooth(true);
		lineWidth->setValue(lw);
		pass->addRenderState(lineWidth);

		XEffect* effect = new XEffect(this);
		effect->addRenderPass(pass);

		setEffect(effect);
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
