#include "qtuser3d/framegraph/colorpicker.h"
#include "qtuser3d/framegraph/texturerendertarget.h"
#include <QtWidgets/QApplication>
#include <QtCore/QDebug>
namespace qtuser_3d
{
	ColorPicker::ColorPicker(Qt3DCore::QNode* parent)
		:QFrameGraphNode(parent)
		, m_textureRenderTarget(nullptr)
		, m_capturing(false)
	{
#ifdef _DEBUG
		m_debugName = QString("%1/pickTest.bmp").arg(QCoreApplication::applicationDirPath());
		qDebug() << QString("ColorPicker::ColorPicker %1").arg(m_debugName);
#endif
	
		m_clearBuffer = new Qt3DRender::QClearBuffers(this);
		m_clearBuffer->setClearColor(QColor(255, 255, 255, 255));
		m_clearBuffer->setBuffers(Qt3DRender::QClearBuffers::BufferType::ColorDepthBuffer);
		m_renderTargetSelector = new Qt3DRender::QRenderTargetSelector(m_clearBuffer);
		m_renderCapture = new Qt3DRender::QRenderCapture(m_renderTargetSelector);
		m_renderPassFilter = new Qt3DRender::QRenderPassFilter(m_renderCapture);
		m_filterKey = new Qt3DRender::QFilterKey(m_renderPassFilter);
		m_filterKey->setName("pick");
		m_filterKey->setValue(0);
		m_renderPassFilter->addMatch(m_filterKey);

		m_cameraSelector = new Qt3DRender::QCameraSelector();
		m_camera = new Qt3DRender::QCamera(m_cameraSelector);
		m_cameraSelector->setCamera(m_camera);

		//m_textureRenderTarget = new TextureRenderTarget();
		//m_renderTargetSelector->setTarget(m_textureRenderTarget);
	}

	ColorPicker::~ColorPicker()
	{
		if (!m_cameraSelector->parent())
		{
			delete m_cameraSelector;
			m_cameraSelector = nullptr;
		}
	}

	void ColorPicker::resize(const QSize& size)
	{
		qDebug() << "ColorPicker::resize " << size;
		m_colorPickerImage = QImage(size, QImage::Format_RGBA8888);
		m_colorPickerImage.fill(0xFFFFFFFF);
	}

	void ColorPicker::requestCapture()
	{
		m_capturing = true;
		m_captureReply.reset(m_renderCapture->requestCapture());
		connect(m_captureReply.data(), &Qt3DRender::QRenderCaptureReply::completed, this, &ColorPicker::captureCompleted);
	}

	void ColorPicker::captureCompleted()
	{
#if 0
		qDebug() << "ColorPicker::captureCompleted";
#endif
		m_colorPickerImage = m_captureReply->image();
        m_captureReply.reset();
		if (m_requestCallback)
			m_requestCallback(m_colorPickerImage);

#ifdef _DEBUG
        m_colorPickerImage.save(m_debugName);
#endif
		m_capturing = false;
	}

	void ColorPicker::setFilterKey(const QString& name, int value)
	{
		qDebug() << "ColorPicker::setFilterKey " << name << " = " << value;
		m_filterKey->setName(name);
		m_filterKey->setValue(value);
	}

	void ColorPicker::sourceMayChanged()
	{
	}

	void ColorPicker::setClearColor(const QColor& color)
	{
		qDebug() << "ColorPicker::setClearColor " << color;
		m_clearBuffer->setClearColor(color);
	}

	bool ColorPicker::pick(const QPoint& point, int* faceID)
	{
		return pick(point.x(), point.y(), faceID);
	}

	bool ColorPicker::pick(int x, int y, int* faceID)
	{
		if (y >= m_colorPickerImage.height())
			return false;

		QRgb pixelval = m_colorPickerImage.pixel(x, y);

		if (pixelval == 0xFFFFFFFF)
			return false;

		unsigned r = qRed(pixelval);
		unsigned g = qGreen(pixelval);
		unsigned b = qBlue(pixelval);
		unsigned a = qAlpha(pixelval);
#if 0
		qDebug() << " r " << r << " g " << g << " b " << b << " " << a;
#endif
		if (faceID) *faceID = (int)(16777216 * a + 65536 * b + 256 * g + r);
		return true;
	}

	void ColorPicker::setDebugName(const QString& name)
	{
#ifdef _DEBUG
		m_debugName = name;
#endif
	}

	void ColorPicker::setRequestCallback(requestCallFunc func)
	{
		m_requestCallback = func;
	}

	void ColorPicker::useSelfCameraSelector(bool use)
	{
		qDebug() << "ColorPicker::useSelfCameraSelector " << use;
		if (use)
		{
			m_cameraSelector->setParent(m_renderPassFilter);
		}
		else
		{
			m_cameraSelector->setParent((Qt3DCore::QNode*)nullptr);
		}
	}

	Qt3DRender::QCamera* ColorPicker::camera()
	{
		return m_camera;
	}

	Qt3DRender::QTexture2D* ColorPicker::colorTexture()
	{
		if(m_textureRenderTarget) 
			return m_textureRenderTarget->colorTexture();
		return nullptr;
	}

	void ColorPicker::setTextureRenderTarget(TextureRenderTarget* textureRenderTarget)
	{
		qDebug() << "ColorPicker::setTextureRenderTarget " << m_textureRenderTarget;
		m_textureRenderTarget = textureRenderTarget;
		if(m_textureRenderTarget) m_renderTargetSelector->setTarget(m_textureRenderTarget);
	}
}
