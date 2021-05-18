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
		int index = (int)state;
		if (index < 0) index = 0;
		if (index > 2) index = 2;
		if (m_pickableEntity)
			m_pickableEntity->setState(m_stateFactor[index]);
	}

	void ManipulatePickable::setStateFactor(float sf[3])
	{
		for (int i = 0; i < 3; i++)
		{
			m_stateFactor[i] = sf[i];
		}
	}

	void ManipulatePickable::pickableEntityChanged()
	{
		onStateChanged(m_state);
	}
}