#include "supportentity.h"

#include "qtuser3d/effect/effectmanager.h"

namespace qtuser_3d
{
	SupportEntity::SupportEntity(Qt3DCore::QNode* parent)
		:PickableEntity(parent)
	{
		setEffect(EFFECTCREATE("support_pickFaceFlag.pick", m_material));
		
		m_geometry = new Qt3DRender::QGeometry(m_geometryRenderer);
		setGeometry(m_geometry);
		
		m_positionBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_geometry);
		m_normalBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_geometry);
		m_flagBuffer = new Qt3DRender::QBuffer(Qt3DRender::QBuffer::VertexBuffer, m_geometry);
		
		m_positionAttribute = new Qt3DRender::QAttribute(m_positionBuffer, Qt3DRender::QAttribute::defaultPositionAttributeName(), Qt3DRender::QAttribute::Float, 3, 0);
		m_normalAttribute = new Qt3DRender::QAttribute(m_normalBuffer, Qt3DRender::QAttribute::defaultNormalAttributeName(), Qt3DRender::QAttribute::Float, 3, 0);
		m_flagAttribute = new Qt3DRender::QAttribute(m_flagBuffer, "vertexFlag", Qt3DRender::QAttribute::Float, 1, 0);
		
		m_geometry->addAttribute(m_positionAttribute);
		m_geometry->addAttribute(m_normalAttribute);
		m_geometry->addAttribute(m_flagAttribute);
	}
	
	SupportEntity::~SupportEntity()
	{
	}

	void SupportEntity::setPositionBufferData(const QByteArray& bytes)
	{
		m_positionBuffer->setData(bytes);
	}

	void SupportEntity::setNormalBufferData(const QByteArray& bytes)
	{
		m_normalBuffer->setData(bytes);
	}

	void SupportEntity::setFlagsBufferData(const QByteArray& bytes)
	{
		m_flagBuffer->setData(bytes);
	}

	void SupportEntity::setCount(uint count)
	{
		m_positionAttribute->setCount(count);
		m_normalAttribute->setCount(count);
		m_flagAttribute->setCount(count);
	}

	void SupportEntity::updateFlagBufferData(int offset, const QByteArray& bytes)
	{
		m_flagBuffer->updateData(0, bytes);
	}
}