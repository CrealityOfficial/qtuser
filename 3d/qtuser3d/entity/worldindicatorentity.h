#ifndef QTUSER_3D_WORLD_INDICATOR_H
#define QTUSER_3D_WORLD_INDICATOR_H

#include <Qt3DCore/QEntity>
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/module/indicatorpickable.h"
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/camera/screencamera.h"

class CameraController;

namespace qtuser_3d
{
	class QTUSER_3D_API WorldIndicatorEntity : public PickableEntity, public ScreenCameraObserver
	{
		Q_OBJECT

	public:
		WorldIndicatorEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~WorldIndicatorEntity();

		Pickable* pickable() const;

		void setCameraController(CameraController* cc);

		void setHighlightDirections(int dirs);

		void setSelectedDirections(int dirs);
		void setScreenPos(float x, float y);
	protected:
		void onCameraChanged(ScreenCamera* camera) override;
		void setViewport(float x, float y, float w, float h);

	private:
		void setupGeometry();
		void setupTexture();

		void adaptCamera(int dirs);
		void adaptLocal(int dirs);

	private:
		IndicatorPickable* m_pickable;
		CameraController* m_cameraController;
	};
}

#endif //QTUSER_3D_WORLD_INDICATOR_H