#ifndef QTUSER_3D_SUPPORTCHUNKGROUPENTITY_1595474434671_H
#define QTUSER_3D_SUPPORTCHUNKGROUPENTITY_1595474434671_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class SupportChunkEntity;
	class QTUSER_3D_API SupportChunkGroupEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		SupportChunkGroupEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~SupportChunkGroupEntity();

		void setProto(int chunkFaces, int chunks);
		SupportChunkEntity* freeChunkEntity();
		void clearAllChunk();

		void setFaceBase(QPoint& faceBase);
		void setUsedEffect(Qt3DRender::QEffect* effect);
	protected:
		int m_chunkFacesProto;
		int m_chunksProto;

		QPoint m_faceBase;
		QVector<SupportChunkEntity*> m_chunkEntities;

		Qt3DRender::QEffect* m_usedEffect;
	};
}
#endif // QTUSER_3D_SUPPORTCHUNKGROUPENTITY_1595474434671_H