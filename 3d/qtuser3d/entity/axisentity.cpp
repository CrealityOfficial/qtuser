#include "axisentity.h"
#include "qtuser3d/entity/purecolorentity.h"

#include "qtuser3d/utils/primitiveshapecache.h"
namespace qtuser_3d
{
	AxisEntity::AxisEntity(Qt3DCore::QNode* parent)
		:QEntity(parent)
	{
		m_xAxis = new qtuser_3d::PureColorEntity(this);
		m_yAxis = new qtuser_3d::PureColorEntity(this);
		m_zAxis = new qtuser_3d::PureColorEntity(this);

		QMatrix4x4 xMatrix;
		xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
		xMatrix.scale(50.0f, 50.0f, 50.0f);
		QMatrix4x4 yMatrix;
		yMatrix.scale(50.0f, 50.0f, 50.0f);
		QMatrix4x4 zMatrix;
		zMatrix.rotate(90.0f, 1.0f, 0.0f, 0.0f);
		zMatrix.scale(50.0f, 50.0f, 50.0f);
		m_xAxis->setPose(xMatrix);
		m_yAxis->setPose(yMatrix);
		m_zAxis->setPose(zMatrix);

		m_xAxis->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		m_yAxis->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
		m_zAxis->setColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));

		m_xAxis->setGeometry(PRIMITIVESHAPE("arrow"));
		m_yAxis->setGeometry(PRIMITIVESHAPE("arrow"));
		m_zAxis->setGeometry(PRIMITIVESHAPE("arrow"));
	}
	
	AxisEntity::~AxisEntity()
	{
	}

	void AxisEntity::translate(QVector3D v)
	{
		QMatrix4x4 t;
		t.translate(v.x(), v.y());

		QMatrix4x4 xMatrix = m_xAxis->pose();
		xMatrix = t * xMatrix;
		m_xAxis->setPose(xMatrix);

		QMatrix4x4 yMatrix = m_yAxis->pose();
		yMatrix = t * yMatrix;
		m_yAxis->setPose(yMatrix);

		QMatrix4x4 zMatrix = m_zAxis->pose();
		zMatrix = t * zMatrix;
		m_zAxis->setPose(zMatrix);
	}
}