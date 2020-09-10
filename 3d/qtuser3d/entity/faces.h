#ifndef QTUSER_3D_FACES_1595583144387_H
#define QTUSER_3D_FACES_1595583144387_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API Faces : public BasicEntity
	{
		Q_OBJECT
	public:
		Faces(Qt3DCore::QNode* parent = nullptr);
		virtual ~Faces();
		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // QTUSER_3D_FACES_1595583144387_H