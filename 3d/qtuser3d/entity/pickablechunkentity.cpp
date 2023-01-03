#include "pickablechunkentity.h"
#include "qtuser3d/math/space3d.h"
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
		m_chunkBytes = 9 * m_chunkFaces * sizeof(float);
		int vertexSize = 3 * m_chunkFaces * m_chunks;

		BufferHelper::setAttributeCount(m_positionAttribute, vertexSize);
		BufferHelper::setAttributeCount(m_normalAttribute, vertexSize);
		BufferHelper::setAttributeCount(m_flagAttribute, vertexSize);
		 
		m_freeList.reserve(m_chunks);
		m_fillList.clear();
		for (int i = 0; i < m_chunks; ++i)
			m_freeList << i;
	}

	int PickableChunkEntity::freeChunk()
	{
		if (m_freeList.size() == 0)
			return -1;

		int freeIndex = m_freeList.front();
		m_freeList.pop_front();
		m_fillList.push_back(freeIndex);
		return freeIndex;
	}

	int PickableChunkEntity::fillChunkCount()
	{
		return m_fillList.count();
	}

	int PickableChunkEntity::fillVertexSize() const
	{
		int total = m_chunkFaces * m_fillList.size();
		return total * 3;
	}

	int PickableChunkEntity::fillBytesSize() const
	{
		int bytes = sizeof(float) * 3;
		return bytes * fillVertexSize();
	}

	int PickableChunkEntity::chunkBytes() const
	{
		return m_chunkBytes;
	}

	int PickableChunkEntity::freeChunkCount()
	{
		return m_freeList.count();
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

	int PickableChunkEntity::fillDatas(char* buffer)
	{
		int size = 0;
		char* tbuffer = buffer;
		for (int i : m_fillList)
		{
			size += fillChunkDatas(tbuffer, i);
			tbuffer += chunkBytes();
		}
		return size;
	}

	bool PickableChunkEntity::haveDatas() const
	{
		return !(m_freeList.size() == m_chunks);
	}

	int PickableChunkEntity::findChunkById(long id)
	{
		return -1;
	}

	int PickableChunkEntity::findChunkByFaceId(long faceid)
	{
		if (faceid >= m_faceRange.x() && faceid < m_faceRange.y())
		{
			int chunk = (faceid - m_faceRange.x()) / m_chunkFaces;
			return chunk;
		}
		return -1;
	}

	long PickableChunkEntity::findIdByFaceId(long faceid)
	{
		int index = findChunkByFaceId(faceid);
		return -1;
	}

	long PickableChunkEntity::findIdByChunk(int chunk)
	{
		return 0;
	}

	int PickableChunkEntity::allFaces()
	{
		return m_chunkFaces * m_chunks;
	}

	bool PickableChunkEntity::matchChunkFaces(int chunk_faces)
	{
		return m_chunkFaces == chunk_faces;
	}

	bool PickableChunkEntity::containId(long id)
	{
		return false;
	}

	int PickableChunkEntity::freeChunkEx()
	{
		int chunk = freeChunk();
		if (chunk < 0)
		{
			return -1;
		}

		return chunk;
	}

	int PickableChunkEntity::fillChunkDatas(char* buffer, int chunk)
	{
		if (!testChunkValid(chunk))
			return 0;

		int start = chunkVertexHead(chunk);
		int end = chunkVertexHead(chunk + 1);
		BufferHelper::copyAttribute(buffer, m_positionAttribute, start, end);
		return end - start;
	}

	int PickableChunkEntity::chunkVertexHead(int chunk)
	{
		return 3 * m_chunkFaces * chunk;
	}

	void PickableChunkEntity::releaseChunk(int chunk)
	{
		qDebug() << "PickableChunkEntity releaseChunk " << chunk;
		if (chunk < 0 || chunk >= m_chunks)
			return;

		int start = chunkVertexHead(chunk);
		int end = chunkVertexHead(chunk + 1);
		BufferHelper::releaseAttribute(m_positionAttribute, start, end);
		BufferHelper::releaseAttribute(m_normalAttribute, start, end);
		BufferHelper::releaseAttribute(m_flagAttribute, start, end);

		m_freeList << chunk;
		m_fillList.removeOne(chunk);
	}

	void PickableChunkEntity::releaseAllChunks()
	{
		qDebug() << "PickableChunkEntity::releaseAllChunks";
		m_freeList.clear();
		m_fillList.clear();
		for (int i = 0; i < m_chunks; ++i)
			m_freeList << i;

		BufferHelper::clearAttributeBuffer(m_flagAttribute);
	}

	void PickableChunkEntity::check(int faceID, const Ray& ray, QVector3D& position, QVector3D& normal)
	{
		int index = faceID - m_faceRange.x();

		if (index >= 0 && index < m_chunkFaces * m_chunks)
		{
			///QVector3D* positionBuffer = (QVector3D*)m_positionBuffer->data().constData();
			///QVector3D* normalBuffer = (QVector3D*)m_normalBuffer->data().constData();
			///positionBuffer += 3 * index;
			///normalBuffer += 3 * index;
			///
			///normal = *normalBuffer;
			///QVector3D v0 = *(positionBuffer);
			///lineCollidePlane(v0, normal, ray, position);
		}
	}
}