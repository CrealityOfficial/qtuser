#include "supportposentity.h"
#include "qtuser3d/geometry/basicshapecreatehelper.h"
#include "qtuser3d/effect/effectmanager.h"


namespace qtuser_3d
{

	SupportPosEntity::SupportPosEntity(QVector3D position, QObject* parent)
		: PickableEntity(nullptr)
		, m_position(position)
	{
		setGeometry(BasicShapeCreateHelper::createBall(position, 1, 10));
		m_pointNum = (180 / 10) * (360 / 10) * 2 * 3;
	}

	SupportPosEntity::~SupportPosEntity()
	{
		//
	}

	QVector3D SupportPosEntity::position()
	{
		return m_position;
	}

	int SupportPosEntity::pointNum()
	{
		return m_pointNum;
	}

	int SupportPosEntity::offset(int num)
	{
		QVariant v = m_vertexBaseParameter->value();
		QPoint pt = v.value<QPoint>();
		pt.setX(pt.x() + m_pointNum * num);
		m_vertexBaseParameter->setValue(pt);
		return 0;
	}

	QPoint SupportPosEntity::vertexBase() const
	{
		QVariant v = m_vertexBaseParameter->value();
		QPoint pt = v.value<QPoint>();
		return pt;
	}

	void SupportPosEntity::changeFaceBase(int facebase)
	{
		QVariant v = m_vertexBaseParameter->value();
		QPoint pt = v.value<QPoint>();
		pt.setX(pt.x() % 200000 + facebase * 3);
		m_vertexBaseParameter->setValue(pt);
	}

}

