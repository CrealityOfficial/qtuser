#ifndef QTUSER_3D_TEX_FACES_1595583144387_H
#define QTUSER_3D_TEX_FACES_1595583144387_H
#include "faces.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API TexFaces : public XEntity
	{
		Q_OBJECT
	public:
		TexFaces(Qt3DCore::QNode* parent = nullptr);
		virtual ~TexFaces();
		void setColor(const QVector4D& color);
		void updateBox(const Box3D& box);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // QTUSER_3D_TEX_FACES_1595583144387_H