#ifndef QTUSER_3D_BOXENTITY_1594866500149_H
#define QTUSER_3D_BOXENTITY_1594866500149_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API BoxEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		BoxEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~BoxEntity();

		void updateGlobal(const Box3D& box, bool need_bottom = true);
		void updateLocal(const Box3D& box, float ratio = 0.3f);
		void update(const Box3D& box);
		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // QTUSER_3D_BOXENTITY_1594866500149_H