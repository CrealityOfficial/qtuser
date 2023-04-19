#ifndef _QTUSER_3D_COLORPICKER_1589166630506_H
#define _QTUSER_3D_COLORPICKER_1589166630506_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QRenderTargetSelector>
#include <Qt3DRender/QRenderCapture>
#include <Qt3DRender/QClearBuffers>
#include <Qt3DRender/QFilterKey>
#include <Qt3DRender/QRenderPassFilter>
#include <Qt3DRender/QCameraSelector>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QTexture>

#include "qtuser3d/module/facepicker.h"
#include <QtCore/QTimer>

namespace qtuser_3d
{
	class ColorPicker;
	typedef std::function<void(QImage& image)> requestCallFunc;

	class TextureRenderTarget;
	class QTUSER_3D_API ColorPicker : public Qt3DRender::QFrameGraphNode
		, public FacePicker
	{
		Q_OBJECT
	signals:
		void frameCaptureComplete();
	public:
		ColorPicker(Qt3DCore::QNode* parent = nullptr);
		virtual ~ColorPicker();

		void resize(const QSize& size);

		void requestCapture();
		void setFilterKey(const QString& name, int value);
		void setClearColor(const QColor& color);

		bool pick(int x, int y, int* faceID) override;
		bool pick(const QPoint& point, int* faceID) override;
		void sourceMayChanged() override;
		void setRequestCallback(requestCallFunc func);

		void useSelfCameraSelector(bool use);
		Qt3DRender::QCamera* camera();

		void setDebugName(const QString& name);

		Qt3DRender::QTexture2D* colorTexture();
		void setTextureRenderTarget(TextureRenderTarget* textureRenderTarget);

		void use();
		void unUse();

	public slots:
		void captureCompleted();
	protected:
		Qt3DRender::QClearBuffers* m_clearBuffer;
		Qt3DRender::QRenderTargetSelector* m_renderTargetSelector;
		Qt3DRender::QRenderCapture* m_renderCapture;
		Qt3DRender::QRenderPassFilter* m_renderPassFilter;
		Qt3DRender::QFilterKey* m_filterKey;
		Qt3DRender::QCameraSelector* m_cameraSelector;
		Qt3DRender::QCamera* m_camera;

		TextureRenderTarget* m_textureRenderTarget;

		QScopedPointer<Qt3DRender::QRenderCaptureReply> m_captureReply;
		QImage m_colorPickerImage;

		bool m_capturing;

		requestCallFunc m_requestCallback;
#ifdef _DEBUG
		QString m_debugName;
	public:
#endif
	};
}
#endif // _QTUSER_3D_COLORPICKER_1589166630506_H
