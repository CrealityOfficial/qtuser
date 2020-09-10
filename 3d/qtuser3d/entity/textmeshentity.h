#ifndef _QTUSER_3D_TEXTMESHENTITY_1590738216492_H
#define _QTUSER_3D_TEXTMESHENTITY_1590738216492_H
#include "qtuser3d/entity/basicentity.h"
#include <Qt3DExtras/QExtrudedTextMesh>

namespace qtuser_3d
{
	class QTUSER_3D_API TextMeshEntity: public BasicEntity
	{
	public:
		TextMeshEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~TextMeshEntity();

		void setFont(const QString& family);
		void setText(const QString& text);
		void setColor(const QVector4D& color);
	protected:
		Qt3DExtras::QExtrudedTextMesh* m_textMesh;
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // _QTUSER_3D_TEXTMESHENTITY_1590738216492_H
