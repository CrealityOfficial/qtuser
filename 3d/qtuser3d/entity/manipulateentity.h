#ifndef _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#define _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#include "qtuser3d/refactor/pxentity.h"
#include <QtGui/QVector4D>

namespace qtuser_3d
{
	class QTUSER_3D_API ManipulateEntity : public PickXEntity
	{
	public:
		ManipulateEntity(Qt3DCore::QNode* parent = nullptr, bool alpha = true, bool pickable = true, bool depthTest = false, bool overlay = false);
		virtual ~ManipulateEntity();

		void setColor(const QVector4D& color);
		void setChangeColor(const QVector4D& color);
		void setMethod(int mt);
		void setLightEnable(bool flag);

	protected:
		Qt3DRender::QParameter* m_colorParameter;
		Qt3DRender::QParameter* m_changeColorParameter;
		Qt3DRender::QParameter* m_methodParameter;
		Qt3DRender::QParameter* m_lightEnableParameter;
	};
}
#endif // _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
