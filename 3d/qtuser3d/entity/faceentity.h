#ifndef QTUSER_3D_FACEENTITY_1595559517621_H
#define QTUSER_3D_FACEENTITY_1595559517621_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	enum faceType
	{
		left, right, front, up, down, back
	};

	class Faces;
	class QTUSER_3D_API FaceEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		FaceEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~FaceEntity();

		void drawFace(Box3D& box);

		void setVisibility(int type, bool visibility);
		void setColor(int type, const QVector4D& color);
		void updateFace(Box3D& box,faceType type);
	protected:
		Faces* m_faceFront;
		Faces* m_faceBack;
		Faces* m_faceLeft;
		Faces* m_faceRight;
		Faces* m_faceTop;
		Faces* m_faceBottom;
	};
}
#endif // QTUSER_3D_FACEENTITY_1595559517621_H