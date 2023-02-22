#ifndef _QTUSER_3D_XYZINDICATORENTITY_1591765873312_H
#define _QTUSER_3D_XYZINDICATORENTITY_1591765873312_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API XyzIndicatorEntity: public BasicEntity
	{
	public:
		XyzIndicatorEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~XyzIndicatorEntity();

		void setColor(QVector4D color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // _QTUSER_3D_XYZINDICATORENTITY_1591765873312_H
