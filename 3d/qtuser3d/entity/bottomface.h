#ifndef QTUSER_3D_BOTTOM_FACES_1595583144387_H
#define QTUSER_3D_BOTTOM_FACES_1595583144387_H
#include "faces.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API BottomFace : public BasicEntity
	{
		Q_OBJECT
	public:
		BottomFace(Qt3DCore::QNode* parent = nullptr);
		virtual ~BottomFace();
		void setColor(const QVector4D& color);
		void updateBox(const Box3D& box);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // QTUSER_3D_BOTTOM_FACES_1595583144387_H