#ifndef ROTATE_HELPER_ENTITY_H
#define ROTATE_HELPER_ENTITY_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/event/eventhandlers.h"

namespace qtuser_3d
{
	class ManipulateEntity;
	class Pickable;
	class ManipulatePickable;
	class FacePicker;
	class ScreenCamera;
	class RotateCallback;
	class QTUSER_3D_API RotateHelperEntity : public Qt3DCore::QEntity
		,public LeftMouseEventHandler, public HoverEventHandler
	{
		enum class RotateMode
		{
			null,
			x,
			y,
			z,
		};

		Q_OBJECT
	public:
		RotateHelperEntity(Qt3DCore::QNode* parent = nullptr, int type = 0);
		virtual ~RotateHelperEntity();

		Pickable* xPickable();
		Pickable* yPickable();
		Pickable* zPickable();
		QList<Pickable*> pickables();

		void setPickSource(FacePicker* pickSource);
		void setScreenCamera(ScreenCamera* camera);
		void setRotateCallback(RotateCallback* callback);

		void setXVisibility(bool visibility);
		void setYVisibility(bool visibility);
		void setZVisibility(bool visibility);
		QVector3D center();
		void setFixSize(bool fixSize);
	public slots:
		void onBoxChanged(Box3D box);

	protected:
		void onLeftMouseButtonClick(QMouseEvent* event) override {}
		void onLeftMouseButtonPress(QMouseEvent* event) override;
		void onLeftMouseButtonRelease(QMouseEvent* event) override;
		void onLeftMouseButtonMove(QMouseEvent* event) override;
		void onHoverEnter(QHoverEvent* event) override {}
		void onHoverLeave(QHoverEvent* event) override {}
		void onHoverMove(QHoverEvent* event) override;

		QVector3D calculateSpacePoint(QPoint point);
		QQuaternion process(QPoint point);
		void perform(QPoint point, bool release);
	protected:
		ManipulateEntity* m_xRingEntity;
		ManipulateEntity* m_yRingEntity;
		ManipulateEntity* m_zRingEntity;

		ManipulatePickable* m_xPickable;
		ManipulatePickable* m_yPickable;
		ManipulatePickable* m_zPickable;
		Qt3DCore::QTransform* m_transform;

		FacePicker* m_pickSource;
		ScreenCamera* m_screenCamera;
		QVector3D m_center;
		RotateCallback* m_rotateCallback;

		RotateMode m_rotateMode;
		QVector3D m_spacePoint;

		bool m_fixSize;
	};
}

#endif // ROTATE_HELPER_ENTITY_H