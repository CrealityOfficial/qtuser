#ifndef _QTUSER_3D_POINTCLOUDENTITY_1591754596129_H
#define _QTUSER_3D_POINTCLOUDENTITY_1591754596129_H
#include "qtuser3d/entity/basicentity.h"
#include <QtGui/QVector3D>
#include <Qt3DRender/QPointSize>

namespace qtuser_3d
{
	class QTUSER_3D_API PointCloudEntity: public BasicEntity
	{
	public:
		PointCloudEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PointCloudEntity();

		void setColor(const QVector4D& color);
		void setPointSize(float size);
	protected:
		void selectEffect();
	protected:
		Qt3DRender::QParameter* m_colorParameter;
		float m_pointSize;

		Qt3DRender::QPointSize* m_pointSizeState;
		Qt3DRender::QEffect* m_usedEffect;
	};
}
#endif // _QTUSER_3D_POINTCLOUDENTITY_1591754596129_H
