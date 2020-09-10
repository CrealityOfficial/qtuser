#include "manipulatepickable.h"
#include "qtuser3d/entity/pickableentity.h"

namespace qtuser_3d
{
	ManipulatePickable::ManipulatePickable(QObject* parent)
		:SimplePickable(parent)
	{
		setNoPrimitive(true);
	}
	
	ManipulatePickable::~ManipulatePickable()
	{
	}

	void ManipulatePickable::onStateChanged(ControlState state)
	{
		float stateFactor[3] = {1.0, 0.6, 0.6};
		int index = (int)state;
		if (index < 0) index = 0;
		if (index > 2) index = 2;
		if (m_pickableEntity)
			m_pickableEntity->setState(stateFactor[index]);
	}

	void ManipulatePickable::pickableEntityChanged()
	{
		onStateChanged(m_state);
	}
}