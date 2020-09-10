#include "primitiveshapecache.h"
#include "qtuser3d/geometry/boxcreatehelper.h"
#include "qtuser3d/geometry/pointscreatehelper.h"
#include "qtuser3d/geometry/arrowcreatehelper.h"
#include "qtuser3d/geometry/basicshapecreatehelper.h"

namespace qtuser_3d
{
	SINGLETON_IMPL(PrimitiveShapeCache)
	PrimitiveShapeCache::PrimitiveShapeCache()
		:QObject()
		, m_root(nullptr)
	{
		m_root = new Qt3DCore::QNode();

#define PRIMITIVE_ADD(x, y)  m_primitiveShapes.insert(x, y); \
							y->setParent(m_root);

		Qt3DRender::QGeometry* boxGeometry = BoxCreateHelper::create();
		PRIMITIVE_ADD("box", boxGeometry)
		Qt3DRender::QGeometry* pointGeometry = PointsCreateHelper::create();
		PRIMITIVE_ADD("point", pointGeometry)
		Qt3DRender::QGeometry* arrowGeometry = ArrowCreateHelper::create();
		PRIMITIVE_ADD("arrow", arrowGeometry)
		Qt3DRender::QGeometry* cylinderGeometry = BasicShapeCreateHelper::createCylinder();
		PRIMITIVE_ADD("cylinder", cylinderGeometry)
		Qt3DRender::QGeometry* penGeometry = BasicShapeCreateHelper::createPen();
		PRIMITIVE_ADD("pen", penGeometry)
	}
	
	PrimitiveShapeCache::~PrimitiveShapeCache()
	{
		delete m_root;
	}

	Qt3DCore::QNode* PrimitiveShapeCache::root()
	{
		return m_root;
	}

	Qt3DRender::QGeometry* PrimitiveShapeCache::shape(const QString& name)
	{
		QMap<QString, Qt3DRender::QGeometry*>::iterator it = m_primitiveShapes.find(name);
		if (it != m_primitiveShapes.end())
		{
			return it.value();
		}
		return nullptr;
	}
}