#include "triangleentity.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"

namespace qtuser_3d
{
	TriangleEntity::TriangleEntity()
	{

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
}