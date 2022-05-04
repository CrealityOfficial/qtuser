#include "instructionsentity.h"
#include "qtuser3d/entity/purecolorentity.h"

#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/geometry/basicshapecreatehelper.h"
namespace qtuser_3d
{
	InstructionsEntity::InstructionsEntity(Qt3DCore::QNode* parent, int type)
		: QEntity(parent)
		, m_xAxis(nullptr)
		, m_yAxis(nullptr)
		, m_zAxis(nullptr)
	{
		if (type & 0x1)
		{
			m_xAxis = new qtuser_3d::PureColorEntity(this);
			m_xAxis->setColor(QVector4D(1.00f, 0.77f, 0.77f, 1.0f));
		}
		if (type & 0x2)
		{
			m_yAxis = new qtuser_3d::PureColorEntity(this);
			m_yAxis->setColor(QVector4D(0.71f, 0.88f, 0.70f, 1.0f));
		}
		if (type & 0x4)
		{
			m_zAxis = new qtuser_3d::PureColorEntity(this);
			m_zAxis->setColor(QVector4D(0.0f, 0.0f, 0.5f, 1.0f));
		}
	}
	
	InstructionsEntity::~InstructionsEntity()
	{
	}

	void InstructionsEntity::translate(QVector3D v)
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

	void InstructionsEntity::updateGlobal(Box3D& box)
	{
		float extendlen = 4;
		QVector3D sz = box.size();
		if (m_xAxis)
		{
			QMatrix4x4 xMatrix;
			xMatrix.translate(0, sz.y() / 2.0);
			xMatrix.rotate(90.0f, 0.0f, 1.0f, 0.0f);

			m_xAxis->setPose(xMatrix);
			Qt3DRender::QGeometry* geometry = BasicShapeCreateHelper::createInstructions(0.2, sz.x() + extendlen, 1, 8);
			m_xAxis->setGeometry(geometry);
		}
		if (m_yAxis)
		{
			QMatrix4x4 yMatrix;
			yMatrix.translate(sz.x() / 2.0, 0);
			yMatrix.rotate(-90.0f, 1.0f, 0.0f, 0.0f);

			m_yAxis->setPose(yMatrix);
			Qt3DRender::QGeometry* geometry = BasicShapeCreateHelper::createInstructions(0.2, sz.y() + extendlen, 1, 8);
			m_yAxis->setGeometry(geometry);
		}
		if (m_zAxis)
		{
			QMatrix4x4 zMatrix;
			zMatrix.translate(sz.x() / 2.0, sz.y() / 2.0);

			m_zAxis->setPose(zMatrix);
			Qt3DRender::QGeometry* geometry = BasicShapeCreateHelper::createInstructions(0.2, sz.z() + extendlen, 1, 8);
			m_zAxis->setGeometry(geometry);
		}
	}

	void InstructionsEntity::updateGlobal2(Box3D& box)
	{
		float extendlen = 4;
		QVector3D sz = box.size();
		if (m_xAxis)
		{
			QMatrix4x4 xMatrix;
			xMatrix.translate(0, sz.y() / 2.0);
			xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);

			m_xAxis->setPose(xMatrix);
			Qt3DRender::QGeometry* geometry = BasicShapeCreateHelper::createInstructions(0.2, sz.x() + extendlen, 1, 8);
			m_xAxis->setGeometry(geometry);
		}
		if (m_yAxis)
		{
			QMatrix4x4 yMatrix;
			yMatrix.translate(sz.x() / 2.0, 0);

			m_yAxis->setPose(yMatrix);
			Qt3DRender::QGeometry* geometry = BasicShapeCreateHelper::createInstructions(0.2, sz.y() + extendlen, 1, 8);
			m_yAxis->setGeometry(geometry);
		}
		if (m_zAxis)
		{
			QMatrix4x4 zMatrix;
			zMatrix.translate(sz.x() / 2.0, sz.y() / 2.0);
			zMatrix.rotate(90.0f, 1.0f, 0.0f, 1.0f);

			m_zAxis->setPose(zMatrix);
			Qt3DRender::QGeometry* geometry = BasicShapeCreateHelper::createInstructions(0.2, sz.z() + extendlen, 1, 8);
			m_zAxis->setGeometry(geometry);
		}
	}
}