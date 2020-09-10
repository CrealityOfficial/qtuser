#include "basicshapecreatehelper.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include <math.h>
#include <QVector3D>


namespace trimesh
{
	class TriMesh;
}

namespace qtuser_3d
{
	BasicShapeCreateHelper::BasicShapeCreateHelper(QObject* parent)
		:GeometryCreateHelper(parent)
	{
	}
	
	BasicShapeCreateHelper::~BasicShapeCreateHelper()
	{
	}

	Qt3DRender::QGeometry* BasicShapeCreateHelper::createCylinder(Qt3DCore::QNode* parent)
	{
		std::vector<float> datas;
		createCylinderData(0.3, 6, 36, datas);
		Qt3DRender::QAttribute* positionAttribute = nullptr;
		positionAttribute = BufferHelper::CreateVertexAttribute((const char*)&datas[0], AttribueSlot::Position, datas.size() / 3);

		return GeometryCreateHelper::create(parent, positionAttribute, nullptr, nullptr, nullptr);
	}

	Qt3DRender::QGeometry* BasicShapeCreateHelper::createRectangle(float w, float h, Qt3DCore::QNode* parent)
	{
		std::vector<float> datas = { 
			0, 0, 0, 
			w, 0, 0, 
			w, h, 0, 
			0, h, 0 };

		Qt3DRender::QAttribute* positionAttribute = nullptr;
		positionAttribute = BufferHelper::CreateVertexAttribute((const char*)&datas[0], AttribueSlot::Position, datas.size() / 3);

		QVector<unsigned> indices = {
			0, 1, 2,
			0, 2, 3 };

		Qt3DRender::QAttribute* indicesAttribute = nullptr;
		indicesAttribute = BufferHelper::CreateIndexAttribute((const char*)&indices[0], indices.size() / 3);

		return GeometryCreateHelper::create(parent, positionAttribute, indicesAttribute, nullptr, nullptr);
	}

	Qt3DRender::QGeometry* BasicShapeCreateHelper::createPen(Qt3DCore::QNode* parent)
	{
		std::vector<float> vertexDatas, normalDatas;
		createPenData(1.8, 3, 14, 36, vertexDatas, normalDatas);
		Qt3DRender::QAttribute* positionAttribute = nullptr;
		Qt3DRender::QAttribute* normalAttribute = nullptr;
		positionAttribute = BufferHelper::CreateVertexAttribute((const char*)&vertexDatas[0], AttribueSlot::Position, vertexDatas.size() / 3);
		normalAttribute = BufferHelper::CreateVertexAttribute((const char*)&normalDatas[0], AttribueSlot::Normal, normalDatas.size() / 3);

		return GeometryCreateHelper::create(parent, positionAttribute, normalAttribute, nullptr, nullptr);
	}

