#include "dlpsupportchunkgroupentity.h"
#include "qtuser3d/entity/supportchunkentity.h"

namespace qtuser_3d
{
	ChunkProtoInfo protoInfos[(int)DLPUserType::eDLPNum] =
	{
		{476, 100},
		{476, 100},
		{36, 100},
		{76, 100},
		{76, 100},
		{400, 100}
	};

	DLPSupportChunkGroupEntity::DLPSupportChunkGroupEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
		, m_usedEffect(nullptr)
	{
		m_typeEntities.resize((int)DLPUserType::eDLPNum);
	}
	
	DLPSupportChunkGroupEntity::~DLPSupportChunkGroupEntity()
	{
	}

	ChunkBuffer* DLPSupportChunkGroupEntity::freeSupportChunk(DLPUserType userType)
	{
		int index = (int)userType;
		assert(index >= 0 && index < (int)DLPUserType::eDLPNum);
		return freeChunkEntity(m_typeEntities[index], protoInfos[index]);
	}

	void DLPSupportChunkGroupEntity::setFaceBase(QPoint& faceBase)
	{
		m_faceBase = faceBase;
		m_currentBase = m_faceBase;

		int num = (int)DLPUserType::eDLPNum;
		for (int i = 0; i < num; ++i)
		{
			QVector<SupportChunkEntity*>& entities = m_typeEntities[i];
			for (SupportChunkEntity* entity : entities)
				updateChunkEntityFaceBase(entity, protoInfos[i]);
		}
	}

	void DLPSupportChunkGroupEntity::setUsedEffect(Qt3DRender::QEffect* effect)
	{
		m_usedEffect = effect;
	}

	SupportChunkEntity* DLPSupportChunkGroupEntity::freeChunkEntity(QVector<SupportChunkEntity*>& chunkEntities, ChunkProtoInfo& info)
	{
		SupportChunkEntity* chunkEntity = nullptr;
		int size = chunkEntities.size();
		for (int i = 0; i < size; ++i)
		{
			SupportChunkEntity* tChunk = chunkEntities.at(i);
			if (!tChunk->full())
			{
				chunkEntity = tChunk;
				break;
			}
		}

		if (chunkEntity == nullptr)
		{
			chunkEntity = new SupportChunkEntity(this);
			chunkEntity->setEffect(m_usedEffect);

			chunkEntity->create(info.chunkFaces, info.chunks);
			chunkEntities.push_back(chunkEntity);

			updateChunkEntityFaceBase(chunkEntity, info);
		}
		return chunkEntity;
	}

	void DLPSupportChunkGroupEntity::updateChunkEntityFaceBase(SupportChunkEntity* entity, ChunkProtoInfo& info)
	{
		if (!entity)
			return;

		entity->setFaceBase(m_currentBase);
		m_currentBase.setX(m_currentBase.x() + info.chunkFaces * info.chunks);
	}

	ChunkBuffer* DLPSupportChunkGroupEntity::traitChunkBuffer(int faceID)
	{
		ChunkBuffer* buffer = nullptr;
		int num = (int)DLPUserType::eDLPNum;
		for (int i = 0; i < num; ++i)
		{
			QVector<SupportChunkEntity*>& entities = m_typeEntities[i];
			for (SupportChunkEntity* entity : entities)
			{
				if (entity->faceIDIn(faceID))
				{
					buffer = entity;
					break;
				}
			}
		}
		return buffer;
	}

	ChunkBufferUser* DLPSupportChunkGroupEntity::traitChunkBufferUser(int faceID)
	{
		ChunkBufferUser* bufferUser = nullptr;
		int num = (int)DLPUserType::eDLPNum;
		for (int i = 0; i < num; ++i)
		{
			QVector<SupportChunkEntity*>& entities = m_typeEntities[i];
			for (SupportChunkEntity* entity : entities)
			{
				bufferUser = entity->chunkUserFromFaceID(faceID);
				if (bufferUser)
				{
					break;
				}
			}

			if (bufferUser)
				break;
		}
		return bufferUser;
	}
}