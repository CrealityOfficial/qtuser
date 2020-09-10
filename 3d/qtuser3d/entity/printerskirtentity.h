#ifndef QTUSER_3D_PRINTERSKIRTENTITY_1594887310960_H
#define QTUSER_3D_PRINTERSKIRTENTITY_1594887310960_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PrinterSkirtEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		PrinterSkirtEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterSkirtEntity();

		void updateBoundingBox(const Box3D& box);
		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // QTUSER_3D_PRINTERSKIRTENTITY_1594887310960_H