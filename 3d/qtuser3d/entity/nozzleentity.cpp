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

		pass->setParameter("color", QVariant::fromValue(QVector4D(0.8, 0.5, 0.8, 1.0)));
		pass->setParameter("ambient", QVariant::fromValue(QVector4D(0.4, 0.4, 0.4, 1.0)));
		pass->setParameter("diffuse", QVariant::fromValue(QVector4D(0.6, 0.6, 0.6, 1.0)));
		pass->setParameter("specular", QVariant::fromValue(QVector4D(0.125, 0.125, 0.125, 1.0)));
		pass->setParameter("specularPower", QVariant::fromValue(12.0));

	}
	
	NozzleEntity::~NozzleEntity()
	{
	}

}