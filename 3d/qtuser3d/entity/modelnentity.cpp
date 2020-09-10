#include "modelnentity.h"
#include "qtuser3d/entity/boxentity.h"
#include "qtuser3d/entity/lineentity.h"

namespace qtuser_3d
{
	ModelNEntity::ModelNEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_boxEntity = new qtuser_3d::BoxEntity();
		m_boxEntity->setColor(QVector4D(0.180f, 0.541f, 0.968f, 1.0f));

		m_convexXYEntity = new qtuser_3d::LineEntity(this);

		m_stateParameter = createParameter("state", 0.0f);
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
		m_errorParameter = createParameter("error", 0.0f);
		m_supportCosParameter = createParameter("supportCos", 0.7);
		m_showStateParameter = createParameter("showState", 0);
		m_waterParameter = createParameter("water", QVector3D(0, 0, -10000));
		m_zchaParameter = createParameter("zcha", 0.01);
	}
	
	ModelNEntity::~ModelNEntity()
	{
	}

	void ModelNEntity::setBoxVisibility(bool visible)
	{
		m_boxEntity->setParent(visible ? this : nullptr);
	}

	void ModelNEntity::updateBoxLocal(Box3D& box, QMatrix4x4& parentMatrix)
	{
		m_boxEntity->updateLocal(box, parentMatrix);
	}

	void ModelNEntity::updateConvex(QVector<QVector3D>& points, QMatrix4x4& matrix, bool loop)
	{
		m_convexXYEntity->updateGeometry(points, loop);
		m_convexXYEntity->setPose(matrix);
	}

	void ModelNEntity::enterSupportStatus()
	{
		m_showStateParameter->setValue(1);
	}

	void ModelNEntity::leaveSupportStatus()
	{
		m_showStateParameter->setValue(0);
	}

	void ModelNEntity::setState(float state)
	{
		m_stateParameter->setValue(state);
	}

	void ModelNEntity::setVertexBase(QPoint vertexBase)
	{
		m_vertexBaseParameter->setValue(vertexBase);
	}

	void ModelNEntity::setErrorState(bool error)
	{
		m_errorParameter->setValue(error ? 1.0f : 0.0f);
	}

	void ModelNEntity::setSupportCos(float supportCos)
	{
		m_supportCosParameter->setValue(supportCos);
	}

	void ModelNEntity::setWaterPoint(QVector3D pt, float zcha)
	{
		m_waterParameter->setValue(pt);
		m_zchaParameter->setValue(zcha);
	}
}