	int BasicShapeCreateHelper::createCylinderData(float r, float h, int seg, std::vector<float> &datas)
	{
		const float PI = 3.1415926535897932384;
		float hoffset = h/2;

		float angdesSpan = 360.0 / seg;
		for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdesSpan)
		{
			float angrad = angdeg * PI / 180.0;
			float angradNext = (angdeg + angdesSpan) * PI / 180.0;

			datas.push_back(0);
			datas.push_back(h / 2 + hoffset);
			datas.push_back(0);

			datas.push_back(-r * sin(angrad));
			datas.push_back(h / 2 + hoffset);
			datas.push_back(-r * cos(angrad));

			datas.push_back(-r * sin(angradNext));
			datas.push_back(h / 2 + hoffset);
			datas.push_back(-r * cos(angradNext));
		}
		for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdesSpan)
		{
			float angrad = angdeg * PI / 180.0;
			float angradNext = (angdeg + angdesSpan) * PI / 180.0;

			datas.push_back(0);
			datas.push_back(-h / 2 + hoffset);
			datas.push_back(0);

			datas.push_back(-r * sin(angradNext));
			datas.push_back(-h / 2 + hoffset);
			datas.push_back(-r * cos(angradNext));

			datas.push_back(-r * sin(angrad));
			datas.push_back(-h / 2 + hoffset);
			datas.push_back(-r * cos(angrad));
		}
		for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdesSpan)
		{
			float angrad = angdeg * PI / 180.0;
			float angradNext = (angdeg + angdesSpan) * PI / 180.0;

			datas.push_back(-r * sin(angrad));
			datas.push_back(-h / 2 + hoffset);
			datas.push_back(-r * cos(angrad));

			datas.push_back(-r * sin(angradNext));
			datas.push_back(h / 2 + hoffset);
			datas.push_back(-r * cos(angradNext));

			datas.push_back(-r * sin(angrad));
			datas.push_back(h / 2 + hoffset);
			datas.push_back(-r * cos(angrad));

			datas.push_back(-r * sin(angrad));
			datas.push_back(-h / 2 + hoffset);
			datas.push_back(-r * cos(angrad));

			datas.push_back(-r * sin(angradNext));
			datas.push_back(-h / 2 + hoffset);
			datas.push_back(-r * cos(angradNext));

			datas.push_back(-r * sin(angradNext));
			datas.push_back(h / 2 + hoffset);
			datas.push_back(-r * cos(angradNext));
		}

		return 0;
	}

	int BasicShapeCreateHelper::createPenData(float r, float headh, float bodyh, int seg, std::vector<float>& vertexDatas, std::vector<float>& normalDatas)
	{
		const float PI = 3.1415926535897932384;
		float angdesSpan = 360.0 / seg;

		for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdesSpan)
		{
			float angrad = angdeg * PI / 180.0;
			float angradNext = (angdeg + angdesSpan) * PI / 180.0;

			QVector3D v1(-r * sin(angrad), -r * cos(angrad), headh);
			QVector3D v2(-r * sin(angradNext), -r * cos(angradNext), bodyh);
			QVector3D v3(-r * sin(angrad), -r * cos(angrad), bodyh);

			QVector3D n = QVector3D::normal(v1, v2, v3);
			addFaceDataWithQVector3D(v1, v2, v3, n, vertexDatas, normalDatas);

			QVector3D v4(-r * sin(angrad), -r * cos(angrad), headh);
			QVector3D v5(-r * sin(angradNext), -r * cos(angradNext), headh);
			QVector3D v6(-r * sin(angradNext), -r * cos(angradNext), bodyh);

			QVector3D n2 = QVector3D::normal(v4, v5, v6);
			addFaceDataWithQVector3D(v4, v5, v6, n2, vertexDatas, normalDatas);
		}
		for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdesSpan)
		{
			float angrad = angdeg * PI / 180.0;
			float angradNext = (angdeg + angdesSpan) * PI / 180.0;

			QVector3D v1(0, 0, bodyh);
			QVector3D v2(-r * sin(angrad), -r * cos(angrad), bodyh);
			QVector3D v3(-r * sin(angradNext), -r * cos(angradNext), bodyh);

			QVector3D n = QVector3D::normal(v1, v2, v3);
			addFaceDataWithQVector3D(v1, v2, v3, n, vertexDatas, normalDatas);
		}
		for (float angdeg = 0; ceil(angdeg) < 360; angdeg += angdesSpan)
		{
			float angrad = angdeg * PI / 180.0;
			float angradNext = (angdeg + angdesSpan) * PI / 180.0;

			QVector3D v1(0, 0, headh * 0.05);
			QVector3D v2(-r * sin(angrad), -r * cos(angrad), headh);
			QVector3D v3(-r * sin(angradNext), -r * cos(angradNext), headh);

			QVector3D n = QVector3D::normal(v1, v2, v3);
			addFaceDataWithQVector3D(v1, v2, v3, n, vertexDatas, normalDatas);
		}

		return 0;
	}

	int BasicShapeCreateHelper::addFaceDataWithQVector3D(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3, const QVector3D& n, std::vector<float>& vertexDatas, std::vector<float>& normalDatas)
	{
		vertexDatas.push_back(v1.x());
		vertexDatas.push_back(v1.y());
		vertexDatas.push_back(v1.z());

		vertexDatas.push_back(v2.x());
		vertexDatas.push_back(v2.y());
		vertexDatas.push_back(v2.z());

		vertexDatas.push_back(v3.x());
		vertexDatas.push_back(v3.y());
		vertexDatas.push_back(v3.z());

		for (int i = 0; i < 3; i++)
		{
			normalDatas.push_back(n.x());
			normalDatas.push_back(n.y());
			normalDatas.push_back(n.z());
		}

		return 0;
	}
}
