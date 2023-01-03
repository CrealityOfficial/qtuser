#ifndef ROTATE_HELPER_ENTITY_T_H
#define ROTATE_HELPER_ENTITY_T_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/event/eventhandlers.h"

namespace qtuser_3d
{
	class ManipulateEntity;
	class PieFadeEntity;
	class Pickable;
	class ManipulatePickable;
	class FacePicker;
	class ScreenCamera;
	class RotateCallback;

	class QTUSER_3D_API RotateHelperEntity_T : public Qt3DCore::QEntity
		,public LeftMouseEventHandler
	{
		Q_OBJECT
	public:
		RotateHelperEntity_T(Qt3DCore::QNode* parent = nullptr);
		virtual ~RotateHelperEntity_T();

		Pickable* getPickable();

		void setRotateAxis(QVector3D axis, double initAngle = 0);
		QVector3D getRotateAxis() { return m_rotateAxis; }
		void setRotateInitAngle(double angle);

		double getLastestRotAngle() { return m_lastestRotAngles; }

		void setPickSource(FacePicker* pickSource);
		void setScreenCamera(ScreenCamera* camera);
		void setRotateCallback(RotateCallback* callback);

		void setVisibility(bool visibility);
		void setHandlerVisibility(bool visibility);
		void setDialVisibility(bool visibility);
		void setColor(QVector4D v4);
		void setRingColor(QVector4D color);
		void setDialColor(QVector4D color);
		void setHandlerColor(QVector4D color);
		void setHandlerPickedColor(QVector4D color);
		void setFixSize(bool fixSize);

		QVector3D center();
		bool isRotating() { return m_rotatingFlag; }

	public slots:
		void onBoxChanged(Box3D box);

	private:
		void initRing();
		void initHandler();
		void initDial();

	protected:
		void onLeftMouseButtonClick(QMouseEvent* event) override {}
		void onLeftMouseButtonPress(QMouseEvent* event) override;
		void onLeftMouseButtonRelease(QMouseEvent* event) override;
		void onLeftMouseButtonMove(QMouseEvent* event) override;

		QVector3D calculateSpacePoint(QPoint point);
		QQuaternion process(QPoint point);
		void perform(QPoint point, bool release);

	protected:
		bool m_rotatingFlag; 
		double m_lastestRotAngles; // < 0: ˳ʱ�룬= 0��0��> 0����ʱ��
		double m_initRotateDirAngles;
		QVector3D m_rotateAxis;
		QVector3D m_originRotateAxis;
		QVector3D m_initRotateDir;
		QVector3D m_originInitRotateDir;
		QQuaternion m_initQuaternion;

		bool m_fixSize;
		QVector3D m_scale;
		QVector3D m_center;
		QVector3D m_spacePoint;

		double m_ringRadius;
		double m_ringMinorRadius;
		QVector4D m_ringColor;
		PieFadeEntity* m_pRingEntity;
		//ManipulatePickable* m_pRingPickable;

		double m_handlerOffset;
		QVector4D m_handlerColor;
		QVector4D m_handlerPickedColor;
		ManipulateEntity* m_pHandlerEntity;
		ManipulatePickable* m_pHandlerPickable;

		double m_dialRadius;
		double m_degreeRadius;
		double m_markOffset;
		QVector4D m_dialColor;
		QVector4D m_degreeColor;
		PieFadeEntity* m_pDialEntity;
		ManipulateEntity* m_pDegreeEntity;
		//ManipulatePickable* m_pDialPickable;

		Qt3DCore::QTransform* m_pGlobalTransform;
		Qt3DCore::QTransform* m_pRotateTransform;
		Qt3DCore::QTransform* m_pNoRotateTransform;
		QEntity* m_pRotateGroup;
		QEntity* m_pNoRotateGroup;

		FacePicker* m_pPickSource;
		ScreenCamera* m_pScreenCamera;
		RotateCallback* m_pRotateCallback;
	};
}

#endif // ROTATE_HELPER_ENTITY_T_H