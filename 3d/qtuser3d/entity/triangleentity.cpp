#include "triangleentity.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"

namespace qtuser_3d
{
	TriangleEntity::TriangleEntity()
	{
		m_texParameter = createParameter("shapeTexture", QVariant::fromValue(nullptr));
	}

	TriangleEntity::~TriangleEntity()
	{

	}

	void TriangleEntity::updateGeometry(int pointsNum, float* positions, float* normal, int triangleNum, int* indices)
	{
		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles;
		setGeometry(TrianglesCreateHelper::create(pointsNum, positions, normal, nullptr, triangleNum, (unsigned*)indices), type);
	}

	void TriangleEntity::updateGeometry(int pointsNum, float* positions, float* normals)
	{
		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles;
		setGeometry(TrianglesCreateHelper::create(pointsNum, positions, normals, nullptr, 0, nullptr, nullptr), type);
	}

	void TriangleEntity::updateGeometry(int pointsNum, float* positions, float* normals, float* uv)
	{
		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles;
		setGeometry(TrianglesCreateHelper::createWithTex(pointsNum, positions, normals, uv, 0, nullptr, nullptr), type);
	}

	void TriangleEntity::setTexture(Qt3DRender::QTexture2D* texture)
	{
		m_texParameter->setValue(QVariant::fromValue(texture));
	}
}