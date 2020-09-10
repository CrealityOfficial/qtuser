#include "pickableentity.h"

namespace qtuser_3d
{
	PickableEntity::PickableEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_stateParameter = createParameter("state", 0.0f);
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
	}
	
	PickableEntity::~PickableEntity()
	{
	}

	void PickableEntity::setState(float state)
	{
		m_stateParameter->setValue(state);
	}

	void PickableEntity::setVertexBase(QPoint vertexBase)
	{
		m_vertexBaseParameter->setValue(vertexBase);
	}
}