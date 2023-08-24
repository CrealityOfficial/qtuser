#include "pickfacerenderpass.h"

namespace qtuser_3d
{
	PickfaceRenderPass::PickfaceRenderPass(Qt3DCore::QNode* parent)
		: XRenderPass("pickface", parent)
	{
		setPassBlend(Qt3DRender::QBlendEquationArguments::One, Qt3DRender::QBlendEquationArguments::Zero);
		setPassCullFace();
	}

	PickfaceRenderPass::~PickfaceRenderPass()
	{

	}


}