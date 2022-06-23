#include "pickablechunkentity.h"
#include "qtuser3d/math/space3d.h"
#include "qtuser3d/module/chunkbufferuser.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtusercore/module/glcompatibility.h"

namespace qtuser_3d
{
	PickableChunkEntity::PickableChunkEntity(Qt3DCore::QNode* parent)
		:PickableEntity(parent)
		, m_chunkFaces(0)
		, m_chunkBytes(0)
		, m_chunks(0)
	{
		setObjectName("PickableChunkEntity");
		m_geometry = new Qt3DRender::QGeometry(m_geometryRenderer);

		m_positionAttribute = BufferHelper::createDefaultVertexAttribute();
		m_normalAttribute = BufferHelper::createDefaultNormalAttribute();
		m_flagAttribute = BufferHelper::createAttribute("vertexFlag", Qt3DRender::QAttribute::Float, 1);

		m_geometry->addAttribute(m_positionAttribute);
		m_geometry->addAttribute(m_normalAttribute);
		m_geometry->addAttribute(m_flagAttribute);

		setGeometry(m_geometry);
	}

	PickableChunkEntity::~PickableChunkEntity()
	{
	}

	void PickableChunkEntity::create(int chunkFaces, int chunks)
	{
		m_chunkFaces = chunkFaces;
		m_chunks = chunks;

		if (m_chunkFaces <= 0) m_chunkFaces = 100;
		if (m_chunks <= 0) m_chunks = 50;
		m_chunkBytes = 3 * m_chunkFaces * sizeof(float);
		int vertexSize = 3 * m_chunkFaces * m_chunks;

		BufferHelper::setAttributeCount(m_positionAttribute, vertexSize);
		BufferHelper::setAttributeCount(m_normalAttribute, vertexSize);
		BufferHelper::setAttributeCount(m_flagAttribute, vertexSize);
		 
		m_freeList.reserve(m_chunks);
		for (int i = 0; i < m_chunks; ++i)
			m_freeList << i;

		m_users.fill(nullptr, m_chunks);
	}

	int PickableChunkEntity::freeChunk()
	{
		if (m_freeList.size() == 0)
			return -1;

		int freeIndex = m_freeList.front();
		m_freeList.pop_front();
		return freeIndex;
	}

	bool PickableChunkEntity::full()
	{
		return m_freeList.empty();
	}

	void PickableChunkEntity::updateChunk(int chunk, QByteArray* positionBytes, QByteArray* flagsBytes)
	{
		if (chunk < 0 || chunk >= m_chunks)
			return;

		int start = 3 * m_chunkFaces * chunk;
		int end = 3 * m_chunkFaces * (chunk + 1);
		BufferHelper::updatePositionNormalAttributes(m_positionAttribute,
			positionBytes, m_normalAttribute, start, end);
		BufferHelper::updateOneStrideAttributes(m_flagAttribute, flagsBytes, start, end);
	}

	void PickableChunkEntity::setFaceBase(QPoint faceBase)
	{
		QPoint vertexBase(faceBase.x() * 3, faceBase.y());
		setVertexBase(vertexBase);

		m_faceRange.setX(faceBase.x());
		m_faceRange.setY(faceBase.x() + m_chunkFaces * m_chunks);
	}

	bool PickableChunkEntity::checkFace(int chunk, int faceID)
	{
		int start = m_faceRange.x() + chunk * m_chunkFaces;
		int end = start + m_chunkFaces;
		return faceID >= start && faceID < end;
	}

	int PickableChunkEntity::relativeFaceID(int chunk, int faceID)
	{
		return faceID - (m_faceRange.x() + chunk * m_chunkFaces);
	}

	int PickableChunkEntity::chunkFaces()
	{
		return m_chunkFaces;
	}

	bool PickableChunkEntity::faceIDIn(int faceID)
	{
		return faceID >= m_faceRange.x() && faceID < m_faceRange.y();
	}

	void PickableChunkEntity::releaseChunk(int chunk)
	{
		qDebug() << "PickableChunkEntity releaseChunk " << chunk;
		if (chunk < 0 || chunk >= m_chunks)
			return;

		int start = 3 * m_chunkFaces * chunk;
		int end = 3 * m_chunkFaces * (chunk + 1);
		BufferHelper::releaseAttribute(m_positionAttribute, start, end);
		BufferHelper::releaseAttribute(m_normalAttribute, start, end);
		BufferHelper::releaseAttribute(m_flagAttribute, start, end);

		m_freeList << chunk;
		m_users[chunk] = nullptr;
	}

	void PickableChunkEntity::releaseAllChunks()
	{
		qDebug() << "PickableChunkEntity::releaseAllChunks";
		m_freeList.clear();
		for (int i = 0; i < m_chunks; ++i)
			m_freeList << i;

		m_users.fill(nullptr, m_chunks);
		BufferHelper::clearAttributeBuffer(m_flagAttribute);
	}

	void PickableChunkEntity::check(int faceID, const Ray& ray, QVector3D& position, QVector3D& normal)
	{
		int index = faceID - m_faceRange.x();
		if (index >= 0 && index < m_chunkFaces * m_chunks)
			BufferHelper::attributeRayCheck(m_positionAttribute, m_normalAttribute, index,
				ray, position, normal);
	}

	void PickableChunkEntity::setChunkUser(int chunk, ChunkBufferUser* user)
	{
		if (chunk >= 0 && chunk < m_chunks)
			m_users[chunk] = user;
	}

	ChunkBufferUser* PickableChunkEntity::chunkUser(int chunk)
	{
		if (chunk >= 0 && chunk < m_chunks)
			return m_users[chunk];
	
		return nullptr;
	}

	ChunkBufferUser* PickableChunkEntity::chunkUserFromFaceID(int faceID)
	{
		if (faceID >= m_faceRange.x() && faceID < m_faceRange.y())
		{
			int chunk = (faceID - m_faceRange.x()) / m_chunkFaces;
			ChunkBufferUser* user = chunkUser(chunk);

			if (user && !user->tracked())
				user = nullptr;
			return user;
		}
		return nullptr;
	}

	int PickableChunkEntity::chunkIndex(int chunkID)
	{
		int index = -1;
		return index;
	}
}