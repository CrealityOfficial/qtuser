#ifndef QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H
#define QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/math/ray.h"
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

namespace qtuser_3d
{
	class QTUSER_3D_API PickableChunkEntity : public PickableEntity
	{
		Q_OBJECT
	public:
		PickableChunkEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PickableChunkEntity();

		void create(int chunkFaces, int chunks);
		void setFaceBase(QPoint faceBase);

		int freeChunk();
		int fillChunkCount();
		int fillVertexSize() const;
		int fillBytesSize() const;
		int chunkBytes() const;

		int freeChunkCount();
		bool full();
		bool checkFace(int chunk, int faceID);
		int relativeFaceID(int chunk, int faceID);
		int chunkFaces();
		void releaseChunk(int chunk);
		void updateChunk(int chunk, QByteArray* positionBytes, QByteArray* flagsBytes);
		void releaseAllChunks();
		void check(int faceID, const Ray& ray, QVector3D& position, QVector3D& normal);
		bool faceIDIn(int faceID);
		int fillDatas(char* buffer);   // return vertex count
		bool haveDatas() const;

		int findChunkById(long id);
		int findChunkByFaceId(long faceid);
		long findIdByFaceId(long faceid);
		long findIdByChunk(int chunk);

		int allFaces();
		bool matchChunkFaces(int chunk_faces);

		bool containId(long id);

		int freeChunkEx();
	protected:
		int fillChunkDatas(char* buffer, int chunk);  // return vertex count
		int chunkVertexHead(int chunk);

		inline bool testChunkValid(int chunk)
		{
			return chunk >= 0 && chunk < m_chunks;
		}
	protected:
		Qt3DRender::QGeometry* m_geometry;

		Qt3DRender::QAttribute* m_positionAttribute;
		Qt3DRender::QAttribute* m_normalAttribute;
		Qt3DRender::QAttribute* m_flagAttribute;

		QList<int> m_freeList;
		QList<int> m_fillList;
		int m_chunkFaces;
		int m_chunkBytes;
		int m_chunks;

		QPoint m_faceRange;
	};
}
#endif // QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H