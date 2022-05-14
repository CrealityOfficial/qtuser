#include "axisentity.h"
#include "qtuser3d/entity/purecolorentity.h"
#include "qtuser3d/utils/primitiveshapecache.h"

namespace qtuser_3d
{
	AxisEntity::AxisEntity(Qt3DCore::QNode* parent, int axistype, QVector3D* s_use)
		:QEntity(parent)
	{
		setObjectName("AxisEntity");
		m_xAxis = new qtuser_3d::PureColorEntity(this);
		m_xAxis->setObjectName("AxisEntity.xAxis");
		m_yAxis = new qtuser_3d::PureColorEntity(this);
		m_yAxis->setObjectName("AxisEntity.yAxis");
		m_zAxis = new qtuser_3d::PureColorEntity(this);
		m_zAxis->setObjectName("AxisEntity.zAxis");

		Qt3DRender::QGeometry* geometry = nullptr;
		QVector3D s(50, 50, 50);
		if (axistype == 0)
		{
			s = QVector3D(50, 50, 50);
			geometry = createTrianglesPrimitive("arrow");
		}
		else if (axistype == 1)
		{
			s = QVector3D(2.0f, 3.3f, 2.0f);
			geometry = createTrianglesPrimitive("cylinder");
		}
		if (s_use != nullptr)
		{
			s = *s_use;
		}

		QMatrix4x4 xMatrix;
		xMatrix.setToIdentity();
		xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
		xMatrix.scale(s);
		QMatrix4x4 yMatrix;
		yMatrix.setToIdentity();
		yMatrix.scale(s);
		QMatrix4x4 zMatrix;
		zMatrix.setToIdentity();
		zMatrix.rotate(90.0f, 1.0f, 0.0f, 0.0f);
		zMatrix.scale(s);
		m_xAxis->setPose(xMatrix);
		m_yAxis->setPose(yMatrix);
		m_zAxis->setPose(zMatrix);

		m_xAxis->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		m_yAxis->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
		m_zAxis->setColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));

		m_xAxis->setGeometry(geometry);	
		m_yAxis->setGeometry(geometry);
		m_zAxis->setGeometry(geometry);
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