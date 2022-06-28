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
		bool full();
		bool checkFace(int chunk, int faceID);
		int relativeFaceID(int chunk, int faceID);
		int chunkFaces();
		void releaseChunk(int chunk);
		void updateChunk(int chunk, QByteArray* positionBytes, QByteArray* flagsBytes);
		void releaseAllChunks();
		void check(int faceID, const Ray& ray, QVector3D& position, QVector3D& normal);
		bool faceIDIn(int faceID);
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
	};
}
#endif // QTUSER_3D_PICKABLECHUNKENTITY_1595378430410_H