#ifndef QTUSER_3D_CHUNKGROUPENTITY_1612248232078_H
#define QTUSER_3D_CHUNKGROUPENTITY_1612248232078_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/entity/pickablechunkentity.h"
#include <QtCore/QMap>

namespace qtuser_3d
{
	class QTUSER_3D_API ChunkGroupEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		ChunkGroupEntity(int chunkFaces, int chunks, Qt3DCore::QNode* parent = nullptr);
		virtual ~ChunkGroupEntity();

		void setFaceBase(QPoint& faceBase);
		void setUsedEffect(Qt3DRender::QEffect* effect);
		ChunkBuffer* traitChunkBuffer(int faceID);
		ChunkBufferUser* traitChunkBufferUser(int faceID);

		ChunkBuffer* freeChunk();
	protected:
		PickableChunkEntity* freeEntity();
		void updateChunkEntityFaceBase(PickableChunkEntity* entity);
	protected:
		QPoint m_faceBase;
		QPoint m_currentBase;

		int m_chunkFaces;
		int m_chunks;

		Qt3DRender::QEffect* m_usedEffect;

		QVector<PickableChunkEntity*> m_entities;
	};
}

#endif // QTUSER_3D_CHUNKGROUPENTITY_1612248232078_H