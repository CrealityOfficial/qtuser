#ifndef _QTUSER_3D_PUREPICKENTITY_1591765873312_H
#define _QTUSER_3D_PUREPICKENTITY_1591765873312_H
#include "qtuser3d/entity/pickentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PurePickEntity: public PickEntity
	{
	public:
		PurePickEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PurePickEntity();

		void setColor(const QVector4D& color);
		void setHoverColor(const QVector4D& color);

	protected:
		void updateColor();
		void onStateChanged(ControlState state);
	protected:
		QVector4D m_hoverColor;
		QVector4D m_color;

		Qt3DRender::QParameter* m_colorParameter;
	};
}
#endif // _QTUSER_3D_PUREPICKENTITY_1591765873312_H
