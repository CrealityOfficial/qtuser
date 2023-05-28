#include "nozzleentity.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

#include "qtuser3d/utils/primitiveshapecache.h"

namespace qtuser_3d {
	
	NozzleEntity::NozzleEntity(Qt3DCore::QNode* parent)
		:XEntity(parent)
	{
		setGeometry(PRIMITIVESHAPE("pen"));
		
		XRenderPass* pass = new XRenderPass("modelsimple", this);
		pass->addFilterKeyMask("view", 0);
		pass->setPassBlend(Qt3DRender::QBlendEquationArguments::One, Qt3DRender::QBlendEquationArguments::Zero);
		pass->setPassCullFace(Qt3DRender::QCullFace::NoCulling);

		XEffect* effect = new XEffect(this);
		effect->addRenderPass(pass);
		setEffect(effect);
	}
	
	NozzleEntity::~NozzleEntity()
	{
	}

}