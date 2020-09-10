#ifndef QTUSER_3D_CHUNKENTITY_1595378329326_H
#define QTUSER_3D_CHUNKENTITY_1595378329326_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API ChunkEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		ChunkEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~ChunkEntity();
	};
}
#endif // QTUSER_3D_CHUNKENTITY_1595378329326_H