#include "texturecreator.h"

namespace qtuser_3d
{
	Qt3DRender::QTexture2D* createFromSource(const QUrl& url)
	{
		Qt3DRender::QTextureImage* backgroundImage = new Qt3DRender::QTextureImage();
		backgroundImage->setSource(url);

		Qt3DRender::QTexture2D* imageTexture = new Qt3DRender::QTexture2D();
		imageTexture->addTextureImage(backgroundImage);
		imageTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
		imageTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
		return imageTexture;
	}
}