#include "faces.h"
#include "qtuser3d/effect/effectmanager.h"

#include <Qt3DRender/QBlendEquationArguments>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QNoDepthMask>
#include <Qt3DRender/QRenderPass>
namespace qtuser_3d
{
	Faces::Faces(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_colorParameter = createParameter("color", QVector4D(0.180f, 0.541f, 0.968f, 0.3f));
		Qt3DRender::QEffect* effect = EFFECTCREATE("pure", m_material);

		Qt3DRender::QBlendEquationArguments* blend = blendArguments();
		if (blend)
		{
			blend->setSourceRgba(Qt3DRender::QBlendEquationArguments::SourceAlpha);
			blend->setDestinationRgba(Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
		}


		Qt3DRender::QNoDepthMask* mask = new Qt3DRender::QNoDepthMask(this);
		QList<Qt3DRender::QRenderPass*> passes
			= effect->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
		if (passes.size() > 0)
		{
			passes.at(0)->addRenderState(mask);
		}
		setEffect(effect);
	}
	
	Faces::~Faces()
	{
	}

	void Faces::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}
}