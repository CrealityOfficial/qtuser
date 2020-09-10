#include "boxcreatehelper.h"

#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/boxgeometrydata.h"

namespace qtuser_3d
{
	BoxCreateHelper::BoxCreateHelper(QObject* parent)
		:GeometryCreateHelper(parent)
	{
	}
	
	BoxCreateHelper::~BoxCreateHelper()
	{
	}

	Qt3DRender::QGeometry* BoxCreateHelper::create(Qt3DCore::QNode* parent)
	{
		Qt3DRender::QAttribute* positionAttribute = BufferHelper::CreateVertexAttribute((const char*)static_box_position, AttribueSlot::Position, static_box_vertex_count);
		Qt3DRender::QAttribute* indexAttribute = BufferHelper::CreateIndexAttribute((const char*)static_box_indices, 24);

		return GeometryCreateHelper::create(parent, positionAttribute, indexAttribute);
	}

	Qt3DRender::QGeometry* BoxCreateHelper::create(const Box3D& box, Qt3DCore::QNode* parent)
	{
		QVector<QVector3D> positions;
		positions.resize(8);

		QVector3D bmin = box.min;
		QVector3D bmax = box.max;

		positions[0] = bmin;
		positions[1] = QVector3D(bmax.x(), bmin.y(), bmin.z());
		positions[2] = QVector3D(bmax.x(), bmax.y(), bmin.z());
		positions[3] = QVector3D(bmin.x(), bmax.y(), bmin.z());
		positions[4] = QVector3D(bmin.x(), bmin.y(), bmax.z());
		positions[5] = QVector3D(bmax.x(), bmin.y(), bmax.z());
		positions[6] = bmax;
		positions[7] = QVector3D(bmin.x(), bmax.y(), bmax.z());

		Qt3DRender::QAttribute* positionAttribute = BufferHelper::CreateVertexAttribute((const char*)&positions.at(0), AttribueSlot::Position, 8);
		Qt3DRender::QAttribute* indexAttribute = BufferHelper::CreateIndexAttribute((const char*)static_box_indices, 24);

		return GeometryCreateHelper::create(parent, positionAttribute, indexAttribute);
	}
}