#include "dlpsupportchunkgroupentity.h"
#include "qtuser3d/entity/supportchunkentity.h"

namespace qtuser_3d
{
	static bool entityEqualPredicate(SupportChunkEntity *i, SupportChunkEntity *j)
	{
		return i==j;
	};

	ChunkProtoInfo protoInfos[(int)DLPUserType::eDLPNum] =
	{
#ifdef DLP_ONLY
		{36, 400},
		{136, 400},
		{36, 400},
		{236, 400},
		{36, 400},
		{100, 400}
#else
		{476, 100},
		{476, 100},
		{36, 100},
		{76, 100},
		{76, 100}
#endif
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
	SupportChunkEntity* DLPSupportChunkGroupEntity::traitChunkEntity(int faceID)
	{
		SupportChunkEntity* buffer = nullptr;
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
	void DLPSupportChunkGroupEntity::CreateChunkEntityBaseMesh(std::vector<trimesh::TriMesh*>& meshes, qtuser_3d::DLPUserType type)
	{

		//QVector<SupportChunkEntity*> &typeEntities =(QVector<SupportChunkEntity*>) m_typeEntities.at((int)type);
		QVector<SupportChunkEntity*>& typeEntities = m_typeEntities[(int)type];
		for (trimesh::TriMesh* mesh : meshes)
		{
			int facesN = mesh->faces.size();
			int vertexsN = mesh->vertices.size();
			ChunkProtoInfo info = { facesN,1 };
			SupportChunkEntity* entity = freeChunkEntity(typeEntities, info);
			int chunk = entity->freeChunk();
			assert(chunk >= 0);
			QByteArray positionBytes;
			QByteArray flagBytes;
			positionBytes.resize(facesN * 3 * 3 * sizeof(float));
			trimesh::vec3* position = (trimesh::vec3*)positionBytes.data();
			for (trimesh::TriMesh::Face face : mesh->faces)
			{
				*position++ = mesh->vertices.at(face[0]);
				*position++ = mesh->vertices.at(face[1]);
				*position++ = mesh->vertices.at(face[2]);
			}
			{
				flagBytes.resize(facesN * 3 * sizeof(float));
				float* fdata = (float*)flagBytes.data();
				for (int i = 0; i < facesN * 3; ++i)
				{
					*fdata++ = (float)1;
				}
			}

			entity->updateChunk(chunk, &positionBytes, &flagBytes);
			//std::cout << __FILE__ << __FUNCTION__ << std::endl;
		}

	}
	void DLPSupportChunkGroupEntity::deleteChunkEntity(qtuser_3d::DLPUserType type)
	{

		//QVector<SupportChunkEntity*> &typeEntities =(QVector<SupportChunkEntity*>) m_typeEntities.at((int)type);
		QVector<SupportChunkEntity*>& typeEntities = m_typeEntities[(int)type];
		for (SupportChunkEntity* entity : typeEntities)
		{
			entity->releaseAllChunks();
			delete entity;
		}
		typeEntities.clear();
	}
	void DLPSupportChunkGroupEntity::deleteChunkEntity(SupportChunkEntity* entityPtr)
	{
		int num = (int)DLPUserType::eDLPNum;
		std::vector< SupportChunkEntity* >entityTarge;
		entityTarge.emplace_back(entityPtr);
		for (int i = 0; i < num; ++i)
		{
			QVector<SupportChunkEntity*>& entities = m_typeEntities[i];
			auto item=std::search(entities.begin(), entities.end(), entityTarge.begin(), entityTarge.end(), entityEqualPredicate);
			if (item != entities.end())
			{
				delete *item;
				entities.erase(item);
			}

		}
	}


}