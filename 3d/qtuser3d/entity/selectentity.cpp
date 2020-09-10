#include "selectentity.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QBuffer>

namespace qtuser_3d
{
	SelectEntity::SelectEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		setEffect(EFFECT("pure"));
		createParameter("color", QVector4D(0.6f, 0.6f, 0.0f, 1.0f));
	}

	SelectEntity::~SelectEntity()
	{
	}

	void SelectEntity::updateData(const std::vector<QVector3D>& vertexData)
	{
		if (vertexData.size() > 0)
		{
			Qt3DRender::QGeometry* geometry = TrianglesCreateHelper::create((int)vertexData.size(), (float*)&vertexData.at(0), nullptr, nullptr, 0, nullptr);
			setGeometry(geometry);
		}
	}

}