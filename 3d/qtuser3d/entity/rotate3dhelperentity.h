#ifndef ROTATE3D_HELPER_ENTITY_H
#define ROTATE3D_HELPER_ENTITY_H

#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>

#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/event/eventhandlers.h"
#include "qtuser3d/module/manipulatecallback.h"


namespace qtuser_3d
{
	class RotateHelperEntity_T;
	class Pickable;
	class FacePicker;
	class ScreenCamera;
	class RotateCallback;
	class QTUSER_3D_API Rotate3DHelperEntity : public Qt3DCore::QEntity,
		public RotateCallback
	{
		Q_OBJECT
	public:
		Rotate3DHelperEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~Rotate3DHelperEntity();

		inline QList<Pickable*> getPickables();
		inline QList<LeftMouseEventHandler*> getLeftMouseEventHandlers();

		inline void setPickSource(FacePicker* pickSource);
		inline void setScreenCamera(ScreenCamera* camera);
		inline void setRotateCallback(RotateCallback* callback);

		inline void setXVisibility(bool visibility);
		inline void setYVisibility(bool visibility);
		inline void setZVisibility(bool visibility);
		
	public slots:
		void onBoxChanged(Box3D box);

	protected:
		void onStartRotate() override;
		void onRotate(QQuaternion q) override;
		void onEndRotate(QQuaternion q) override;
		void setRotateAngle(QVector3D axis, float angle) override;

	private:
		RotateHelperEntity_T* m_pXRotHelper;
		RotateHelperEntity_T* m_pYRotHelper;
		RotateHelperEntity_T* m_pZRotHelper;

		RotateCallback* m_pRotateCallback;
	};
}

#endif // ROTATE3D_HELPER_ENTITY_H