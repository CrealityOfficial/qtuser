#include "bottomface.h"
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

#include <QVector2D>

namespace qtuser_3d
{
	BottomFace::BottomFace(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		Qt3DRender::QEffect* effect = EFFECTCREATE("bottomface", this);
		setObjectName("BottomFace");

		m_colorParameter = createParameter("color", QVector4D(0.90f, 0.90f, 0.90f, 0.3f));

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

		QList<Qt3DRender::QRenderPass*> passes = effect->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
		for (Qt3DRender::QRenderPass* pass : passes)
		{
			pass->addRenderState(mask);
			pass->addRenderState(antiAlias);
			pass->addRenderState(dither);
			pass->addRenderState(lineWidth);
		}
		setEffect(effect);
	}
	
	BottomFace::~BottomFace()
	{
	}

	void BottomFace::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}


	void BottomFace::updateBox(const Box3D& box)
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

		//down
		indices.push_back(0); indices.push_back(1); indices.push_back(3);
		indices.push_back(0); indices.push_back(3); indices.push_back(2);
		Qt3DRender::QGeometry* geometry = qtuser_3d::TrianglesCreateHelper::createWithTex(vertexNum, (float*)&positions.at(0), (float*)&normals.at(0), nullptr, triangleNum, (unsigned*)&indices.at(0));
		setGeometry(geometry);
		setEnabled(true);
	}

}