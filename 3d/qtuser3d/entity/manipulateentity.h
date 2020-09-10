#ifndef _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#define _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#include "qtuser3d/entity/pickableentity.h"
#include <QtGui/QVector4D>

namespace qtuser_3d
{
	class QTUSER_3D_API ManipulateEntity : public PickableEntity
	{
	public:
		ManipulateEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~ManipulateEntity();

		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
