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
		setObjectName("BoxEntity");
	}

	BoxEntity::~BoxEntity()
	{

	}

	void BoxEntity::updateGlobal(const Box3D& box, bool need_bottom)
	{
		QMatrix4x4 m;

		m.translate(box.min);
		m.scale(box.size());

		if (need_bottom)
		{
			setGeometry(createLinesPrimitive("box"), Qt3DRender::QGeometryRenderer::Lines);
		}
		else
		{
			setGeometry(createLinesPrimitive("box_nobottom"), Qt3DRender::QGeometryRenderer::Lines);
		}
		setPose(m);
	}

	void BoxEntity::updateLocal(const Box3D& box, float ratio)
	{
		setGeometry(BoxCreateHelper::createPartBox(box, ratio), Qt3DRender::QGeometryRenderer::Lines);
	}

	void BoxEntity::update(const Box3D& box)
	{
		setGeometry(BoxCreateHelper::create(box), Qt3DRender::QGeometryRenderer::Lines);
	}

	void BoxEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}