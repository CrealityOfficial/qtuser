#ifndef _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#define _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
#include "qtuser3d/refactor/pxentity.h"
#include <QtGui/QVector4D>

namespace qtuser_3d
{
	class QTUSER_3D_API ManipulateEntity : public PickXEntity
	{
	public:
		ManipulateEntity(Qt3DCore::QNode* parent = nullptr, bool lightEnable = false , bool alpha = true, bool pickable = true, bool depthTest = false, bool overlay = false);
		virtual ~ManipulateEntity();

		void setColor(const QVector4D& color);
		void setTriggeredColor(const QVector4D& color);
		void setTriggerible(bool enable);

		virtual void setVisualState(qtuser_3d::ControlState state) override;

	protected:
		QVector4D m_color;
		QVector4D m_triggeredColor;
		bool m_isTriggerible;

	};
}
#endif // _QTUSER_3D_MANIPULATEENTITY_1590036879870_H
