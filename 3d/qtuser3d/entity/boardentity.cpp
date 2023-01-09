#include "qtuser3d/entity/boardentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QTexture>


namespace qtuser_3d
{
	BoardEntity::BoardEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		QString shader_type = "boardentity.alpha";

		qtuser_3d::UEffect* effect = (qtuser_3d::UEffect*)EFFECTCREATE(shader_type, m_material);
		setEffect(effect);

		m_boardTextureParameter = createParameter("boardTexture", QVariant());

		setPassDepthTest("boardentity.alpha", Qt3DRender::QDepthTest::Always);
		setPassCullFace("boardentity.alpha", Qt3DRender::QCullFace::NoCulling);
	}

	BoardEntity::~BoardEntity()
	{

	}

	void BoardEntity::setBoardTexture(Qt3DRender::QTexture2D* texture)
	{
		m_boardTextureParameter->setValue(QVariant::fromValue(texture));
	}
}
