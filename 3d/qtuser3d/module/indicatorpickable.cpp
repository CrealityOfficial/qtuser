#include "indicatorpickable.h"
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/entity/worldindicatorentity.h"

#include <QtCore/QDebug>

namespace qtuser_3d
{
	IndicatorPickable::IndicatorPickable(QObject* parent)
		:Pickable(parent)
		, m_pickableEntity(nullptr)
	{
		m_enableSelect = false;
	}
	
	IndicatorPickable::~IndicatorPickable()
	{
	}

	void IndicatorPickable::setPickableEntity(PickableEntity* entity)
	{
		m_pickableEntity = entity;
		if (m_pickableEntity)
		{
			m_pickableEntity->setState((float)m_state);

			QPoint vertexBase;
			vertexBase.setX(m_faceBase * 3);
			vertexBase.setY(m_noPrimitive ? 1 : 0);
			m_pickableEntity->setVertexBase(vertexBase);

		}
	}

	void IndicatorPickable::faceBaseChanged(int faceBase)
	{
		if (m_pickableEntity)
		{
			QPoint vertexBase;
			vertexBase.setX(faceBase * 3);
			vertexBase.setY(m_noPrimitive ? 1 : 0);
			m_pickableEntity->setVertexBase(vertexBase);
		}
	}

	int IndicatorPickable::primitiveNum()
	{
		return 1 << 7;
	}

	bool IndicatorPickable::hoverPrimitive(int primitiveID)
	{
		qDebug() << QString("IndicatorPickable::hoverPrimitive [%1]").arg(primitiveID);

		if (!m_pickableEntity)
			return false;

		WorldIndicatorEntity* entity = dynamic_cast<WorldIndicatorEntity*>(m_pickableEntity);
		if (entity == nullptr)
			return false;

		entity->setHighlightDirections(primitiveID);

		return true;
	}

	void IndicatorPickable::unHoverPrimitive()
	{
		qDebug() << QString("IndicatorPickable::unHoverPrimitive []");
		if (!m_pickableEntity)
			return;

		WorldIndicatorEntity* entity = dynamic_cast<WorldIndicatorEntity*>(m_pickableEntity);
		if (entity == nullptr)
			return;

		entity->setHighlightDirections(0);
	}

	void IndicatorPickable::onStateChanged(ControlState state)
	{
		
	}

	bool IndicatorPickable::isGroup()
	{
		return true;
	}

	void IndicatorPickable::pick(int primitiveID)
	{
		qDebug() << QString("IndicatorPickable::pick [%1]").arg(primitiveID);
		if (!m_pickableEntity)
			return;

		WorldIndicatorEntity* entity = dynamic_cast<WorldIndicatorEntity*>(m_pickableEntity);
		if (entity == nullptr)
			return;
		entity->setSelectedDirections(primitiveID);
	}
}