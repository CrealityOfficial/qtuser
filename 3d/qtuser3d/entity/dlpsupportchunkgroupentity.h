#ifndef QTUSER_3D_DLPSUPPORTCHUNKGROUPENTITY_1595824470375_H
#define QTUSER_3D_DLPSUPPORTCHUNKGROUPENTITY_1595824470375_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/module/dlpchunkbufferprovider.h"
#include <QtCore/QMap>

namespace qtuser_3d
{
	struct ChunkProtoInfo
	{
		int chunkFaces;
		int chunks;
	};
	class SupportChunkEntity;
	class QTUSER_3D_API DLPSupportChunkGroupEntity : public BasicEntity, public DLPChunkBufferProvider
	{
		Q_OBJECT
	public:
		DLPSupportChunkGroupEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~DLPSupportChunkGroupEntity();

		void setFaceBase(QPoint& faceBase);
		void setUsedEffect(Qt3DRender::QEffect* effect);
		ChunkBuffer* traitChunkBuffer(int faceID);
		ChunkBufferUser* traitChunkBufferUser(int faceID);
	protected:
		SupportChunkEntity* freeChunkEntity(QVector<SupportChunkEntity*>& chunkEntities, ChunkProtoInfo& info);

		ChunkBuffer* freeSupportChunk(DLPUserType userType) override;

		void updateChunkEntityFaceBase(SupportChunkEntity* entity, ChunkProtoInfo& info);
	protected:
		QPoint m_faceBase;
		QPoint m_currentBase;
		QVector<SupportChunkEntity*> m_dlpSupportEntities;
		QVector<SupportChunkEntity*> m_crossEntities;

		Qt3DRender::QEffect* m_usedEffect;

		QVector<QVector<SupportChunkEntity*>> m_typeEntities;
	};
}
#endif // QTUSER_3D_DLPSUPPORTCHUNKGROUPENTITY_1595824470375_H