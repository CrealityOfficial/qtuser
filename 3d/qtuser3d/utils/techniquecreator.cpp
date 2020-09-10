#include "techniquecreator.h"

namespace qtuser_3d
{
	TechniqueCreator::TechniqueCreator(QObject* parent)
		:QObject(parent)
	{
	}
	
	TechniqueCreator::~TechniqueCreator()
	{
	}

	Qt3DRender::QTechnique* TechniqueCreator::createOpenGLBase(Qt3DCore::QNode* parent)
	{
		Qt3DRender::QTechnique* technique = new Qt3DRender::QTechnique(parent);
		technique->graphicsApiFilter()->setProfile(Qt3DRender::QGraphicsApiFilter::CoreProfile);
		technique->graphicsApiFilter()->setApi(Qt3DRender::QGraphicsApiFilter::OpenGL);
		technique->graphicsApiFilter()->setMajorVersion(2);
		technique->graphicsApiFilter()->setMinorVersion(1);
		return technique;
	}
}