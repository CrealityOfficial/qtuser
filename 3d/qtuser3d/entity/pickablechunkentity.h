#ifndef QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H
#define QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/module/chunkbuffer.h"
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

namespace qtuser_3d
{
	class QTUSER_3D_API PickableChunkEntity : public PickableEntity
		, public ChunkBuffer
	{
		Q_OBJECT
	public:
		PickableChunkEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PickableChunkEntity();

		void create(int chunkFaces, int chunks);
		void setFaceBase(QPoint faceBase);

		int freeChunk() override;
		bool full() override;
		bool checkFace(int chunk, int faceID) override;
		int relativeFaceID(int chunk, int faceID) override;
		int chunkFaces() override;
		void releaseChunk(int chunk) override;
		void updateChunk(int chunk, QByteArray* positionBytes, QByteArray* flagsBytes) override;
		void releaseAllChunks();
		void check(int faceID, const Ray& ray, QVector3D& position, QVector3D& normal) override;
		void setChunkUser(int chunk, ChunkBufferUser* user) override;
		ChunkBufferUser* chunkUser(int chunk) override;
		bool faceIDIn(int faceID);
		ChunkBufferUser* chunkUserFromFaceID(int faceID);

		int chunkIndex(int chunkID);
	protected:
		Qt3DRender::QGeometry* m_geometry;

		Qt3DRender::QAttribute* m_positionAttribute;
		Qt3DRender::QAttribute* m_normalAttribute;
		Qt3DRender::QAttribute* m_flagAttribute;

		QList<int> m_freeList;
		int m_chunkFaces;
		int m_chunkBytes;
		int m_chunks;

		QPoint m_faceRange;

		QVector<ChunkBufferUser*> m_users;
	};
}
#endif // QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H