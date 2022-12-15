#ifndef QTUSER_3D_HOTBEDENTITY_1595559517621_H
#define QTUSER_3D_HOTBEDENTITY_1595559517621_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	enum class bedType
	{
		CR_10_Inspire_Pro,
		CR_GX,
		CR_10H1,
		None
	};


	class Faces;
	class QTUSER_3D_API HotbedEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		HotbedEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~HotbedEntity();

		void drawFace(bedType _bedType);
		void setVisibility(int faceIndex, bool visibility);
		void setColor(const QVector4D& color);
		//void setBedType(bedType _bedType);
		//int faceNum();
		void clearData();
		void checkBed(QList<Box3D>& boxes);
	protected:
		bedType m_bedType;
		std::vector<Faces*> m_bedFaces;
		std::vector<Box3D> m_hotZone;
		std::vector<bool> m_isHots;

		std::map<bedType, std::vector<Faces*>> m_mapofBedFacesByType;
	};
}
#endif // QTUSER_3D_HOTBEDENTITY_1595559517621_H