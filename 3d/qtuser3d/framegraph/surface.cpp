#include "qtuser3d/framegraph/surface.h"
#include "Qt3DRender/QFilterKey"

namespace qtuser_3d
{
	Surface::Surface(Qt3DCore::QNode* parent)
		:QRenderSurfaceSelector(parent)
	{
		m_offSurface = new QOffscreenSurface();
		m_offSurface->create();
		setSurface(m_offSurface);

		m_viewPort = new Qt3DRender::QViewport(this);
		m_viewPort->setNormalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
		
		m_cameraSelector = new Qt3DRender::QCameraSelector(m_viewPort);

		m_clearBuffer = new Qt3DRender::QClearBuffers(m_cameraSelector);
		m_clearBuffer->setClearColor(QColor(120, 120, 120, 255));
		m_clearBuffer->setBuffers(Qt3DRender::QClearBuffers::BufferType::ColorDepthBuffer);

		m_renderPassFilter = new Qt3DRender::QRenderPassFilter(m_clearBuffer);
		Qt3DRender::QFilterKey* filterKey = new Qt3DRender::QFilterKey();
		filterKey->setName("view");
		filterKey->setValue(0);
		m_renderPassFilter->addMatch(filterKey);

		m_alphaPassFilter = new Qt3DRender::QRenderPassFilter(m_cameraSelector);
		Qt3DRender::QFilterKey* alphaKey = new Qt3DRender::QFilterKey();
		alphaKey->setName("alpha");
		alphaKey->setValue(0);
		m_alphaPassFilter->addMatch(alphaKey);
	}

	Surface::~Surface()
	{
		setSurface(nullptr);
		m_offSurface->deleteLater();
	}

	Qt3DRender::QFrameGraphNode* Surface::getCameraViewportFrameGraphNode()
	{
		return m_cameraSelector;
	}

	void Surface::setClearColor(const QColor& color)
	{
		m_clearBuffer->setClearColor(color);
	}

	void Surface::setViewport(const QRectF& rect)
	{
		m_viewPort->setNormalizedRect(rect);
	}

	void Surface::setCamera(Qt3DRender::QCamera* camera)
	{
		m_cameraSelector->setCamera(camera);
	}

	void Surface::updateSurfaceSize(const QSize& size)
	{
		setExternalRenderTargetSize(size);
	}
}
