#ifndef QTUSER_3D_TRIANGLEENTITY_1603033844646_H
#define QTUSER_3D_TRIANGLEENTITY_1603033844646_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/basicentity.h"
#include <Qt3DRender/QTexture>

namespace qtuser_3d
{
	class QTUSER_3D_API TriangleEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		TriangleEntity();
		~TriangleEntity();

		void updateGeometry(int pointsNum, float* positions, float* normals, int triangleNum, int* indices);
		void updateGeometry(int pointsNum, float* positions, float* normals);
		void updateGeometry(int pointsNum, float* positions, float* normals, float* uv);

		void setTexture(Qt3DRender::QTexture2D* texture);
	protected:
		Qt3DRender::QParameter* m_texParameter;
	};
}

#endif // QTUSER_3D_TRIANGLEENTITY_1603033844646_H