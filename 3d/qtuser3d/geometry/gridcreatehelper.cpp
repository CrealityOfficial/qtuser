#include "gridcreatehelper.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include <QtCore/qmath.h>
#include <QtGui/QVector2D>
#include <QtGui/QVector3D>

namespace qtuser_3d
{
	GridCreateHelper::GridCreateHelper(QObject* parent)
		:GeometryCreateHelper(parent)
	{
	}
	
	GridCreateHelper::~GridCreateHelper()
	{
	}

	Qt3DRender::QGeometry* GridCreateHelper::create(Box3D& box, float gap, Qt3DCore::QNode * parent)
	{
		QVector3D size = box.size();
		if (size.x() == 0 || size.y() == 0) return nullptr;

		float minX = box.min.x();
		float maxX = box.max.x();
		float minY = box.min.y();
		float maxY = box.max.y();

		int startX = 1;
		int endX = qFloor(maxX / gap);
		if (float(endX) * gap == maxX) endX -= 1;
		int startY = 1;
		int endY = qFloor(maxY / gap);
		if (float(endY) * gap == maxY) endY -= 1;

		int sizeX = endX - startX + 1;
		int sizeY = endY - startY + 1;
		if (sizeX <= 0 || sizeY <= 0)
			return nullptr;

		int vertexCount = 2 * (sizeX + sizeY);
		std::vector<QVector3D> positions(vertexCount);
		std::vector<QVector2D> flags(vertexCount);

		int vertexIndex = 0;
		for (int i = startX; i <= endX; ++i)
		{
			positions.at(vertexIndex) = QVector3D(float(i) * gap, minY, -0.1f);
			flags.at(vertexIndex) = QVector2D(-1.0f, float(i) * gap);
			vertexIndex++;
			positions.at(vertexIndex) = QVector3D(float(i) * gap, maxY, -0.1f);
			flags.at(vertexIndex) = QVector2D(-1.0f, float(i) * gap);
			vertexIndex++;
		}
		for (int i = startY; i <= endY; ++i)
		{
			positions.at(vertexIndex) = QVector3D(minX, float(i) * gap, -0.1f);
			flags.at(vertexIndex) = QVector2D(1.0f, float(i) * gap);
			vertexIndex++;
			positions.at(vertexIndex) = QVector3D(maxX, float(i) * gap, -0.1f);
			flags.at(vertexIndex) = QVector2D(1.0f, float(i) * gap);
			vertexIndex++;
		}

		Qt3DRender::QAttribute* positionAttribute = BufferHelper::CreateVertexAttribute((const char*)& positions.at(0), AttribueSlot::Position, vertexCount);
		Qt3DRender::QAttribute* flagAttribute = BufferHelper::CreateVertexAttribute("vertexFlag", (const char*)& flags.at(0), 2, vertexCount);
		return GeometryCreateHelper::create(parent, positionAttribute, flagAttribute);
	}

	Qt3DRender::QGeometry* GridCreateHelper::createMid(Box3D& box, float gap, float offset, Qt3DCore::QNode* parent)
	{
		QVector3D size = box.size();
		if (size.x() == 0 || size.y() == 0) return nullptr;

		float minX = box.min.x();
		float maxX = box.max.x();
		float minY = box.min.y();
		float maxY = box.max.y();

		if (maxX - minX <= 0 || maxY - minY <= 0)
		{
			return nullptr;
		}
		//minX -= offset;
		//maxX += offset;
		//minY -= offset;
		//maxY += offset;

		int startX = 0;
		int endX = qFloor(maxX / gap);
//		if (float(endX) * gap == maxX) endX -= 1;
		int startY = 0;
		int endY = qFloor(maxY / gap);
//		if (float(endY) * gap == maxY) endY -= 1;

		float midX = ((float)startX + (float)endX) / 2.0;
		float midY = ((float)startY + (float)endY) / 2.0;

		int xNum = ceil(endX - midX);
		int yNum = ceil(endY - midY);

		int sizeX = xNum * 2 + 1;
		int sizeY = yNum * 2 + 1;

		int vertexCount = 2 * (sizeX + sizeY);
		std::vector<QVector3D> positions(vertexCount);
		std::vector<QVector2D> flags(vertexCount);

		float zcoord = 0.05f;

		int vertexIndex = 0;
		float i;
		for (i = midX; i < endX; i += 1.0)
		{
			positions.at(vertexIndex) = QVector3D(i * gap, minY, zcoord);
			flags.at(vertexIndex) = QVector2D((i - midX) * gap, 1.0);
			vertexIndex++;
			positions.at(vertexIndex) = QVector3D(i * gap, maxY, zcoord);
			flags.at(vertexIndex) = QVector2D((i - midX) * gap, 1.0);
			vertexIndex++;
		}
		positions.at(vertexIndex) = QVector3D(float(endX) * gap, minY, zcoord);
		flags.at(vertexIndex) = QVector2D((endX - midX) * gap, 1.0);
		vertexIndex++;
		positions.at(vertexIndex) = QVector3D(float(endX) * gap, maxY, zcoord);
		flags.at(vertexIndex) = QVector2D((endX - midX) * gap, 1.0);
		vertexIndex++;

		for (i = midX - 1.0; i > startX; i -= 1.0)
		{
			positions.at(vertexIndex) = QVector3D(i * gap, minY, zcoord);
			flags.at(vertexIndex) = QVector2D((i - midX) * gap, 1.0);
			vertexIndex++;
			positions.at(vertexIndex) = QVector3D(i * gap, maxY, zcoord);
			flags.at(vertexIndex) = QVector2D((i - midX) * gap, 1.0);
			vertexIndex++;
		}
		positions.at(vertexIndex) = QVector3D(float(startX) * gap, minY, zcoord);
		flags.at(vertexIndex) = QVector2D((startX - midX) * gap, 1.0);
		vertexIndex++;
		positions.at(vertexIndex) = QVector3D(float(startX) * gap, maxY, zcoord);
		flags.at(vertexIndex) = QVector2D((startX - midX) * gap, 1.0);
		vertexIndex++;

		for (i = midY; i < endY; i += 1.0)
		{
			positions.at(vertexIndex) = QVector3D(minX, i * gap, zcoord);
			flags.at(vertexIndex) = QVector2D(1.0f, (i - midY) * gap);
			vertexIndex++;
			positions.at(vertexIndex) = QVector3D(maxX, i * gap, zcoord);
			flags.at(vertexIndex) = QVector2D(1.0f, (i - midY) * gap);
			vertexIndex++;
		}
		positions.at(vertexIndex) = QVector3D(minX, endY * gap, zcoord);
		flags.at(vertexIndex) = QVector2D(1.0f, (endY - midY) * gap);
		vertexIndex++;
		positions.at(vertexIndex) = QVector3D(maxX, endY * gap, zcoord);
		flags.at(vertexIndex) = QVector2D(1.0f, (endY - midY) * gap);
		vertexIndex++;

		for (i = midY - 1.0; i > startY; i -= 1.0)
		{
			positions.at(vertexIndex) = QVector3D(minX, i * gap, zcoord);
			flags.at(vertexIndex) = QVector2D(1.0f, (i - midY) * gap);
			vertexIndex++;
			positions.at(vertexIndex) = QVector3D(maxX, i * gap, zcoord);
			flags.at(vertexIndex) = QVector2D(1.0f, (i - midY) * gap);
			vertexIndex++;
		}
		positions.at(vertexIndex) = QVector3D(minX, startY * gap, zcoord);
		flags.at(vertexIndex) = QVector2D(1.0f, (startY - midY) * gap);
		vertexIndex++;
		positions.at(vertexIndex) = QVector3D(maxX, startY * gap, zcoord);
		flags.at(vertexIndex) = QVector2D(1.0f, (startY - midY) * gap);
		vertexIndex++;

		Qt3DRender::QAttribute* positionAttribute = BufferHelper::CreateVertexAttribute((const char*)&positions.at(0), AttribueSlot::Position, vertexCount);
		Qt3DRender::QAttribute* flagAttribute = BufferHelper::CreateVertexAttribute("vertexFlag", (const char*)&flags.at(0), 2, vertexCount);
		return GeometryCreateHelper::create(parent, positionAttribute, flagAttribute);
	}

