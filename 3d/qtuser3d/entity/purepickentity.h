#ifndef QTUSER_3D_PUREPICKENTITY_1689588292644_H
#define QTUSER_3D_PUREPICKENTITY_1689588292644_H
#include "qtuser3d/refactor/pxentity.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PurePickEntity : public PickXEntity
	{
		Q_OBJECT
	public:
		PurePickEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PurePickEntity();

		void setColor(const QVector4D& color);
		void setHoverColor(const QVector4D& color);
	protected:
		void onStateChanged(ControlState state) override;
	protected:
		QVector4D m_color;
		QVector4D m_hoverColor;

		Qt3DRender::QParameter* m_colorParameter;
	};
}

#endif // QTUSER_3D_PUREPICKENTITY_1689588292644_H