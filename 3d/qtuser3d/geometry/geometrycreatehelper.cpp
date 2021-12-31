#include "geometrycreatehelper.h"

namespace qtuser_3d
{
	GeometryCreateHelper::GeometryCreateHelper(QObject* parent)
		:QObject(parent)
	{
	}
	
	GeometryCreateHelper::~GeometryCreateHelper()
	{
	}

	Qt3DRender::QGeometry* GeometryCreateHelper::create(Qt3DCore::QNode* parent, Qt3DRender::QAttribute* attribute0, Qt3DRender::QAttribute* attribute1, Qt3DRender::QAttribute* attribute2,
		Qt3DRender::QAttribute* attribute3)
	{
		Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(parent);

		if (attribute0) geometry->addAttribute(attribute0);
		if (attribute1) geometry->addAttribute(attribute1);
		if (attribute2) geometry->addAttribute(attribute2);
		if (attribute3) geometry->addAttribute(attribute3);

		return geometry;
	}


	Qt3DRender::QGeometry* GeometryCreateHelper::createEx(Qt3DCore::QNode* parent, Qt3DRender::QAttribute* attribute0, Qt3DRender::QAttribute* attribute1, Qt3DRender::QAttribute* attribute2,
		Qt3DRender::QAttribute* attribute3, Qt3DRender::QAttribute* attribute4, Qt3DRender::QAttribute* attribute5)
	{
		Qt3DRender::QGeometry* geometry = new Qt3DRender::QGeometry(parent);

		if (attribute0) geometry->addAttribute(attribute0);
		if (attribute1) geometry->addAttribute(attribute1);
		if (attribute2) geometry->addAttribute(attribute2);
		if (attribute3) geometry->addAttribute(attribute3);
		if (attribute4) geometry->addAttribute(attribute4);
		if (attribute5) geometry->addAttribute(attribute5);

		return geometry;
	}
}