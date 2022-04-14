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
		void setLogColor(const QVector4D& color);
		void loadImage(int type = 0);
		void updateBox(const Box3D& box);
		void setImageVisible(bool visible);
		int isImageColor();
	protected:
		Qt3DRender::QParameter* m_colorParameter;
		Qt3DRender::QParameter* m_logcolorParameter;
		Qt3DRender::QParameter* m_visibleParameter;
		Qt3DRender::QParameter* m_platformsizeParameter;
		Qt3DRender::QParameter* m_colorVisibleParameter;
		Qt3DRender::QParameter* m_imagebili;
		Qt3DRender::QParameter* m_imageshape;
		Qt3DRender::QParameter* m_shapeTexture;
		int m_imageColor;
	};
}
#endif // QTUSER_3D_TEX_FACES_1595583144387_H