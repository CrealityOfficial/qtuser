#ifndef QTUSER_3D_SUPPORTCHUNKENTITY_1595472726414_H
#define QTUSER_3D_SUPPORTCHUNKENTITY_1595472726414_H
#include "qtuser3d/entity/pickablechunkentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API SupportChunkEntity : public PickableChunkEntity
	{
		Q_OBJECT
	public:
		SupportChunkEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~SupportChunkEntity();
	};
}
#endif // QTUSER_3D_SUPPORTCHUNKENTITY_1595472726414_H