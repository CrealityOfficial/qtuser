#include "modelnentity.h"
#include "qtuser3d/entity/boxentity.h"
#include "qtuser3d/entity/lineentity.h"

namespace qtuser_3d
{
	ModelNEntity::ModelNEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_boxEntity = new qtuser_3d::BoxEntity();
		m_boxEntity->setColor(QVector4D(0.306f, 0.702f, 0.769f, 1.0f));

		m_convexXYEntity = new qtuser_3d::LineEntity(this);
		m_convexXYEntity->setObjectName("ModelNEntity");

		m_stateParameter = createParameter("state", 0.0f);
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
		m_errorParameter = createParameter("error", 0.0f);
		m_supportCosParameter = createParameter("supportCos", 0.5);
		m_hoverParameter = createParameter("hoverState", 0);
		m_waterParameter = createParameter("water", QVector3D(0, 0, -10000));
		m_zchaParameter = createParameter("zcha", 0.01);
		m_waterShowParameter = createParameter("waterState", 0);
		m_fanzhuanParameter = createParameter("fanzhuan", 0);

		m_customColorParameter = createParameter("customColor", QColor::fromRgb(0.0, 0.0, 0.0));
		m_transparencyParameter = createParameter("transparency", 1.0f);
		m_lightingFlagParameter = createParameter("lightingEnable", 1);

		m_checkScopeParameter = createParameter("checkscope", 1);

		m_nozzleParameter = createParameter("nozzle", 0);

		m_renderModeParameter = createParameter("renderModel", 1);

