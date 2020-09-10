#include "simplepickable.h"
#include "qtuser3d/entity/pickableentity.h"

namespace qtuser_3d
{
	SimplePickable::SimplePickable(QObject* parent)
		:Pickable(parent)
		, m_pickableEntity(nullptr)
	{
		m_faceBase = 0;
		m_noPrimitive = true;
	}
	
	SimplePickable::~SimplePickable()
	{
	}

	void SimplePickable::setPickableEntity(PickableEntity* entity)
	{
		m_pickableEntity = entity;
		if (m_pickableEntity)
		{
			m_pickableEntity->setState((float)m_state);

			QPoint vertexBase;
			vertexBase.setX(m_faceBase * 3);
			vertexBase.setY(m_noPrimitive ? 1 : 0);
			m_pickableEntity->setVertexBase(vertexBase);

			pickableEntityChanged();
		}
	}

	void SimplePickable::pickableEntityChanged()
	{

	}

	void SimplePickable::onStateChanged(ControlState state)
	{
		if (m_pickableEntity)
			m_pickableEntity->setState((float)state);
	}

	void SimplePickable::faceBaseChanged(int faceBase)
	{
		if (m_pickableEntity)
		{
			QPoint vertexBase;
			vertexBase.setX(faceBase * 3);
			vertexBase.setY(m_noPrimitive ? 1 : 0);
			m_pickableEntity->setVertexBase(vertexBase);
		}
	}
}