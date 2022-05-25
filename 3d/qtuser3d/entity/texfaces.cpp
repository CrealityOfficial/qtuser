#include "texfaces.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "qtuser3d/utils/textureload.h"

#include <Qt3DRender/QBlendEquationArguments>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QNoDepthMask>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QTextureImage>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QMultiSampleAntiAliasing>
#include <Qt3DRender/QDithering>
#include <Qt3DRender/QLineWidth>
#include <Qt3DRender/QPolygonOffset>

#include <QVector2D>

namespace qtuser_3d
{
	TexFaces::TexFaces(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		Qt3DRender::QEffect* effect = EFFECTCREATE("printerbottom", this);
		//loadImage();

		m_colorParameter = createParameter("color", QVector4D(0.27f, 0.27f, 0.27f, 1.0f));//createParameter("color", QVector4D(0.90f, 0.90f, 0.90f, 0.3f));
		m_logcolorParameter = createParameter("logcolor", QVector4D(0.2f, 0.2f, 0.2f, 0.8f));
		//Qt3DRender::QEffect* effect = EFFECTCREATE("pure", m_material);
		m_visibleParameter = createParameter("visible", 1);
		
		ImageTexture* backgroundImage = new ImageTexture(QString::fromUtf8(":/UI/images/creality_logo.png"));
		Qt3DRender::QTexture2D* imageTexture = new Qt3DRender::QTexture2D();
		imageTexture->addTextureImage(backgroundImage);
		imageTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
		imageTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
		QVector2D imageshape((float)(backgroundImage->width()), (float)(backgroundImage->height()));

		m_imageshape =createParameter("imageshape", imageshape);
		m_shapeTexture =createParameter("shapeTexture", QVariant::fromValue(imageTexture));
		m_imagebili = createParameter("imagebili", QVector2D(2.0f, 1.4f));
		m_colorVisibleParameter = createParameter("colorVisible", 0);
		m_imageColor = 0;
		 
		
		m_platformsizeParameter = createParameter("platformsize", QVector2D(1.0f, 1.0f));

		Qt3DRender::QBlendEquationArguments* blend = blendArguments();
		if (blend)
		{
			blend->setSourceRgba(Qt3DRender::QBlendEquationArguments::SourceAlpha);
			blend->setDestinationRgba(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);    // OneMinusSourceAlpha
		}

		Qt3DRender::QDithering* dither = new Qt3DRender::QDithering(this);
		Qt3DRender::QMultiSampleAntiAliasing* antiAlias = new Qt3DRender::QMultiSampleAntiAliasing(this);
		Qt3DRender::QNoDepthMask* mask = new Qt3DRender::QNoDepthMask(this);
		Qt3DRender::QLineWidth* lineWidth = new Qt3DRender::QLineWidth(this);
		lineWidth->setSmooth(true);
		Qt3DRender::QPolygonOffset* polygonOffset = new Qt3DRender::QPolygonOffset(this);
		polygonOffset->setScaleFactor(3.0);

		QList<Qt3DRender::QRenderPass*> passes = effect->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
		for (Qt3DRender::QRenderPass* pass : passes)
		{
			pass->addRenderState(mask);
			pass->addRenderState(antiAlias);
			pass->addRenderState(dither);
			pass->addRenderState(lineWidth);
			pass->addRenderState(polygonOffset);
		}
		setEffect(effect);
	}
	
	TexFaces::~TexFaces()
	{
	}

	void TexFaces::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}

	void TexFaces::setLogColor(const QVector4D& color)
	{
		m_logcolorParameter->setValue(color);
	}

	void TexFaces::loadImage(int type)
	{
		m_imageColor = type;
		if (type ==1)
		{
			ImageTexture* backgroundImage = new ImageTexture(QString::fromUtf8(":/UI/images/hotbottom.png"));
			Qt3DRender::QTexture2D* imageTexture = new Qt3DRender::QTexture2D();
			imageTexture->addTextureImage(backgroundImage);
			imageTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
			imageTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
			QVector2D imageshape((float)(backgroundImage->width()), (float)(backgroundImage->height()));
			
			m_imageshape->setValue(imageshape);
			m_shapeTexture->setValue(QVariant::fromValue(imageTexture));
			m_imagebili->setValue(QVector2D(1.0f, 1.0f));
			m_colorVisibleParameter->setValue(1);
			m_visibleParameter->setValue(1);
		} 
		else if (type == 2)
		{
			ImageTexture* backgroundImage = new ImageTexture(QString::fromUtf8(":/UI/images/hotbottom9.png"));
			Qt3DRender::QTexture2D* imageTexture = new Qt3DRender::QTexture2D();
			imageTexture->addTextureImage(backgroundImage);
			imageTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
			imageTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
			QVector2D imageshape((float)(backgroundImage->width()), (float)(backgroundImage->height()));

			m_imageshape->setValue(imageshape);
			m_shapeTexture->setValue(QVariant::fromValue(imageTexture));
			m_imagebili->setValue(QVector2D(1.0f, 1.0f));
			m_colorVisibleParameter->setValue(1);
			m_visibleParameter->setValue(1);
		}
		else
		{
			ImageTexture* backgroundImage = new ImageTexture(QString::fromUtf8(":/UI/images/creality_logo.png"));
			Qt3DRender::QTexture2D* imageTexture = new Qt3DRender::QTexture2D();
			imageTexture->addTextureImage(backgroundImage);
			imageTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
			imageTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
			QVector2D imageshape((float)(backgroundImage->width()), (float)(backgroundImage->height()));

			m_imageshape->setValue(imageshape);
			m_shapeTexture->setValue(QVariant::fromValue(imageTexture));
			m_imagebili->setValue(QVector2D(2.0f, 1.4f));
			m_colorVisibleParameter->setValue(0);
		}
	}

	void TexFaces::updateBox(const Box3D& box)
	{
		int vertexNum = 4;
		int triangleNum = 2;
		QVector<QVector3D> positions;
		QVector<QVector3D> normals;
		QVector<unsigned> indices;
		float minX = box.min.x();
		float maxX = box.max.x();
		float minY = box.min.y();
		float maxY = box.max.y();
		float minZ = box.min.z();
		float maxZ = box.max.z();
//		minZ -= 0.001;
		positions.push_back(QVector3D(minX, minY, minZ));
		positions.push_back(QVector3D(maxX, minY, minZ));
		positions.push_back(QVector3D(minX, maxY, minZ));
		positions.push_back(QVector3D(maxX, maxY, minZ));

		normals.push_back(QVector3D(0, 0, 1));
		normals.push_back(QVector3D(0, 0, 1));
		normals.push_back(QVector3D(0, 0, 1));
		normals.push_back(QVector3D(0, 0, 1));

		QVector3D sz = box.size();
		m_platformsizeParameter->setValue(QVector2D(sz.x(), sz.y()));

		std::vector<float> texcoords = { 0, 1, 1, 1, 0, 0, 1, 0 };
		//down
		indices.push_back(0); indices.push_back(1); indices.push_back(3);
		indices.push_back(0); indices.push_back(3); indices.push_back(2);
		Qt3DRender::QGeometry* geometry = qtuser_3d::TrianglesCreateHelper::createWithTex(vertexNum, (float*)&positions.at(0), (float*)&normals.at(0), (float*)&texcoords.at(0), triangleNum, (unsigned*)&indices.at(0));
		setGeometry(geometry);
		setEnabled(true);
	}

	void TexFaces::setImageVisible(bool visible)
	{
		m_visibleParameter->setValue(visible ? 1 : 0);
	}

	int TexFaces::isImageColor()
	{
		return m_imageColor;
	}

}