#ifndef QTUSER_3D_GEOMETRYCREATEHELPER_1594863285229_H
#define QTUSER_3D_GEOMETRYCREATEHELPER_1594863285229_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QAttribute>

namespace qtuser_3d
{
	class QTUSER_3D_API GeometryCreateHelper : public QObject
	{
		Q_OBJECT
	public:
		GeometryCreateHelper(QObject* parent = nullptr);
		virtual ~GeometryCreateHelper();

		static Qt3DRender::QGeometry* create(Qt3DCore::QNode* parent = nullptr, Qt3DRender::QAttribute* attribute0 = nullptr, Qt3DRender::QAttribute* attribute1 = nullptr, Qt3DRender::QAttribute* attribute2 = nullptr, 
			Qt3DRender::QAttribute* attribute3 = nullptr);

		static Qt3DRender::QGeometry* createEx(Qt3DCore::QNode* parent = nullptr, Qt3DRender::QAttribute* attribute0 = nullptr, Qt3DRender::QAttribute* attribute1 = nullptr, Qt3DRender::QAttribute* attribute2 = nullptr,
			Qt3DRender::QAttribute* attribute3 = nullptr, Qt3DRender::QAttribute* attribute4 = nullptr, Qt3DRender::QAttribute* attribute5 = nullptr);
	};
}
#endif // QTUSER_3D_GEOMETRYCREATEHELPER_1594863285229_H