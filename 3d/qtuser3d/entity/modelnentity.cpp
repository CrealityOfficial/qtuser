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

		m_convexXYEntity = new qtuser_3d::LineEntity();
		m_convexXYEntity->setObjectName("ModelNEntity");

		m_stateParameter = createParameter("state", 0.0f);
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
		m_errorParameter = createParameter("error", 0.0f);
		m_supportCosParameter = createParameter("supportCos", 0.5);
		m_hoverParameter = createParameter("hoverState", 0);
		m_waterParameter = createParameter("water", QVector3D(0, 0, -10000));
		m_zchaParameter = createParameter("zcha", 0.01);
		m_waterShowParameter = createParameter("waterState", 0);
		m_fanzhuanParameter = createParameter("fanzhuan", 0);

		m_customColorParameter = createParameter("customColor", QColor::fromRgb(0.0, 0.0, 0.0));
		m_transparencyParameter = createParameter("transparency", 1.0f);
		m_lightingFlagParameter = createParameter("lightingEnable", 1);

		m_checkScopeParameter = createParameter("checkscope", 1);

		m_nozzleParameter = createParameter("nozzle", 0);

		m_renderModeParameter = createParameter("renderModel", 1);
	}
	
	ModelNEntity::~ModelNEntity()
	{
		if (!m_boxEntity->parent())
			delete m_boxEntity;
	}

	void ModelNEntity::setConvexName(QString name)
	{
		m_convexXYEntity->setObjectName(name);
	}

	void ModelNEntity::setBoxVisibility(bool visible)
	{
		m_boxEntity->setParent(visible ? (Qt3DCore::QNode*)this->parent() : nullptr);
		////m_convexXYEntity->setParent(visible ? (Qt3DCore::QNode*)this->parent() : nullptr);
	}

	void ModelNEntity::updateBoxLocal(const Box3D& box, const QMatrix4x4& parentMatrix)
	{
		m_boxEntity->update(box, parentMatrix);
	}

	void ModelNEntity::setBoxColor(QVector4D color)
	{
		m_boxEntity->setColor(color);
	}

	void ModelNEntity::updateConvex(QVector<QVector3D>& points, QMatrix4x4& matrix, bool loop)
	{
		m_convexXYEntity->updateGeometry(points, loop);
		//m_convexXYEntity->setPose(matrix);
		QMatrix4x4 matrix2;
		matrix2.setToIdentity();
		m_convexXYEntity->setPose(matrix2);
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

	void ModelNEntity::setWaterPoint(const QVector3D& pt, float zcha)
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

	void ModelNEntity::setCustomColor(QColor color)
	{
		m_customColorParameter->setValue(color);
	}

	QColor ModelNEntity::getCustomColor()
	{
		return m_customColorParameter->value().value<QColor>();
	}

	void ModelNEntity::setTransparency(float alpha)
	{
		m_transparencyParameter->setValue(alpha);
	}

	float ModelNEntity::getTransparency()
	{
		return m_transparencyParameter->value().toFloat();
	}

	void ModelNEntity::setLightingEnable(bool enable)
	{
		m_lightingFlagParameter->setValue(enable ? 1 : 0);
	}

	bool ModelNEntity::getLightingEnable()
	{
		return m_lightingFlagParameter->value().toBool();
	}

	void ModelNEntity::setNeedCheckScope(int checkscope)
	{
		m_checkScopeParameter->setValue(checkscope);
	}


	void ModelNEntity::setRenderMode(int mode)
	{
		m_renderModeParameter->setValue(mode);
	}
}