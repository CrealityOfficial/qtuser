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
		Qt3DRender::QEffect* effect = EFFECTCREATE("pure.alpha", m_material);

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