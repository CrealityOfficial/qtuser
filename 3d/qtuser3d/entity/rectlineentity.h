#ifndef QTUSER_3D_RECTLINEENTITY_H
#define QTUSER_3D_RECTLINEENTITY_H

#include "lineexentity.h"

namespace qtuser_3d
{
	class ScreenCamera;

	class QTUSER_3D_API RectLineEntity : public LineExEntity
	{
		Q_OBJECT
	public:
		RectLineEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~RectLineEntity();

		void setCamera(ScreenCamera* camera);

		void setRectOfScreen(const QRect& rect);

	protected:
		QVector3D makeWorldPositionFromScreen(ScreenCamera* screenCamera, const QPoint& pos);

	private:
		QRect m_rect;
		ScreenCamera* m_camera;
	};
}

#endif //QTUSER_3D_RECTLINEENTITY_H