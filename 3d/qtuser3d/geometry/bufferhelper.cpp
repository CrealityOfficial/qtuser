#include "qtuser3d/geometry/bufferhelper.h"

namespace qtuser_3d
{
	Qt3DRender::QAttribute* BufferHelper::CreateVertexAttribute(const char* buffer, AttribueSlot slot, uint count)
	{
		if (count == 0 || !buffer) return nullptr;

		uint vertexSize = 3;
		QString attributeName;
		switch (slot)
		{
		case AttribueSlot::Position:
			attributeName = Qt3DRender::QAttribute::defaultPositionAttributeName();
			break;
		case AttribueSlot::Normal:
			attributeName = Qt3DRender::QAttribute::defaultNormalAttributeName();
			break;
		case AttribueSlot::Color:
			attributeName = Qt3DRender::QAttribute::defaultColorAttributeName();
			vertexSize = 4;
			break;
		case AttribueSlot::Texcoord:
			attributeName = "vertexTexcoord"; // Qt3DRender::QAttribute::defaultTextureCoordinate1AttributeName();
			vertexSize = 2;
			break;
		default:
			break;
		}

		Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		qBuffer->setData(QByteArray(buffer, vertexSize * sizeof(float) * count));
		Qt3DRender::QAttribute* attribute = new Qt3DRender::QAttribute(qBuffer, attributeName, Qt3DRender::QAttribute::Float, vertexSize, count);
		return attribute;
	}

	Qt3DRender::QAttribute* BufferHelper::CreateVertexAttribute(const QString& name, const char* buffer, uint vertexSize, uint count)
	{
		Qt3DRender::QBuffer* qBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer);
		qBuffer->setData(QByteArray(buffer, vertexSize * sizeof(float) * count));
		Qt3DRender::QAttribute* attribute = new Qt3DRender::QAttribute(qBuffer, name, Qt3DRender::QAttribute::Float, vertexSize, count);
		return attribute;
	}

	Qt3DRender::QAttribute* BufferHelper::CreateIndexAttribute(const char* buffer, uint count)
	{
		Qt3DRender::QBuffer* indexBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::IndexBuffer);
		indexBuffer->setData(QByteArray(buffer, sizeof(unsigned) * count));

		Qt3DRender::QAttribute* indexAttribute = new Qt3DRender::QAttribute(indexBuffer, Qt3DRender::QAttribute::UnsignedInt, 1, count);
		indexAttribute->setAttributeType(Qt3DRender::QAttribute::IndexAttribute);
		return indexAttribute;
	}
}
