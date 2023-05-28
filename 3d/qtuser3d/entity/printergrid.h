#ifndef _QTUSER_3D_PRINTERGRID_1590674912369_H
#define _QTUSER_3D_PRINTERGRID_1590674912369_H
#include "qtuser3d/refactor/xentity.h"
#include "qtuser3d/math/box3d.h"
#include <QVector4D>

namespace qtuser_3d
{
	class QTUSER_3D_API PrinterGrid: public XEntity
	{
	public:
		PrinterGrid(Qt3DCore::QNode* parent = nullptr, float lw = 1.0);
		virtual ~PrinterGrid();

		void updateBounding(Box3D& box, int createtype = 0);
		void visibleSubGrid(bool visible);
		void setShowColor(QVector4D clr, int xy = 0);
		void setLineColor(QVector4D clr);
		void setGap(float gap);
	protected:
		Qt3DRender::QParameter* m_visible;
		QVector4D m_lineColor;
		QVector4D m_xshowColor;
		QVector4D m_yshowColor;

		float m_gap;

		Qt3DRender::QParameter* m_lineColorParam;
		Qt3DRender::QParameter* m_xshowColorParam;
		Qt3DRender::QParameter* m_yshowColorParam;
	};
}
#endif // _QTUSER_3D_PRINTERGRID_1590674912369_H
