#ifndef _QTUSER_CORE_CONVEXPOLYGON_1591761947220_H
#define _QTUSER_CORE_CONVEXPOLYGON_1591761947220_H
#include "qtuser3d/qtuser3dexport.h"
#include <QtGui/QVector3D>
#include <QtCore/QVector>

namespace qtuser_3d
{
	class QTUSER_3D_API ConvexPolygon
	{
	public:
		ConvexPolygon();
		virtual ~ConvexPolygon();

		QVector<QVector3D> m_polygons;    // z is zero

		bool check();  // debug function   
	};
}
#endif // _QTUSER_CORE_CONVEXPOLYGON_1591761947220_H
