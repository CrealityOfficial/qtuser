#ifndef _QTUSER_3D_PRINTERGRID_1590674912369_H
#define _QTUSER_3D_PRINTERGRID_1590674912369_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"
#include <QVector4D>

namespace qtuser_3d
{

	class UEffect;

	class QTUSER_3D_API PrinterGrid: public BasicEntity
	{
	public:
		PrinterGrid(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterGrid();

		void updateBounding(Box3D& box, int createtype = 0);
		void visibleSubGrid(bool visible);
		void setShowColor(QVector4D clr);
		void setLineColor(QVector4D clr);
		void setGap(float gap);
	protected:
		Qt3DRender::QParameter* m_visible;
		QVector4D m_lineColor;
		QVector4D m_showColor;

		float m_gap;

		Qt3DRender::QParameter* m_lineColorParam;
		Qt3DRender::QParameter* m_showColorParam;
	};
}
#endif // _QTUSER_3D_PRINTERGRID_1590674912369_H
