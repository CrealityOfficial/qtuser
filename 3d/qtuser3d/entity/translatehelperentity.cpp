#include "qtuser3d/entity/translatehelperentity.h"
#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/module/manipulatepickable.h"

#include "qtuser3d/utils/primitiveshapecache.h"
namespace qtuser_3d
{
	TranslateHelperEntity::TranslateHelperEntity(Qt3DCore::QNode* parent, int type)
		:Qt3DCore::QEntity(parent)
		, m_xArrowEntity(nullptr)
		, m_yArrowEntity(nullptr)
		, m_zArrowEntity(nullptr)
		, m_xPickable(nullptr)
		, m_yPickable(nullptr)
		, m_zPickable(nullptr)
		, m_fixSize(false)
	{
		m_transform = new Qt3DCore::QTransform(this);
		addComponent(m_transform);

		Qt3DRender::QGeometry* geometry = PRIMITIVESHAPE("arrow");

		if (type & 1)
		{
			QMatrix4x4 xMatrix;
			xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
			xMatrix.scale(50.0f, 50.0f, 50.0f);

			m_xArrowEntity = new qtuser_3d::ManipulateEntity(this);
			m_xArrowEntity->setPose(xMatrix);
			m_xArrowEntity->setColor(QVector4D(1.0f, 0.0f, 0.0f, 1.0f));
			m_xArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);

			m_xPickable = new ManipulatePickable(this);
			m_xPickable->setPickableEntity(m_xArrowEntity);
		}
		if (type & 2)
		{
			QMatrix4x4 yMatrix;
			yMatrix.scale(50.0f, 50.0f, 50.0f);

			m_yArrowEntity = new qtuser_3d::ManipulateEntity(this);
			m_yArrowEntity->setPose(yMatrix);
			m_yArrowEntity->setColor(QVector4D(0.0f, 1.0f, 0.0f, 1.0f));
			m_yArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);

			m_yPickable = new ManipulatePickable(this);
			m_yPickable->setPickableEntity(m_yArrowEntity);
		}
		if (type & 4)
		{
			QMatrix4x4 zMatrix;
			zMatrix.rotate(90.0f, 1.0f, 0.0f, 0.0f);
			zMatrix.scale(50.0f, 50.0f, 50.0f);

			m_zArrowEntity = new qtuser_3d::ManipulateEntity(this);
			m_zArrowEntity->setPose(zMatrix);
			m_zArrowEntity->setColor(QVector4D(0.0f, 0.0f, 1.0f, 1.0f));
			m_zArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);

			m_zPickable = new ManipulatePickable(this);
			m_zPickable->setPickableEntity(m_zArrowEntity);
		}
	}

	TranslateHelperEntity::~TranslateHelperEntity()
	{
		if (m_xArrowEntity && m_xArrowEntity->parent())
			delete m_xArrowEntity;
		if (m_yArrowEntity && m_yArrowEntity->parent())
			delete m_yArrowEntity;
		if (m_zArrowEntity && m_zArrowEntity->parent())
			delete m_zArrowEntity;
	}

	Pickable* TranslateHelperEntity::xPickable()
	{
		return m_xPickable;
	}

	Pickable* TranslateHelperEntity::yPickable()
	{
		return m_yPickable;
	}

	Pickable* TranslateHelperEntity::zPickable()
	{
		return m_zPickable;
	}

	void TranslateHelperEntity::setXVisibility(bool visibility)
	{
		if(m_xArrowEntity)
			visibility ? m_xArrowEntity->setParent(this) : m_xArrowEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void TranslateHelperEntity::setYVisibility(bool visibility)
	{
		if(m_yArrowEntity)
			visibility ? m_yArrowEntity->setParent(this) : m_yArrowEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void TranslateHelperEntity::setZVisibility(bool visibility)
	{
		if(m_zArrowEntity)
			visibility ? m_zArrowEntity->setParent(this) : m_zArrowEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void TranslateHelperEntity::updateBox(const Box3D& box)
	{
		QVector3D center = box.center();
		QMatrix4x4 matrix;
		matrix.translate(center);
		m_center = center;

		float len = 1.0f;
		if (!m_fixSize)
		{
			QVector3D sz = box.size();
			double maxlen = sz.x() > sz.y() ? sz.x() : sz.y();
			maxlen = maxlen > sz.z() ? maxlen : sz.z();
			if (maxlen > 80)
			{
				len = maxlen / 80;
			}
		}

		matrix.scale(len);
		m_transform->setMatrix(matrix);
	}

	void TranslateHelperEntity::setFixSize(bool fixSize)
	{
		m_fixSize = fixSize;
	}

	QVector3D TranslateHelperEntity::center()
	{
		return m_center;
	}
}
