#ifndef _QTUSER_3D_BUFFERHELPER_1588133045652_H
#define _QTUSER_3D_BUFFERHELPER_1588133045652_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/math/box3d.h"
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>
#include <QtGui/QMatrix4x4>

namespace qtuser_3d
{
	enum class AttribueSlot
	{
		Position,
		Normal,
		Color,
		Texcoord
	};

	class QTUSER_3D_API BufferHelper
	{
	public:
		static Qt3DRender::QAttribute* CreateVertexAttribute(const char* buffer, AttribueSlot slot, uint count);
		static Qt3DRender::QAttribute* CreateVertexAttribute(const QString& name, const char* buffer, uint vertexSize, uint count);
		static Qt3DRender::QAttribute* CreateIndexAttribute(const char* buffer, uint count);

		static Qt3DRender::QAttribute* createAttribute(const QString& name, Qt3DRender::QAttribute::VertexBaseType vertexBaseType, uint vertexSize);
		static Qt3DRender::QAttribute* createDefaultVertexAttribute();
		static Qt3DRender::QAttribute* createDefaultNormalAttribute();
		static void setAttributeCount(Qt3DRender::QAttribute* attribute, int count);
		static void clearAttributeBuffer(Qt3DRender::QAttribute* attribute);

		static qtuser_3d::Box3D calculateVertexAttributeBox(Qt3DRender::QAttribute* attribute, int start, int end, const QMatrix4x4& matrix);
		static void updatePositionNormalAttributes(Qt3DRender::QAttribute* positionAttribute, QByteArray* positionBytes, Qt3DRender::QAttribute* normalAttribute, int start, int end);
	};
}
#endif // _QTUSER_3D_BUFFERHELPER_1588133045652_H
