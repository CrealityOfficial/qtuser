#include "pickentity.h"

namespace qtuser_3d
{
	PickEntity::PickEntity(Qt3DCore::QNode* parent)
		: BasicEntity(parent)
		, m_pickable(nullptr)
	{
		Pickable* pickable = new Pickable(this);
		pickable->setNoPrimitive(true);
		bindPickable(pickable);
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

			m_pickable->signalStateChanged(m_pickable->state());
			m_pickable->signalFaceBaseChanged(m_pickable->faceBase());
		}
	}

	void PickEntity::slotStateChanged(ControlState state)
	{
		onStateChanged(state);
	}

	void PickEntity::onStateChanged(ControlState state)
	{

	}

	void PickEntity::slotFaceBaseChanged(int faceBase)
	{
		QPoint vertexBase;
		vertexBase.setX(faceBase * 3);
		vertexBase.setY(m_pickable->noPrimitive() ? 1 : 0);
		m_vertexBaseParameter->setValue(vertexBase);
	}
}