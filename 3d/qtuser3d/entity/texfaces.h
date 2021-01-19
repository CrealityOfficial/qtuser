#ifndef QTUSER_3D_TEX_FACES_1595583144387_H
#define QTUSER_3D_TEX_FACES_1595583144387_H
#include "faces.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API TexFaces : public BasicEntity
	{
		Q_OBJECT
	public:
		TexFaces(Qt3DCore::QNode* parent = nullptr);
		virtual ~TexFaces();
		void setColor(const QVector4D& color);
		void loadImage();
		void updateBox(const Box3D& box);
		void setImageVisible(bool visible);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
		Qt3DRender::QParameter* m_visibleParameter;
		Qt3DRender::QParameter* m_platformsizeParameter;
	};
}
#endif // QTUSER_3D_TEX_FACES_1595583144387_H