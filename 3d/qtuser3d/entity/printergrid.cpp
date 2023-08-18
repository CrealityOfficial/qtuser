#include "printergrid.h"
#include <Qt3DRender/QLineWidth>
#include "qtuser3d/geometry/gridcreatehelper.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	PrinterGrid::PrinterGrid(Qt3DCore::QNode* parent, float lw)
		: XEntity(parent)
		, m_gap(10.0f)
	{
		setObjectName("PrinterGrid");

		XRenderPass* pass = new XRenderPass("pure", this);
		pass->addFilterKeyMask("view", 0);
		pass->setPassDepthTest(Qt3DRender::QDepthTest::Less);
		Qt3DRender::QLineWidth* lineWidth = new Qt3DRender::QLineWidth(this);
		lineWidth->setSmooth(true);
		lineWidth->setValue(lw);
		pass->addRenderState(lineWidth);

		XEffect* effect = new XEffect(this);
		effect->addRenderPass(pass);

		setEffect(effect);

		setLineColor(QVector4D(0.32f, 0.32f, 0.32f, 1.0f));

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

	void PrinterGrid::setGap(float gap)
	{
		m_gap = gap;
	}

	void PrinterGrid::setLineColor(QVector4D clr)
	{
		m_lineColor = clr;
		setParameter("color", m_lineColor);
	}
}
