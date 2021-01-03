#include "imageentity.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QBuffer>
#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/geometry/basicshapecreatehelper.h"
#include "qtuser3d/geometry/gridcreatehelper.h"
#include "qtuser3d/utils/textureload.h"
#include "qtusercore/module/systemutil.h"

#include <QCoreApplication>
#include <QDir>

namespace qtuser_3d
{
	ImageEntity::ImageEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		setEffect(EFFECTCREATE("texshape", this));
		loadImage();
	}

	ImageEntity::~ImageEntity()
	{
	}

	void ImageEntity::loadImage()
	{
		//QString file = QString::fromUtf8(":/UI/images/logo.png");
		//QImage *image = new QImage();
		//image->load(file, "png");
		//qDebug() << "image.size() = [" << image->width() << ", " << image->height() << "]";

		LogoTextureImage* backgroundImage = new LogoTextureImage();

		//Qt3DRender::QTextureImage* backgroundImage = new Qt3DRender::QTextureImage();
		//backgroundImage->setSource(QUrl("qrc:/UI/images/logo.png"));

		Qt3DRender::QTexture2D* imageTexture = new Qt3DRender::QTexture2D();
		imageTexture->addTextureImage(backgroundImage);
		imageTexture->setMinificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);
		imageTexture->setMagnificationFilter(Qt3DRender::QAbstractTexture::LinearMipMapLinear);

		createParameter("shapeTexture", QVariant::fromValue(imageTexture));
	}

	void ImageEntity::updateGlobal(Box3D& box)
	{
		int vertexNum = 4;
		int triangleNum = 2;
		QVector<QVector3D> positions;
		
		float minX = box.min.x();
		float maxX = box.max.x();
		float minY = box.min.y();
		float maxY = box.max.y();
		float woffset = -(maxX - minX) / 8.0 * 3.0;
		float hoffset = 2 / 220.0 * (maxX - minX);
		float h = 8.0 / 220.0 * (maxX - minX);

		positions.push_back(QVector3D(minX - woffset, minY - hoffset - h, 0.0f));
		positions.push_back(QVector3D(maxX + woffset, minY - hoffset - h, 0.0f));
		positions.push_back(QVector3D(maxX + woffset, minY - hoffset, 0.0f));
		positions.push_back(QVector3D(minX - woffset, minY - hoffset, 0.0f));

		std::vector<float> texcoords = { 0, 0, 1, 0, 1, 1, 0, 1 };
		QVector<unsigned> indices = { 0, 1, 2, 0, 2, 3 };

		Qt3DRender::QGeometry* geometry = qtuser_3d::TrianglesCreateHelper::createWithTex(vertexNum, (float*)&positions.at(0), nullptr, (float*)&texcoords.at(0), triangleNum, (unsigned*)&indices.at(0));
		setGeometry(geometry);

		//QVector3D minPos = box.min;

		//Box3D boxnew(QVector3D(minX - woffset, minY - hoffset - h, 0.0), QVector3D(maxX + woffset, minY - hoffset, 0.0));

		//Qt3DRender::QGeometry* geometry = GridCreateHelper::createTextureQuad(boxnew, true);
		//setGeometry(geometry);
	}

	void ImageEntity::show()
	{
		setEnabled(true);
	}

	void ImageEntity::hide()
	{
		setEnabled(false);
	}


}