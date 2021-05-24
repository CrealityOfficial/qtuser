#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>

namespace qtuser_3d
{
	ManipulateEntity::ManipulateEntity(Qt3DCore::QNode* parent, int type)
		:PickableEntity(parent)
	{
		QString shader_type = "manipulate.view_pickFace.pick";
		if (type == 1)
		{
			shader_type = "manipulate.view";
		}
		else if (type == 2)
		{
			shader_type = "pickFace.pick";
		}
		setEffect(EFFECTCREATE(shader_type, m_material));
		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 1.0f));
		m_changeColorParameter = createParameter("changecolor", QVector4D(0.0f, 0.0f, 0.0f, 0.0f));
		m_methodParameter = createParameter("mt", 0);

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

	void ManipulateEntity::setChangeColor(const QVector4D& color)
	{
		m_changeColorParameter->setValue(color);
	}

	void ManipulateEntity::setMethod(int mt)
	{
		m_methodParameter->setValue(mt);
	}
}
