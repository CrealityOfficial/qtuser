#ifndef QTUSER_3D_BOXCREATEHELPER_1594864230337_H
#define QTUSER_3D_BOXCREATEHELPER_1594864230337_H
#include "qtuser3d/geometry/geometrycreatehelper.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API BoxCreateHelper : public GeometryCreateHelper
	{
		Q_OBJECT
	public:
		BoxCreateHelper(QObject* parent = nullptr);
		virtual ~BoxCreateHelper();

		static Qt3DRender::QGeometry* create(Qt3DCore::QNode* parent = nullptr);
		static Qt3DRender::QGeometry* create(const Box3D& box, Qt3DCore::QNode* parent = nullptr);
	};
}
#endif // QTUSER_3D_BOXCREATEHELPER_1594864230337_H