		m_textureDiffuse = createParameter("textureDiffuse", QVariant::fromValue(nullptr));
		m_textureAmbient = createParameter("textureAmbient", QVariant::fromValue(nullptr));
		m_textureSpecular = createParameter("textureSpecular", QVariant::fromValue(nullptr));
		m_textureNormal = createParameter("textureNormal", QVariant::fromValue(nullptr));
	}
	
	ModelNEntity::~ModelNEntity()
	{
		if (!m_boxEntity->parent())
			delete m_boxEntity;
	}

	void ModelNEntity::setConvexName(QString name)
	{
		m_convexXYEntity->setObjectName(name);
	}

	void ModelNEntity::setBoxVisibility(bool visible)
	{
		m_boxEntity->setParent(visible ? (Qt3DCore::QNode*)this->parent() : nullptr);
		m_convexXYEntity->setParent(visible ? (Qt3DCore::QNode*)this->parent() : nullptr);
	}

	void ModelNEntity::updateBoxLocal(const Box3D& box, const QMatrix4x4& parentMatrix)
	{
		m_boxEntity->update(box, parentMatrix);
	}

	void ModelNEntity::setBoxColor(QVector4D color)
	{
		m_boxEntity->setColor(color);
	}

	void ModelNEntity::updateConvex(QVector<QVector3D>& points, QMatrix4x4& matrix, bool loop)
	{
		m_convexXYEntity->updateGeometry(points, loop);
		//m_convexXYEntity->setPose(matrix);
		QMatrix4x4 matrix2;
		matrix2.setToIdentity();
		m_convexXYEntity->setPose(matrix2);
	}

	void ModelNEntity::enterSupportStatus()
	{
		m_hoverParameter->setValue(1);
	}

	void ModelNEntity::leaveSupportStatus()
	{
		m_hoverParameter->setValue(0);
	}

	void ModelNEntity::setState(float state)
	{
		m_stateParameter->setValue(state);
	}

	void ModelNEntity::setNozzle(float nozzle)
	{
		m_nozzleParameter->setValue(nozzle);
	}

	void ModelNEntity::setVertexBase(QPoint vertexBase)
	{
		m_vertexBaseParameter->setValue(vertexBase);
	}

	void ModelNEntity::setErrorState(bool error)
	{
		m_errorParameter->setValue(error ? 1.0f : 0.0f);
	}

	void ModelNEntity::setSupportCos(float supportCos)
	{
		m_supportCosParameter->setValue(supportCos);
	}

	void ModelNEntity::setWaterPoint(const QVector3D& pt, float zcha)
	{
		m_waterShowParameter->setValue(1);
		m_waterParameter->setValue(pt);
		m_zchaParameter->setValue(zcha);
	}

	void ModelNEntity::unSetWaterPoint()
	{
		m_waterShowParameter->setValue(0);
	}

	void ModelNEntity::setFanZhuan(int fz)
	{
		m_fanzhuanParameter->setValue(fz);
	}

	//void ModelNEntity::setTexture(int buffsize[], unsigned char* buffs[])
	//{
		//int maptype = trimesh::Material::MapType::DIFFUSE;
		//int bufferSize = m_mesh->map_bufferSize[maptype];
		//enchase::ImageDataFormat formattype = enchase::ImageDataFormat::FORMAT_RGBA_8888;
		//unsigned char* imagedata = m_mesh->map_buffers[maptype];
		//if (imagedata)
		//{
		//	imgproc::ImageData bufferData;
		//	bufferData.format = imgproc::ImageDataFormat::FORMAT_RGBA_8888;
		//	imgproc::loadImageFromMem_freeImage(imagedata, bufferSize, bufferData);
		//	updateTextTexture(m_textureDiffuse, bufferData.width, bufferData.height, bufferData.data, formattype);
		//}

		//maptype = trimesh::Material::MapType::SPECULAR;
		//bufferSize = m_mesh->map_bufferSize[maptype];
		//imagedata = m_mesh->map_buffers[maptype];
		//if (imagedata)
		//{
		//	imgproc::ImageData specularData;
		//	specularData.format = imgproc::ImageDataFormat::FORMAT_RGBA_8888;
		//	imgproc::loadImageFromMem_freeImage(imagedata, bufferSize, specularData);
		//	updateTextTexture(m_textureSpecular, specularData.width, specularData.height, specularData.data, formattype);
		//}

		//maptype = trimesh::Material::MapType::AMBIENT;
		//bufferSize = m_mesh->map_bufferSize[maptype];
		//imagedata = m_mesh->map_buffers[maptype];
		//if (imagedata)
		//{
		//	imgproc::ImageData ambientData;
		//	ambientData.format = imgproc::ImageDataFormat::FORMAT_RGBA_8888;
		//	imgproc::loadImageFromMem_freeImage(imagedata, bufferSize, ambientData);
		//	updateTextTexture(m_textureAmbient, ambientData.width, ambientData.height, ambientData.data, formattype);
		//}

		//maptype = trimesh::Material::MapType::NORMAL;
		//bufferSize = m_mesh->map_bufferSize[maptype];
		//imagedata = m_mesh->map_buffers[maptype];
		//if (imagedata)
		//{
		//	imgproc::ImageData normalData;
		//	normalData.format = imgproc::ImageDataFormat::FORMAT_RGBA_8888;
		//	imgproc::loadImageFromMem_freeImage(imagedata, bufferSize, normalData);
		//	updateTextTexture(m_textureNormal, normalData.width, normalData.height, normalData.data, formattype);
		//}
	//}

	void ModelNEntity::setCustomColor(QColor color)
	{
		m_customColorParameter->setValue(color);
	}

	QColor ModelNEntity::getCustomColor()
	{
		return m_customColorParameter->value().value<QColor>();
	}

	void ModelNEntity::setTransparency(float alpha)
	{
		m_transparencyParameter->setValue(alpha);
	}

	float ModelNEntity::getTransparency()
	{
		return m_transparencyParameter->value().toFloat();
	}

	void ModelNEntity::setLightingEnable(bool enable)
	{
		m_lightingFlagParameter->setValue(enable ? 1 : 0);
	}

	bool ModelNEntity::getLightingEnable()
	{
		return m_lightingFlagParameter->value().toBool();
	}

	void ModelNEntity::setNeedCheckScope(int checkscope)
	{
		m_checkScopeParameter->setValue(checkscope);
	}


	void ModelNEntity::setRenderMode(int mode)
	{
		m_renderModeParameter->setValue(mode);
	}

	void ModelNEntity::setTDiffuse(Qt3DRender::QTexture2D* aDiffuse)
	{
		m_textureDiffuse->setValue(QVariant::fromValue(aDiffuse));
	}

	void ModelNEntity::setTAmbient(Qt3DRender::QTexture2D* aAmbient)
	{
		m_textureAmbient->setValue(QVariant::fromValue(aAmbient));
	}

	void ModelNEntity::setTSpecular(Qt3DRender::QTexture2D* aSpecular)
	{
		m_textureSpecular->setValue(QVariant::fromValue(aSpecular));
	}

	void ModelNEntity::setTNormal(Qt3DRender::QTexture2D* aNormal)
	{
		m_textureNormal->setValue(QVariant::fromValue(aNormal));
	}

	Qt3DRender::QTexture2D* ModelNEntity::updateTextTexture(int width, int height, unsigned char* data)
	{
		if (width < 1
			|| height < 1
			|| data == nullptr)
		{
			return nullptr;
		}

		QImage pTextImage;
		int size = width * height;
		//unsigned char* inData = new unsigned char[size];
		//memcpy(inData, data, size * sizeof(unsigned char));
		pTextImage.loadFromData(data, size);

		//QImage anew("C:\\Users\\cx0689\\Desktop\\cube_twoface\\cura-icon.png");
		Qt3DRender::QTexture2D* atexture = qtuser_3d::createFromImage(pTextImage);
		//atexture->setFormat(Qt3DRender::QTexture2D::RGBA8_SNorm);
		return atexture;
	}

	//bool ModelNEntity::updateTextTexture(Qt3DRender::QTexture2D* texture, int width, int height, unsigned char* data)
	//{

	//}

	//bool ModelNEntity::updateTexture(Qt3DRender::QTexture2D* textTex)
	//{

	//}

}