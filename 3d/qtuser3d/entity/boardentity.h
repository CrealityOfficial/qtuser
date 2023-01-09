#ifndef QTUSER3D_BOARDENTITY_H
#define QTUSER3D_BOARDENTITY_H

#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/basicentity.h"

#include <Qt3DRender/QTexture>


namespace qtuser_3d
{
	class QTUSER_3D_API BoardEntity : public BasicEntity
	{
	public:
		BoardEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~BoardEntity();

		void setBoardTexture(Qt3DRender::QTexture2D* texture);

	protected:
		Qt3DRender::QParameter* m_boardTextureParameter;
	};
}

#endif // QTUSER3D_BOARDENTITY_H