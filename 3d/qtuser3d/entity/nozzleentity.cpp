#include "nozzleentity.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

#include "qtuser3d/utils/primitiveshapecache.h"

namespace qtuser_3d {
	
	NozzleEntity::NozzleEntity(Qt3DCore::QNode* parent)
		:XEntity(parent)
	{
		setGeometry(PRIMITIVESHAPE("pen"));
		
		XRenderPass* pass = new XRenderPass("phong", this);
		pass->addFilterKeyMask("view", 0);
		pass->setPassBlend(Qt3DRender::QBlendEquationArguments::One, Qt3DRender::QBlendEquationArguments::Zero);
		pass->setPassCullFace(Qt3DRender::QCullFace::NoCulling);

		XEffect* effect = new XEffect(this);
		effect->addRenderPass(pass);
		setEffect(effect);

		setParameter("color", QVector4D(0.8f, 0.5f, 0.8f, 1.0f));
	}
	
	NozzleEntity::~NozzleEntity()
	{
	}

}