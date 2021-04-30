#include "modelnentity.h"
#include "qtuser3d/entity/boxentity.h"
#include "qtuser3d/entity/lineentity.h"

namespace qtuser_3d
{
	ModelNEntity::ModelNEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_boxEntity = new qtuser_3d::BoxEntity();
		m_boxEntity->setColor(QVector4D(0.306f, 0.702f, 0.769f, 1.0f));

		m_convexXYEntity = new qtuser_3d::LineEntity(this);

		m_stateParameter = createParameter("state", 0.0f);
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
		m_errorParameter = createParameter("error", 0.0f);
		m_supportCosParameter = createParameter("supportCos", 0.7);
		m_hoverParameter = createParameter("hoverState", 0);
		m_waterParameter = createParameter("water", QVector3D(0, 0, -10000));
		m_zchaParameter = createParameter("zcha", 0.01);
		m_waterShowParameter = createParameter("waterState", 0);
		m_fanzhuanParameter = createParameter("fanzhuan", 0);

		m_checkScopeParameter = createParameter("checkscope", 1);

		m_nozzleParameter = createParameter("nozzle", 0);
	}
	
	ModelNEntity::~ModelNEntity()
	{
		if (!m_boxEntity->parent())
			delete m_boxEntity;
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
		m_hoverParameter->setValue(1);
	}

	void ModelNEntity::leaveSupportStatus()
	{
		m_hoverParameter->setValue(0);
	}

	void ModelNEntity::setState(float state)
	{
		qDebug() << "model setState " << state;
		m_stateParameter->setValue(state);
	}

	void ModelNEntity::setNozzle(float nozzle)
	{
		m_nozzleParameter->setValue(nozzle);
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
		m_waterShowParameter->setValue(1);
		m_waterParameter->setValue(pt);
		m_zchaParameter->setValue(zcha);
	}

	void ModelNEntity::unSetWaterPoint()
	{
		m_waterShowParameter->setValue(0);
	}

	void ModelNEntity::setFanZhuan(int fz)
	{
		m_fanzhuanParameter->setValue(fz);
	}

	void ModelNEntity::setNeedCheckScope(int checkscope)
	{
		m_checkScopeParameter->setValue(checkscope);
	}
}