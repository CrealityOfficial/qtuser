#include "boxentity.h"

#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/boxcreatehelper.h"
namespace qtuser_3d
{
	BoxEntity::BoxEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_colorParameter = createParameter("color", QVector4D(1.0f, 0.0f, 0.0f, 1.0f));

		setEffect(EFFECT("pure"));
	}

	BoxEntity::~BoxEntity()
	{

	}

	void BoxEntity::updateGlobal(Box3D& box)
	{
		QMatrix4x4 m;

		m.translate(box.min);
		m.scale(box.size());

		setGeometry(PRIMITIVESHAPE("box"), Qt3DRender::QGeometryRenderer::Lines);
		setPose(m);
	}

	void BoxEntity::updateLocal(Box3D& box, const QMatrix4x4& parentMatrix)
	{
		setGeometry(BoxCreateHelper::createPartBox(box, 0.3), Qt3DRender::QGeometryRenderer::Lines);

		QMatrix4x4 m = parentMatrix.inverted();
		setPose(m);
	}

	void BoxEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}