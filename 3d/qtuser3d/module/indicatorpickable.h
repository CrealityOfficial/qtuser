#ifndef QTUSER_3D_INDICATOR_PICKABLE_H
#define QTUSER_3D_INDICATOR_PICKABLE_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/module/pickable.h"

namespace qtuser_3d
{
	class PickableEntity;
	class QTUSER_3D_API IndicatorPickable : public Pickable
	{
		Q_OBJECT
	public:
		IndicatorPickable(QObject* parent = nullptr);
		virtual ~IndicatorPickable();

		void setPickableEntity(PickableEntity* entity);

		int primitiveNum() override;
		
		bool isGroup() override;

		bool hoverPrimitive(int primitiveID) override;
		void unHoverPrimitive() override;

		void pick(int primitiveID) override;

	protected:
		void onStateChanged(ControlState state) override;
		void faceBaseChanged(int faceBase) override;
		
	protected:
		PickableEntity* m_pickableEntity;
	};
}
#endif // QTUSER_3D_INDICATOR_PICKABLE_H