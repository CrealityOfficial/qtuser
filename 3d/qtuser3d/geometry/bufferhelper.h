#ifndef _QTUSER_3D_BUFFERHELPER_1588133045652_H
#define _QTUSER_3D_BUFFERHELPER_1588133045652_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

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
	};
}
#endif // _QTUSER_3D_BUFFERHELPER_1588133045652_H
