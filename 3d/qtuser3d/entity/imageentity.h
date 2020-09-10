#ifndef QTUSER_3D_IMAGE_ENTITY_H
#define QTUSER_3D_IMAGE_ENTITY_H

#include "qtuser3d/entity/basicentity.h"
#include <QtGui/QVector3D>
#include <vector>
#include <Qt3DRender/QTexture>
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	
	class QTUSER_3D_API ImageEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		ImageEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~ImageEntity();

		void loadImage();

		void updateGlobal(Box3D& box);

		void show();
		void hide();

	protected:
		Qt3DRender::QTexture2D* m_imageTexture;
	};
}
#endif // QTUSER_3D_BASICENTITY_1594569444448_H