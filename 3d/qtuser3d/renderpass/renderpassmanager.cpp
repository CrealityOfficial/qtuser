#include "renderpassmanager.h"
#include "qtuser3d/utils/shaderprogrammanager.h"

#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QMultiSampleAntiAliasing>
#include <Qt3DRender/QBlendEquationArguments>
#include <Qt3DRender/QDithering>
#include <Qt3DRender/QFrontFace>

namespace qtuser_3d
{
	SINGLETON_IMPL(RenderPassManager)
	RenderPassManager::RenderPassManager()
		:QObject()
		,m_root(nullptr)
	{
		m_root = new Qt3DCore::QNode();
	}
	
	RenderPassManager::~RenderPassManager()
	{
		delete m_root;
	}

	Qt3DCore::QNode* RenderPassManager::root()
	{
		return m_root;
	}

	Qt3DRender::QRenderPass* RenderPassManager::create(const QString& name, Qt3DCore::QNode* parent)
	{
		Qt3DRender::QRenderPass* pass = new Qt3DRender::QRenderPass(parent);

		QString filterName("view");
		QString shaderName = name;
		QStringList passSegs = name.split(".");
		if (passSegs.size() == 1)
		{
			shaderName = passSegs.at(0);
			Qt3DRender::QFilterKey* filterKey = new Qt3DRender::QFilterKey(pass);
			filterKey->setName(filterName);
			filterKey->setValue(0);
			pass->addFilterKey(filterKey);
		}
		else if (passSegs.size() > 1)
		{
			shaderName = passSegs.at(0);
			for (int i = 1; i < passSegs.size(); ++i)
			{
				Qt3DRender::QFilterKey* filterKey = new Qt3DRender::QFilterKey(pass);
				filterKey->setName(passSegs.at(i));
				filterKey->setValue(0);
				pass->addFilterKey(filterKey);
			}
		}
		pass->setObjectName(shaderName);

		//add render state
		//Qt3DRender::QCullFace* cullFace = new Qt3DRender::QCullFace(pass);
		//cullFace->setMode(Qt3DRender::QCullFace::NoCulling);
		//pass->addRenderState(cullFace);

		Qt3DRender::QBlendEquationArguments* blendArguments = new Qt3DRender::QBlendEquationArguments(pass);
		blendArguments->setSourceRgba(Qt3DRender::QBlendEquationArguments::One);
		blendArguments->setDestinationRgba(Qt3DRender::QBlendEquationArguments::Zero);
		pass->addRenderState(blendArguments);

		//Qt3DRender::QMultiSampleAntiAliasing* antiAliasing = new Qt3DRender::QMultiSampleAntiAliasing(pass);
		//pass->addRenderState(antiAliasing);

		//Qt3DRender::QDithering* dithering = new Qt3DRender::QDithering(pass);
		//pass->addRenderState(dithering);

		Qt3DRender::QFrontFace* frontface = new Qt3DRender::QFrontFace(pass);
		frontface->setDirection(Qt3DRender::QFrontFace::CounterClockWise);
		pass->addRenderState(frontface);

		Qt3DRender::QDepthTest* depthTest = new Qt3DRender::QDepthTest(pass);
		depthTest->setDepthFunction(Qt3DRender::QDepthTest::Less);

		pass->addRenderState(depthTest);

		pass->setShaderProgram(SHADERS(shaderName));
		return pass;
	}

	Qt3DRender::QRenderPass* RenderPassManager::renderPass(const QString& name)
	{
		QMap<QString, Qt3DRender::QRenderPass*>::iterator it = m_renderPasses.find(name);
		if (it != m_renderPasses.end()) return (*it);

		Qt3DRender::QRenderPass* pass = create(name, m_root);
		if (pass)
		{
			it = m_renderPasses.insert(name, pass);
			if (it == m_renderPasses.end())
			{//insert error
				delete pass;
				pass = nullptr;
			}
		}
		return pass;
	}
}