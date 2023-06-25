#ifndef QTUSER_3D_PRIMITIVESHAPECACHE_1594861761536_H
#define QTUSER_3D_PRIMITIVESHAPECACHE_1594861761536_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtusercore/module/singleton.h"
#include <Qt3DRender/QGeometry>
#include <QtCore/QMap>

namespace qtuser_3d
{
	class QTUSER_3D_API PrimitiveShapeCache : public QObject
	{
		Q_OBJECT
		SINGLETON_DECLARE(PrimitiveShapeCache)
	public:
		virtual ~PrimitiveShapeCache();

		void uninitialize();

		Qt3DCore::QNode* root();

		Qt3DRender::QGeometry* create(const QString& name);
		Qt3DRender::QGeometry* shape(const QString& name);

	protected:
		Qt3DCore::QNode* m_root;
		QMap<QString, Qt3DRender::QGeometry*> m_primitiveShapes;
	};
	
	SINGLETON_EXPORT(QTUSER_3D_API, PrimitiveShapeCache)
}

#define PRIMITIVE_CACHE_UNINITIALIZE qtuser_3d::getPrimitiveShapeCache()->uninitialize()
#define PRIMITIVEROOT qtuser_3d::getPrimitiveShapeCache()->root()
#define PRIMITIVESHAPE(x) qtuser_3d::getPrimitiveShapeCache()->create(x)

namespace qtuser_3d
{
	QTUSER_3D_API Qt3DRender::QGeometry* createLinesPrimitive(const QString& name);
	QTUSER_3D_API Qt3DRender::QGeometry* createPointsPrimitive(const QString& name);
	QTUSER_3D_API Qt3DRender::QGeometry* createTrianglesPrimitive(const QString& name);
	QTUSER_3D_API Qt3DRender::QGeometry* createTrianglesPrimitiveDLP(const QString& name);
}
#endif // QTUSER_3D_PRIMITIVESHAPECACHE_1594861761536_H