	Qt3DRender::QGeometry* GridCreateHelper::createPlane(float width, float height, bool triangle, Qt3DCore::QNode* parent)
	{
		float w = qAbs(width);
		float h = qAbs(height);
		std::vector<QVector3D> positions;
		if (triangle)
		{
			positions.push_back(QVector3D(-w, -h, 0.0f));
			positions.push_back(QVector3D(w, -h, 0.0f));
			positions.push_back(QVector3D(-w, h, 0.0f));
			positions.push_back(QVector3D(w, -h, 0.0f));
			positions.push_back(QVector3D(w, h, 0.0f));
			positions.push_back(QVector3D(-w, h, 0.0f));
		}
		else
		{
			positions.push_back(QVector3D(-w, -h, 0.0f));
			positions.push_back(QVector3D(w, -h, 0.0f));
			positions.push_back(QVector3D(w, -h, 0.0f));
			positions.push_back(QVector3D(w, h, 0.0f));
			positions.push_back(QVector3D(w, h, 0.0f));
			positions.push_back(QVector3D(-w, h, 0.0f));
			positions.push_back(QVector3D(-w, h, 0.0f));
			positions.push_back(QVector3D(-w, -h, 0.0f));
		}

		Qt3DRender::QAttribute* positionAttribute = BufferHelper::CreateVertexAttribute((const char*)& positions.at(0), AttribueSlot::Position, (int)positions.size());
		return GeometryCreateHelper::create(parent, positionAttribute);
	}

	Qt3DRender::QGeometry* GridCreateHelper::createTextureQuad(Box3D& box, bool flatZ, Qt3DCore::QNode* parent)
	{
		QVector<QVector3D> points;
		QVector3D bmin = box.min;
		bmin.setZ(0.0f);
		QVector3D bmax = box.max;
		bmax.setZ(0.0f);
		points.push_back(bmin);
		points.push_back(QVector3D(bmax.x(), bmin.y(), 0.0f));
		points.push_back(bmax);
		points.push_back(bmin);
		points.push_back(bmax);
		points.push_back(QVector3D(bmin.x(), bmax.y(), 0.0f));

		QVector<QVector2D> texcoord;
		QVector2D tmin(0.0f, 0.0f);
		QVector2D tmax(1.0f, 1.0f);
		texcoord.push_back(tmin);
		texcoord.push_back(QVector2D(tmax.x(), tmin.y()));
		texcoord.push_back(tmax);
		texcoord.push_back(tmin);
		texcoord.push_back(tmax);
		texcoord.push_back(QVector2D(tmin.x(), tmax.y()));

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)(&points.at(0)), qtuser_3d::AttribueSlot::Position, points.size());
		Qt3DRender::QAttribute* texcoordAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute("vertexTexcoord", (const char*)(&texcoord.at(0)), 2, points.size());

		return GeometryCreateHelper::create(parent, positionAttribute, texcoordAttribute);;
	}

}