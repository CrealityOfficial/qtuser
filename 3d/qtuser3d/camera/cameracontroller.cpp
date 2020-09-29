#include "qtuser3d/camera/cameracontroller.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/camera/trackballcameramanipulator.h"
#include "qtuser3d/camera/eularmousemanipulator.h"
#include "qmath.h"
#include <qdebug.h>

#define USE_EULAR_MANIPULATOR

CameraController::CameraController(QObject* parent)
	:QObject(parent)
	, m_screenCamera(nullptr)
	, m_cameraManipulator(nullptr)
	, m_isOrtho(false)
{
#ifdef USE_EULAR_MANIPULATOR
	m_cameraManipulator = new qtuser_3d::EularMouseManipulator(this);
#else
	m_cameraManipulator = new qtuser_3d::TrackballCameraManipulator(this);
#endif

}

CameraController::~CameraController()
{
}

void CameraController::setScreenCamera(qtuser_3d::ScreenCamera* camera)
{
	m_screenCamera = camera;
	Qt3DRender::QCamera* pickerCamera = m_screenCamera->camera();
	m_cameraManipulator->setCamera(m_screenCamera);
}

void CameraController::viewFromOrthographic()
{
	float per = 1;
	Qt3DRender::QCamera* pickerCamera = m_screenCamera->camera();
	float fnearPlane = -10;  // pickerCamera->nearPlane();
	float ffarPlane = pickerCamera->farPlane();
	float left = pickerCamera->left() / per;
	float right = pickerCamera->right() / per;
	float bottom = pickerCamera->bottom() / per;
	float top = pickerCamera->top() / per;

	float r = (ffarPlane - fnearPlane) / 2 + fnearPlane;
	float flen = r * tan(pickerCamera->fieldOfView() *M_PI / 360);
	top = flen;
	bottom = -flen;

	float fratio = pickerCamera->aspectRatio();
	left = -fratio* flen;
	right = fratio * flen;

	m_cameraPos = pickerCamera->position();

	QVector3D cameraPosition = pickerCamera->position();
	QVector3D cameraCenter = pickerCamera->viewCenter();
	QVector3D cameraView = cameraCenter - cameraPosition;
	cameraView.normalize();
	QVector3D newPosition = cameraCenter - cameraView * 500;
	pickerCamera->setPosition(newPosition);
	
	pickerCamera->setProjectionType(Qt3DRender::QCameraLens::ProjectionType::OrthographicProjection);
	//QMatrix4x4
	QMatrix4x4 M = {2/(right- left),0,0,-(right+left)/(right-left),
	0,2/(top- bottom),0,-(top+bottom)/(top-bottom),
	0,0,-2/(ffarPlane- fnearPlane),-(ffarPlane+fnearPlane)/(ffarPlane-fnearPlane),
	0,0,0,1};

	pickerCamera->setProjectionMatrix(M);
	m_isOrtho = true;
	emit signalViewChanged(false);
}

void CameraController::viewFromPerspective()
{
	Qt3DRender::QCamera* pickerCamera = m_screenCamera->camera();
	float fnearPlane = pickerCamera->nearPlane();
	float ffarPlane = pickerCamera->farPlane();
	float left = pickerCamera->left();
	float right = pickerCamera->right();
	float bottom = pickerCamera->bottom();
	float top = pickerCamera->top();

	if (m_isOrtho)
	{
		pickerCamera->setPosition(m_cameraPos);
	}
	m_isOrtho = false;
	
	pickerCamera->setProjectionType(Qt3DRender::QCameraLens::ProjectionType::PerspectiveProjection);

	QMatrix4x4 M = { 2 * fnearPlane / (right - left), 0,(right + left) / (right - left),0,
					0, 2 * fnearPlane / (top - bottom),(top + bottom) / (top - bottom),0,
					0,0,-(ffarPlane + fnearPlane) / (ffarPlane - fnearPlane),-2 * (ffarPlane * fnearPlane) / (ffarPlane - fnearPlane),
					0,0,-1,0 };
	
	emit signalViewChanged(false);
}

void CameraController::showSelf()
{
	qDebug() << "show CameraController:";
	m_screenCamera->showSelf();
}

void CameraController::uninitialize()
{
}

void CameraController::fittingBoundingBox(const qtuser_3d::Box3D& box)
{
	if(m_screenCamera)
		m_screenCamera->fittingBoundingBox(box);
}

void CameraController::home(const qtuser_3d::Box3D& box)
{
	if(m_screenCamera)
		m_screenCamera->home(box);
}

void CameraController::onResize(const QSize& size)
{
	if(m_screenCamera)
		m_screenCamera->setScreenSize(size);
}


void CameraController::onRightMouseButtonPress(QMouseEvent* event)
{
	m_cameraManipulator->onRightMouseButtonPress(event);
}

void CameraController::onRightMouseButtonMove(QMouseEvent* event)
{
	m_cameraManipulator->onRightMouseButtonMove(event);
	emit signalViewChanged(false);
}

void CameraController::onRightMouseButtonRelease(QMouseEvent* event)
{
	emit signalViewChanged(true);
}

void CameraController::onRightMouseButtonClick(QMouseEvent* event)
{
	(void)(event);
}


void CameraController::onMidMouseButtonPress(QMouseEvent* event)
{
	m_cameraManipulator->onMidMouseButtonPress(event);
}

void CameraController::onMidMouseButtonMove(QMouseEvent* event)
{
	m_cameraManipulator->onMidMouseButtonMove(event);
	emit signalViewChanged(false);
}

void CameraController::onMidMouseButtonRelease(QMouseEvent* event)
{
	emit signalViewChanged(true);
}

void CameraController::onMidMouseButtonClick(QMouseEvent* event)
{
	(void)(event);
}


void CameraController::onWheelEvent(QWheelEvent* event)
{
	if (!m_isOrtho && m_screenCamera && m_screenCamera->zoom(event->delta() > 0 ? 1.0f / 1.1f : 1.1f))
	{
		emit signalViewChanged(true);
	}
}

void CameraController::viewFromBottom()
{
	QVector3D dir(0.0f, 0.0f, 1.0f);
	QVector3D right(1.0f, 0.0f, 0.0f);
	view(dir, right);
}

void CameraController::viewFromTop()
{
	QVector3D dir(0.0f, 0.0f, -1.0f);
	QVector3D right(1.0f, 0.0f, 0.0f);
	view(dir, right);
}

void CameraController::viewFromLeft()
{
	QVector3D dir(1.0f, 0.0f, 0.0f);
	QVector3D right(0.0f, -1.0f, 0.0f);
	view(dir, right);
}

void CameraController::viewFromRight()
{
	QVector3D dir(-1.0f, 0.0f, 0.0f);
	QVector3D right(0.0f, 1.0f, 0.0f);
	view(dir, right);
}

void CameraController::viewFromFront()
{
	QVector3D dir(0.0f, 1.0f, 0.0f);
	QVector3D right(1.0f, 0.0f, 0.0f);
	view(dir, right);
}

void CameraController::viewFromBack()
{
	QVector3D dir(0.0f, -1.0f, 0.0f);
	QVector3D right(-1.0f, 0.0f, 0.0f);
	view(dir, right);
}

void CameraController::view(const QVector3D& dir, const QVector3D& right)
{
	if (m_screenCamera)
	{
		m_screenCamera->viewFrom(dir, right);
		emit signalViewChanged(true);
	}
}
