#ifndef QTUSER_3D_XRENDERGRAPH_1679973441799_H
#define QTUSER_3D_XRENDERGRAPH_1679973441799_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/framegraph/rendergraph.h"
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
	class QTUSER_3D_API XRenderGraph : public qtuser_3d::RenderGraph
	{
	public:
		XRenderGraph(Qt3DCore::QNode* parent = nullptr);
		virtual ~XRenderGraph();

		void setClearColor(const QColor& color);
		void setViewport(const QRectF& rect);
		void updateSurfaceSize(const QSize& size);
	protected:
		void updateRenderSize(const QSize& size) override;
		QSize surfaceSize();

	protected:
		Qt3DRender::QRenderSurfaceSelector* m_surfaceSelector;
		QOffscreenSurface* m_offSurface;
		Qt3DRender::QViewport* m_viewPort;
		Qt3DRender::QCameraSelector* m_cameraSelector;
		Qt3DRender::QClearBuffers* m_clearBuffer;

		Qt3DRender::QCamera* m_camera;
	};
}

#endif // QTUSER_3D_XRENDERGRAPH_1679973441799_H