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

		Qt3DRender::QGeometry* boxGeometry = BoxCreateHelper::create();  //盒子
		PRIMITIVE_ADD("box", boxGeometry)
		Qt3DRender::QGeometry* box_no_bottom = BoxCreateHelper::createNoBottom();  //没底座的盒子
		PRIMITIVE_ADD("box_nobottom", box_no_bottom)
		Qt3DRender::QGeometry* pointGeometry = PointsCreateHelper::create();
		PRIMITIVE_ADD("point", pointGeometry)
			Qt3DRender::QGeometry* arrowGeometry = BasicShapeCreateHelper::createInstructions(0.02, 0.8, 0.08, 0.3);//ArrowCreateHelper::create();
		PRIMITIVE_ADD("arrow", arrowGeometry)	// 箭头
		Qt3DRender::QGeometry* cylinderGeometry = BasicShapeCreateHelper::createCylinder();	
		PRIMITIVE_ADD("cylinder", cylinderGeometry)	//圆柱
		Qt3DRender::QGeometry* penGeometry = BasicShapeCreateHelper::createPen();
		PRIMITIVE_ADD("pen", penGeometry)
		Qt3DRender::QGeometry* scaleIndicatorGeometry = BasicShapeCreateHelper::createScaleIndicator(0.02, 0.8, 18, 0.15);
		PRIMITIVE_ADD("scaleindicator", scaleIndicatorGeometry)
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

	Qt3DRender::QGeometry* createLinesPrimitive(const QString& name)
	{
		if (name == "box")
			return BoxCreateHelper::create();
		else if(name == "box_nobottom")
			return BoxCreateHelper::createNoBottom();
		else
			return nullptr;
	}

	Qt3DRender::QGeometry* createPointsPrimitive(const QString& name)
	{
		return nullptr;
	}

	Qt3DRender::QGeometry* createTrianglesPrimitive(const QString& name)
	{
		if (name == "cylinder")
			return BasicShapeCreateHelper::createCylinder();
		else if ("arrow")
			return BasicShapeCreateHelper::createInstructions(0.02f, 0.8f, 0.08f, 0.3f);
		else if ("scaleindicator")
			return BasicShapeCreateHelper::createScaleIndicator(0.02f, 0.8f, 18.0f, 0.15f);
		else
			return nullptr;
	}
}