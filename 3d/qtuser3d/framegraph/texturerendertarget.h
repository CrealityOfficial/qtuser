#ifndef _QTUSER_3D_TEXTURERENDERTARGET_1589166630506_H
#define _QTUSER_3D_TEXTURERENDERTARGET_1589166630506_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QRenderTarget>
#include <Qt3DRender/QRenderTargetOutput>
#include <Qt3DRender/QTexture>

namespace qtuser_3d
{
	class QTUSER_3D_API TextureRenderTarget: public Qt3DRender::QRenderTarget
	{
	public:
        TextureRenderTarget(Qt3DCore::QNode* parent = nullptr, const QSize& size = QSize(500, 500));
        virtual ~TextureRenderTarget();

        void resize(const QSize& size);
		Qt3DRender::QTexture2D* colorTexture();
    private:
        QSize m_size;
        Qt3DRender::QRenderTargetOutput* m_colorOutput;
        Qt3DRender::QTexture2D* m_colorTexture;
        // To enable depth testing
        Qt3DRender::QRenderTargetOutput* m_depthOutput;
        Qt3DRender::QTexture2D* m_depthTexture;
	};
}
#endif // _QTUSER_3D_TEXTURERENDERTARGET_1589166630506_H
