#include "printerskirtentity.h"
#include "qtuser3d/effect/effectmanager.h"

#include "qtuser3d/geometry/trianglescreatehelper.h"
namespace qtuser_3d
{
	PrinterSkirtEntity::PrinterSkirtEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		//QVector4D color(0.180f, 0.541f, 0.968f, 1.0f);
		QVector4D color(0.44f, 0.44f, 0.44f, 1.0f);
		m_colorParameter = createParameter("color", color);

		setEffect(EFFECT("pure"));
	}
	
	PrinterSkirtEntity::~PrinterSkirtEntity()
	{
	}

	void PrinterSkirtEntity::updateBoundingBox(const Box3D& box)
	{
		QVector<QVector3D> positions;
		QVector<unsigned> indices;

		float minX = box.min.x();
		float maxX = box.max.x();
		float minY = box.min.y();
		float maxY = box.max.y();

		float offset = 12.0f / 220.0 * (maxX - minX);
		float textoffset = 14.0 / 220.0 * (maxX - minX);

		float logoffset = (maxX - minX) / 8.0 * 3.0;
		float cha = 2.0 / 220.0 * (maxX - minX);;

		positions.push_back(QVector3D(minX, minY, 0.0f));
		positions.push_back(QVector3D(minX, minY, 0.0f) + QVector3D(-offset, -offset, 0.0f));

		positions.push_back(QVector3D(minX + logoffset, minY, 0.0f));
		positions.push_back(QVector3D(minX + logoffset, minY - offset, 0.0f));
		positions.push_back(QVector3D(maxX - logoffset, minY, 0.0f));
		positions.push_back(QVector3D(maxX - logoffset, minY - offset, 0.0f));

		positions.push_back(QVector3D(maxX, minY, 0.0f));
		positions.push_back(QVector3D(maxX, minY, 0.0f) + QVector3D(offset, -offset, 0.0f));
		positions.push_back(QVector3D(maxX, maxY, 0.0f));
		positions.push_back(QVector3D(maxX, maxY, 0.0f) + QVector3D(offset, offset, 0.0f));
		positions.push_back(QVector3D(minX, maxY, 0.0f));
		positions.push_back(QVector3D(minX, maxY, 0.0f) + QVector3D(-offset, offset, 0.0f));

		positions.push_back(QVector3D(minX + logoffset, minY - cha, 0.0f));
		positions.push_back(QVector3D(minX + logoffset, minY - offset + cha, 0.0f));
		positions.push_back(QVector3D(maxX - logoffset, minY - cha, 0.0f));
		positions.push_back(QVector3D(maxX - logoffset, minY - offset + cha, 0.0f));

		//positions.push_back(QVector3D(minX - offset, minY - textoffset, 0.0f));
		//positions.push_back(QVector3D(maxX + offset, minY - textoffset, 0.0f));
		//positions.push_back(QVector3D(maxX + offset, minY - offset, 0.0f));
		//positions.push_back(QVector3D(minX - offset, minY - offset, 0.0f));


		//indices.push_back(0); indices.push_back(1); indices.push_back(2);
		//indices.push_back(2); indices.push_back(1); indices.push_back(3);
		//indices.push_back(2); indices.push_back(3); indices.push_back(4);
		//indices.push_back(4); indices.push_back(3); indices.push_back(5);
		//indices.push_back(4); indices.push_back(7); indices.push_back(6);
		//indices.push_back(4); indices.push_back(5); indices.push_back(7);
		//indices.push_back(6); indices.push_back(7); indices.push_back(1);
		//indices.push_back(0); indices.push_back(6); indices.push_back(1);

		//indices.push_back(8); indices.push_back(9); indices.push_back(10);
		//indices.push_back(8); indices.push_back(10); indices.push_back(11);


		indices.push_back(0); indices.push_back(1); indices.push_back(2);
		indices.push_back(2); indices.push_back(1); indices.push_back(3);
		indices.push_back(4); indices.push_back(5); indices.push_back(6);
		indices.push_back(6); indices.push_back(5); indices.push_back(7);

		indices.push_back(2 + 4); indices.push_back(3 + 4); indices.push_back(4 + 4);
		indices.push_back(4 + 4); indices.push_back(3 + 4); indices.push_back(5 + 4);
		indices.push_back(4 + 4); indices.push_back(7 + 4); indices.push_back(6 + 4);
		indices.push_back(4 + 4); indices.push_back(5 + 4); indices.push_back(7 + 4);
		indices.push_back(6 + 4); indices.push_back(7 + 4); indices.push_back(1);
		indices.push_back(0); indices.push_back(6 + 4); indices.push_back(1);

		indices.push_back(2); indices.push_back(12); indices.push_back(4);
		indices.push_back(4); indices.push_back(12); indices.push_back(14);
		indices.push_back(13); indices.push_back(3); indices.push_back(15);
		indices.push_back(15); indices.push_back(3); indices.push_back(5);

		Qt3DRender::QGeometry* geometry = qtuser_3d::TrianglesCreateHelper::create(positions.size(), (float*)&positions.at(0), nullptr, nullptr, indices.size() / 3, (unsigned*)&indices.at(0));
		setGeometry(geometry);
	}

	void PrinterSkirtEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}