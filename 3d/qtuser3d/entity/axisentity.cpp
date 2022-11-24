#include "axisentity.h"
#include "qtuser3d/entity/purecolorentity.h"

#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/entity/textmeshentity.h"
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
			geometry = PRIMITIVESHAPE("arrow");
		}
		else if (axistype == 1)
		{
			s = QVector3D(2, 3.3, 2);
			geometry = PRIMITIVESHAPE("cylinder");
		}
		if (s_use != nullptr)
		{
			s = *s_use;
		}

		QMatrix4x4 xMatrix;
		xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
		xMatrix.scale(s);
		QMatrix4x4 yMatrix;
		yMatrix.scale(s);
		QMatrix4x4 zMatrix;
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


		QMatrix4x4 x_txtmatrix;
		TextMeshEntity* textEntity = new TextMeshEntity(this);
		textEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
		textEntity->setText("X");
		x_txtmatrix.scale(3);
		x_txtmatrix.translate(7.0f, 0.0f, 0.0f);
		x_txtmatrix.rotate(90, 1, 0, 0);
		textEntity->setPose(x_txtmatrix);

		QMatrix4x4 y_txtmatrix;
		TextMeshEntity* textEntity2 = new TextMeshEntity(this);
		textEntity2->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
		textEntity2->setText("Y");
		y_txtmatrix.scale(3);
		y_txtmatrix.translate(-0.1f, 7.0f, 0.0f);
		y_txtmatrix.rotate(90, 1, 0, 0);
		textEntity2->setPose(y_txtmatrix);

		QMatrix4x4 z_txtmatrix;
		TextMeshEntity* textEntity3 = new TextMeshEntity(this);
		textEntity3->setColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
		textEntity3->setText("Z");
		z_txtmatrix.scale(3);
		z_txtmatrix.translate(-0.1f, 0.0f, 7.0f);
		z_txtmatrix.rotate(90, 1, 0, 0);
		textEntity3->setPose(z_txtmatrix);
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