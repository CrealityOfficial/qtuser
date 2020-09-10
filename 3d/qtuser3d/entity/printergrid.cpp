#include "printergrid.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/gridcreatehelper.h"

namespace qtuser_3d
{
	PrinterGrid::PrinterGrid(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		setEffect(EFFECTCREATE("printergrid", m_material));
		m_visible = createParameter("visible", 0.0f);
	}

	PrinterGrid::~PrinterGrid()
	{
	}

	void PrinterGrid::updateBounding(Box3D& box)
	{
		setGeometry(GridCreateHelper::create(box), Qt3DRender::QGeometryRenderer::Lines);
	}

	void PrinterGrid::visibleSubGrid(bool visible)
	{
		m_visible->setValue(visible ? 1.0f : 0.0f);
	}
}
