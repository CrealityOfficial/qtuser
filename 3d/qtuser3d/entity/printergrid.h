#ifndef _QTUSER_3D_PRINTERGRID_1590674912369_H
#define _QTUSER_3D_PRINTERGRID_1590674912369_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PrinterGrid: public BasicEntity
	{
	public:
		PrinterGrid(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterGrid();

		void updateBounding(Box3D& box);
		void visibleSubGrid(bool visible);
	protected:
		Qt3DRender::QParameter* m_visible;
	};
}
#endif // _QTUSER_3D_PRINTERGRID_1590674912369_H
