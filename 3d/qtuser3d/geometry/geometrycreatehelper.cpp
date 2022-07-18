#include "geometrycreatehelper.h"
#include <Qt3DRender/QBuffer>

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

	Qt3DRender::QGeometry* GeometryCreateHelper::create(Qt3DCore::QNode* parent, AttributeShade* attribute1,
		AttributeShade* attribute2, AttributeShade* attribute3,
		AttributeShade* attribute4, AttributeShade* attribute5)
	{
		auto f = [](AttributeShade* attribute)-> Qt3DRender::QAttribute* {
			Qt3DRender::QAttribute* qAttribute = nullptr;
			if (attribute && attribute->count > 0 && !attribute->name.isEmpty())
			{
				Qt3DRender::QBuffer* buffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
				buffer->setData(attribute->bytes);
				qAttribute = new Qt3DRender::QAttribute(buffer, attribute->name, Qt3DRender::QAttribute::Float, attribute->stride, attribute->count);
			}
			return qAttribute;
		};
		Qt3DRender::QAttribute* qAttribute1 = f(attribute1);
		Qt3DRender::QAttribute* qAttribute2 = f(attribute2);
		Qt3DRender::QAttribute* qAttribute3 = f(attribute3);
		Qt3DRender::QAttribute* qAttribute4 = f(attribute4);
		Qt3DRender::QAttribute* qAttribute5 = f(attribute5);
		return createEx(parent, qAttribute1, qAttribute2, qAttribute3, qAttribute4, qAttribute5);
	}
}