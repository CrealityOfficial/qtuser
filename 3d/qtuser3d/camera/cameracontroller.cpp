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
	, m_mask(7)
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

void CameraController::setNeed360Rotate(bool is_need)
{
	m_cameraManipulator->setNeed360Rotate(is_need);
}

void CameraController::setScreenCamera(qtuser_3d::ScreenCamera* camera)
{
	m_screenCamera = camera;
	//Qt3DRender::QCamera* pickerCamera = m_screenCamera->camera();
	m_cameraManipulator->setCamera(m_screenCamera);
}

qtuser_3d::ScreenCamera* CameraController::screenCamera() 
{
	return m_screenCamera;
}

void CameraController::viewFromOrthographic()
{
	Qt3DRender::QCamera* pickerCamera = m_screenCamera->camera();
	if (pickerCamera->projectionType() == Qt3DRender::QCameraLens::OrthographicProjection)
		return;

	m_cameraPos = pickerCamera->position();
	QMatrix4x4 m_viewMatrix = pickerCamera->projectionMatrix();

	float fnearPlane = pickerCamera->nearPlane();
	float ffarPlane = pickerCamera->farPlane();

	float r = (ffarPlane - fnearPlane) / 2 + fnearPlane;
	float flen = r * tan(pickerCamera->fieldOfView() *M_PI / 360);
	float top = flen;
	float bottom = -flen;

	float fratio = pickerCamera->aspectRatio();
	float left = -fratio* flen;
	float right = fratio * flen;

	pickerCamera->setTop(top);
	pickerCamera->setBottom(bottom);
	pickerCamera->setLeft(left);
	pickerCamera->setRight(right);

	pickerCamera->setProjectionType(Qt3DRender::QCameraLens::ProjectionType::OrthographicProjection);
	emit signalViewChanged(false);
}

void CameraController::viewFromPerspective()
{
	Qt3DRender::QCamera* pickerCamera = m_screenCamera->camera();
	if (pickerCamera->projectionType() == Qt3DRender::QCameraLens::PerspectiveProjection)
		return;

	pickerCamera->setProjectionType(Qt3DRender::QCameraLens::ProjectionType::PerspectiveProjection);
	emit signalViewChanged(false);
}

void CameraController::showSelf()
{
	qDebug() << "show CameraController:";
	m_screenCamera->showSelf();
}

void CameraController::sendViewUpdateSingle(bool capture)
{
	emit signalViewChanged(capture);
}

void CameraController::uninitialize()
{
}

void CameraController::fittingBoundingBox(const qtuser_3d::Box3D& box)
{
	if(m_screenCamera)
		m_screenCamera->fittingBoundingBox(box);
}

void CameraController::updateNearFar(const qtuser_3d::Box3D& box)
{
	if (m_screenCamera)
		m_screenCamera->updateNearFar(box);
}

void CameraController::home(const qtuser_3d::Box3D& box, int type)
{
	if(m_screenCamera)
		m_screenCamera->home(box, type);
}

void CameraController::onResize(const QSize& size)
{
	if(m_screenCamera)
		m_screenCamera->setScreenSize(size);
}

QVector3D CameraController::getViewPosition() const
{
	return m_screenCamera->camera()->position();
}
QVector3D CameraController::getViewupVector() const
{
	return m_screenCamera->camera()->upVector();
}
QVector3D CameraController::getviewCenter() const
{
	return m_screenCamera->camera()->viewCenter();
}

QVector3D CameraController::getViewDir() const
{
	QVector3D v = -m_screenCamera->camera()->viewVector();
	return v.normalized();
}

void CameraController::setViewPosition(const QVector3D position)
{
	m_screenCamera->camera()->setPosition(position);
}
void CameraController::setViewupVector(const QVector3D upVector)
{
	m_screenCamera->camera()->setUpVector(upVector);
}
void CameraController::setviewCenter(const QVector3D viewCenter)
{
	m_screenCamera->camera()->setViewCenter(viewCenter);
}

void CameraController::onRightMouseButtonPress(QMouseEvent* event)
{
	if (!(m_mask & 1))
		return;

	m_cameraManipulator->onRightMouseButtonPress(event);
}

