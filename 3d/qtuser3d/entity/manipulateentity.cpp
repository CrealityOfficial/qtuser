#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent)
		:PickableEntity(parent)
	{
		setEffect(EFFECTCREATE("manipulate_pickFace.pick", m_material));
		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));

		QList<Qt3DRender::QDepthTest*> tests = m_material->findChildren<Qt3DRender::QDepthTest*>(QString(), Qt::FindChildrenRecursively);
		for(Qt3DRender::QDepthTest* test : tests)
			test->setDepthFunction(Qt3DRender::QDepthTest::Always);
	}

	ManipulateEntity::~ManipulateEntity()
	{

	}

	void ManipulateEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}
