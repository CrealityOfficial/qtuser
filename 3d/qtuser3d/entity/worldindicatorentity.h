#ifndef QTUSER_3D_WORLD_INDICATOR_H
#define QTUSER_3D_WORLD_INDICATOR_H

#include <Qt3DCore/QEntity>
#include <QPropertyAnimation>

#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/module/indicatorpickable.h"
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/camera/screencamera.h"

#include <QUrl>

class CameraController;

namespace qtuser_3d
{
	class QTUSER_3D_API WorldIndicatorEntity : public PickableEntity, public ScreenCameraObserver
	{
		Q_OBJECT
			Q_PROPERTY(float lambda READ lambda WRITE setLambda NOTIFY lambdaChanged)
	public:
		WorldIndicatorEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~WorldIndicatorEntity();

		Pickable* pickable() const;

		void setCameraController(CameraController* cc);

		void setHighlightDirections(int dirs);

		void setSelectedDirections(int dirs);

		void setScreenPos(const QPoint& p);

		void setTheme(int theme);
		void setupLightTexture(const QUrl& url);
		void setupDarkTexture(const QUrl& url);
		void setupSelectTexture(const QUrl& url);

	signals:
		void lambdaChanged();

	protected:
		void onCameraChanged(ScreenCamera* camera) override;
		void setViewport(float x, float y, float w, float h);

	public Q_SLOTS:
		void aspectRatioChanged(float aspectRatio);

	private:
		void setupGeometry();

		void adaptCamera(int dirs);

		void setLambda(float lambda);
		float lambda() const;

		void updateBasicTexture();

	private:
		IndicatorPickable* m_pickable;
		CameraController* m_cameraController;
		
		// for animate
		QSharedPointer<QPropertyAnimation> m_animation;
		float m_lambda;
		QVector3D m_startDir;
		QVector3D m_startUp;
		QVector3D m_endDir;
		QVector3D m_endUp;

		QPoint m_showOnPoint;

		int m_theme;
		QUrl m_lightTextureUrl;
		QUrl m_darkTextureUrl;
	};
}

#endif //QTUSER_3D_WORLD_INDICATOR_H