void CameraController::onRightMouseButtonMove(QMouseEvent* event)
{
	if (!(m_mask & 1))
		return;

	m_cameraManipulator->onRightMouseButtonMove(event);
	emit signalViewChanged(false);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position-center, getViewupVector());
}

void CameraController::onRightMouseButtonRelease(QMouseEvent* event)
{
	if (!(m_mask & 1))
		return;

	emit signalViewChanged(true);
}

void CameraController::onRightMouseButtonClick(QMouseEvent* event)
{
	(void)(event);
}


void CameraController::onMidMouseButtonPress(QMouseEvent* event)
{
	if (!(m_mask & 4))
		return;

	m_cameraManipulator->onMidMouseButtonPress(event);
}

void CameraController::onMidMouseButtonMove(QMouseEvent* event)
{
	if (!(m_mask & 4))
		return;

	m_cameraManipulator->onMidMouseButtonMove(event);
	emit signalViewChanged(false);
}

void CameraController::onMidMouseButtonRelease(QMouseEvent* event)
{
	if (!(m_mask & 4))
		return;

	emit signalViewChanged(true);
}

void CameraController::onMidMouseButtonClick(QMouseEvent* event)
{
	(void)(event);
}

void CameraController::enableRotate(bool enabled)
{
	if (enabled)
		m_mask |= 1;
	else
		m_mask &= ~1;
}

void CameraController::enableScale(bool enabled)
{
	if (enabled)
		m_mask |= 2;
	else
		m_mask &= ~2;
}

void CameraController::enableTranslate(bool enabled)
{
	if (enabled)
		m_mask |= 4;
	else
		m_mask &= ~4;
}

void CameraController::onWheelEvent(QWheelEvent* event)
{
	if (!(m_mask & 2))
		return;

	if (m_screenCamera && m_screenCamera->zoom(event->delta() > 0 ? 1.0f / 1.1f : 1.1f))
	{
		emit signalViewChanged(true);
	}
}

void CameraController::viewFromBottom(QVector3D* specificCenter)
{
	QVector3D dir(0.0f, 0.0f, 1.0f);
	QVector3D right(1.0f, 0.0f, 0.0f);
	view(dir, right, specificCenter);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position - center, getViewupVector());
}

void CameraController::viewFromTop(QVector3D* specificCenter)
{
	QVector3D dir(0.0f, 0.0f, -1.0f);
	QVector3D right(1.0f, 0.0f, 0.0f);
	view(dir, right, specificCenter);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position - center, getViewupVector());
}

void CameraController::viewFromLeft(QVector3D* specificCenter)
{
	QVector3D dir(1.0f, 0.0f, 0.0f);
	QVector3D right(0.0f, -1.0f, 0.0f);
	view(dir, right, specificCenter);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position - center, getViewupVector());
}

void CameraController::viewFromRight(QVector3D* specificCenter)
{
	QVector3D dir(-1.0f, 0.0f, 0.0f);
	QVector3D right(0.0f, 1.0f, 0.0f);
	view(dir, right, specificCenter);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position - center, getViewupVector());
}

void CameraController::viewFromFront(QVector3D* specificCenter)
{
	QVector3D dir(0.0f, 1.0f, 0.0f);
	QVector3D right(1.0f, 0.0f, 0.0f);
	view(dir, right, specificCenter);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position - center, getViewupVector());
}

void CameraController::viewFromBack(QVector3D* specificCenter)
{
	QVector3D dir(0.0f, -1.0f, 0.0f);
	QVector3D right(-1.0f, 0.0f, 0.0f);
	view(dir, right, specificCenter);

	QVector3D position = getViewPosition();
	QVector3D upview = getViewupVector();
	QVector3D center = getviewCenter();
	QVector3D diff = position - center;
	emit signalCameraChaged(position - center, getViewupVector());
}

void CameraController::viewFromUserSetting(QVector3D posion, QVector3D viewCenter, QVector3D upVector, QVector3D* specificCenter)
{
	QVector3D dir = viewCenter - posion;
	QVector3D right = QVector3D::crossProduct(dir, upVector);
	view(dir, right, specificCenter);
}

void CameraController::view(const QVector3D& dir, const QVector3D& right, QVector3D* specificCenter)
{
	if (m_screenCamera)
	{
		m_screenCamera->viewFrom(dir, right, specificCenter);
		emit signalViewChanged(true);
	}
}
