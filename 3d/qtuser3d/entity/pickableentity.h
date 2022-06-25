#ifndef QTUSER_3D_PICKABLEENTITY_1595086493891_H
#define QTUSER_3D_PICKABLEENTITY_1595086493891_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PickableEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		PickableEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PickableEntity();

		void setState(float state);
	protected:
		Qt3DRender::QParameter* m_stateParameter;
	};
}
#endif // QTUSER_3D_PICKABLEENTITY_1595086493891_H