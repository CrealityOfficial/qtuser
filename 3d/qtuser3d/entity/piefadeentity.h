#ifndef _QTUSER_3D_PIEFADEENTITY_1590036879870_H
#define _QTUSER_3D_PIEFADEENTITY_1590036879870_H

#include "qtuser3d/entity/pickableentity.h"
#include <QtGui/QVector4D>

namespace qtuser_3d
{
	class QTUSER_3D_API PieFadeEntity : public PickableEntity
	{
	public:
		PieFadeEntity(Qt3DCore::QNode* parent = nullptr, int type = 3);
		virtual ~PieFadeEntity();

		void setColor(const QVector4D& color);
		void setChangeColor(const QVector4D& color);
		void setMethod(int mt);

		// 旋转交互相关参数，非旋转相关交互请保持默认值
		void setRotMode(int mode);
		void setRotRadians(float radians);
		void setRotCenter(QVector3D center);
		void setRotInitDir(QVector3D dir);
		void setRotAxis(QVector3D axis);

	protected:
		Qt3DRender::QParameter* m_colorParameter;
		Qt3DRender::QParameter* m_changeColorParameter;
		Qt3DRender::QParameter* m_methodParameter;

		Qt3DRender::QParameter* m_rotModeParameter;
		Qt3DRender::QParameter* m_rotRadiansParameter;
		Qt3DRender::QParameter* m_rotCenterParameter;
		Qt3DRender::QParameter* m_rotInitDirParameter;
		Qt3DRender::QParameter* m_rotAxisParameter;
	};
}
#endif // _QTUSER_3D_PIEFADEENTITY_1590036879870_H
