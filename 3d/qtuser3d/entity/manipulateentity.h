#ifndef _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#define _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#include "qtuser3d/entity/pickableentity.h"
#include <QtGui/QVector4D>

namespace qtuser_3d
{
	class QTUSER_3D_API ManipulateEntity : public PickableEntity
	{
	public:
		ManipulateEntity(Qt3DCore::QNode* parent = nullptr, int type = 3);
		virtual ~ManipulateEntity();

		void setColor(const QVector4D& color);
		void setChangeColor(const QVector4D& color);
		void setMethod(int mt);

	protected:
		Qt3DRender::QParameter* m_colorParameter;
		Qt3DRender::QParameter* m_changeColorParameter;
		Qt3DRender::QParameter* m_methodParameter;
	};
}
#endif // _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
