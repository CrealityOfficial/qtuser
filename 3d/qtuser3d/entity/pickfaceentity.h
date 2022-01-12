#ifndef _QTUSER_3D_PICKFACEENTITY_1639375311688_H
#define _QTUSER_3D_PICKFACEENTITY_1639375311688_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/module/pickable.h"
#include <QtGui/QVector3D>
#include <vector>

namespace qtuser_3d
{
	class QTUSER_3D_API PickFaceEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		PickFaceEntity(Qt3DCore::QNode* parent);
		virtual ~PickFaceEntity();

		void updateData(const std::vector<QVector3D>& vertexData);
		void setColor(const QVector4D& color);
		void setState(float state);
		void setVertexBase(QPoint vertexBase);
	private:
		Qt3DRender::QParameter* m_colorParameter;
		Qt3DRender::QParameter* m_stateParameter;
		Qt3DRender::QParameter* m_vertexBaseParameter;
		Qt3DRender::QGeometry*	m_geometry;
	};
}
#endif // _QTUSER_3D_PICKFACEENTITY_1639375311688_H
