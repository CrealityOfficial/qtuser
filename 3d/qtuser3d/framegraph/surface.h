#ifndef _QTUSER_3D_SURFACE_1589794908795_H
#define _QTUSER_3D_SURFACE_1589794908795_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QViewport>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QRenderPassFilter>
#include <Qt3DRender/QSortPolicy>
#include <Qt3DRender/QRenderSurfaceSelector>
#include <QtGui/QOffscreenSurface>

namespace qtuser_3d
{
	class QTUSER_3D_API Surface: public Qt3DRender::QRenderSurfaceSelector
	{
	public:
		Surface(Qt3DCore::QNode* parent = nullptr);
		~Surface();

		void setClearColor(const QColor& color);
		void setViewport(const QRectF& rect);
		void setCamera(Qt3DRender::QCamera* camera);

		void updateSurfaceSize(const QSize& size);

		Qt3DRender::QFrameGraphNode* getCameraViewportFrameGraphNode();
	private:
		QOffscreenSurface* m_offSurface;

		Qt3DRender::QViewport* m_viewPort;
		Qt3DRender::QCameraSelector* m_cameraSelector;
		Qt3DRender::QClearBuffers* m_clearBuffer;

		Qt3DRender::QRenderPassFilter* m_renderPassFilter;
		Qt3DRender::QRenderPassFilter* m_alphaPassFilter;
		Qt3DRender::QRenderPassFilter* m_alphaPassFilter2;
	};
}
#endif // _QTUSER_3D_SURFACE_1589794908795_H
