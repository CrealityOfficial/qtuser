#include "pickentity.h"

namespace qtuser_3d
{
	PickEntity::PickEntity(Qt3DCore::QNode* parent)
		: BasicEntity(parent)
		, m_pickable(nullptr)
	{
		m_stateParameter = createParameter("state", 0.0f);
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
		bindPickable(new Pickable(this));
	}
	
	PickEntity::~PickEntity()
	{
	}

	Pickable* PickEntity::pickable()
	{
		return m_pickable;
	}

	void PickEntity::bindPickable(Pickable* pickable)
	{
		if (!pickable || (m_pickable == pickable))
			return;

		if (m_pickable)
		{
			disconnect(m_pickable, SIGNAL(signalStateChanged(ControlState)), this, SLOT(slotStateChanged(ControlState)));
			disconnect(m_pickable, SIGNAL(signalFaceBaseChanged(int)), this, SLOT(slotFaceBaseChanged(int)));
			delete m_pickable;
		}

		m_pickable = pickable;
		if (m_pickable)
		{
			connect(m_pickable, SIGNAL(signalStateChanged(ControlState)), this, SLOT(slotStateChanged(ControlState)));
			connect(m_pickable, SIGNAL(signalFaceBaseChanged(int)), this, SLOT(slotFaceBaseChanged(int)));
		}
	}

	void PickEntity::slotStateChanged(ControlState state)
	{
		m_stateParameter->setValue((float)state);
	}

	void PickEntity::slotFaceBaseChanged(int faceBase)
	{
		QPoint vertexBase;
		vertexBase.setX(m_pickable->primitiveNum() * 3);
		vertexBase.setY(m_pickable->noPrimitive() ? 1 : 0);
		m_vertexBaseParameter->setValue(vertexBase);
	}
}