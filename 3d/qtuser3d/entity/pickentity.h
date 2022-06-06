#ifndef QTUSER_3D_PICKENTITY_1595086493891_H
#define QTUSER_3D_PICKENTITY_1595086493891_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/module/pickable.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PickEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		PickEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PickEntity();

		Pickable* pickable();
		void bindPickable(Pickable* pickable);
	protected slots:
		void slotStateChanged(ControlState state);
		void slotFaceBaseChanged(int faceBase);
	protected:
		Pickable* m_pickable;

		Qt3DRender::QParameter* m_stateParameter;
		Qt3DRender::QParameter* m_vertexBaseParameter;
	};
}
#endif // QTUSER_3D_PICKENTITY_1595086493891_H