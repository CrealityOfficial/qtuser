#ifndef QTUSER_3D_HOTBEDENTITY_1595559517621_H
#define QTUSER_3D_HOTBEDENTITY_1595559517621_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class Faces;
	class QTUSER_3D_API HotbedEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		HotbedEntity(Qt3DCore::QNode* parent = nullptr, int faceNum=0);
		virtual ~HotbedEntity();

		void drawFace(int faceNum);
		void setVisibility(int faceIndex, bool visibility);
		void setColor(const QVector4D& color);
		void setFaceNum(int faceNum);
		int faceNum();
		void clearData();
		void checkBed(QList<Box3D>& boxes);
	protected:
		int m_faceNum;
		std::vector<Faces*> m_bedFaces;
		std::vector<Box3D> m_hotZone;
		std::vector<bool> m_isHots;
	};
}
#endif // QTUSER_3D_HOTBEDENTITY_1595559517621_H