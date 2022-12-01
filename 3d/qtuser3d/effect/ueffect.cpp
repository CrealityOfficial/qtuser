#include "ueffect.h"
#include "qtuser3d/renderpass/renderpassmanager.h"
#include <Qt3DRender/QTechnique>
#include <qstencilmask>
#include <qstenciloperation>
#include <QStencilTest>


namespace qtuser_3d
{
	UEffect::UEffect(Qt3DCore::QNode* parent)
		:QEffect(parent)
	{
	}
	
	UEffect::~UEffect()
	{
	}

	Qt3DRender::QParameter* UEffect::createParameter(const QString& name, const QVariant& value)
	{
		Qt3DRender::QParameter* parameter = new Qt3DRender::QParameter(name, value, this);
		addParameter(parameter);
		return parameter;
	}

	void UEffect::destroyParameter(Qt3DRender::QParameter* parameter)
	{
		if (parameter)
		{
			removeParameter(parameter);
			delete parameter;
		}
	}

	void UEffect::addRenderPass(QString& name)
	{
		Qt3DRender::QRenderPass* renderPass = RENDERPASSCREATE(name, this);
		techniques().at(0)->addRenderPass(renderPass);
		m_attachedPasses.insert(name, renderPass);
	}

	void UEffect::removeRenderPass(QString& name)
	{
		auto it = m_attachedPasses.find(name);
		if (it != m_attachedPasses.end())
		{
			techniques().at(0)->removeRenderPass((*it));
			m_attachedPasses.erase(it);
			delete (*it);
		}
	}

	void UEffect::registerRenderPass(const QString& name, Qt3DRender::QRenderPass* pass)
	{
		if (name.isEmpty() || !pass)
			return;

		m_attachedPasses.insert(name, pass);
	}

	void UEffect::unregisterRenderPass(const QString& name)
	{
		auto it = m_attachedPasses.find(name);
		if (it != m_attachedPasses.end())
			m_attachedPasses.erase(it);
	}

	Qt3DRender::QRenderPass* UEffect::traitRenderPass(const QString& name)
	{
		return m_attachedPasses.value(name);
	}

	void UEffect::setPassCullFace(const QString& passName, Qt3DRender::QCullFace::CullingMode cullingMode)
	{
		Qt3DRender::QRenderPass* pass = traitRenderPass(passName);
		if (pass)
		{
			//add render state
			Qt3DRender::QCullFace* cullFace = new Qt3DRender::QCullFace(pass);
			cullFace->setMode(cullingMode);
			pass->addRenderState(cullFace);
		}
	}

	void UEffect::setPassBlend(const QString& passName, Qt3DRender::QBlendEquationArguments::Blending source,
		Qt3DRender::QBlendEquationArguments::Blending destination)
	{
		Qt3DRender::QRenderPass* pass = traitRenderPass(passName);
		if (pass)
		{
			Qt3DRender::QBlendEquationArguments* blend = new Qt3DRender::QBlendEquationArguments(pass);
			blend->setSourceRgba(source);
			blend->setDestinationRgba(destination);
			pass->addRenderState(blend);
		}
	}

	void UEffect::setPassStencilMask(const QString& passName, int mask)
	{
		Qt3DRender::QRenderPass* pass = traitRenderPass(passName);
		if (pass)
		{
			Qt3DRender::QStencilMask* maskItem = new Qt3DRender::QStencilMask(pass);
			maskItem->setFrontOutputMask(mask);
			pass->addRenderState(maskItem);
		}
	}

	void UEffect::setPassStencilOperation(const QString& passName,
		Qt3DRender::QStencilOperationArguments::Operation depthFail,
		Qt3DRender::QStencilOperationArguments::Operation stencilFail,
		Qt3DRender::QStencilOperationArguments::Operation allPass)
	{
		Qt3DRender::QRenderPass* pass = traitRenderPass(passName);
		if (pass)
		{
			Qt3DRender::QStencilOperation* op = new Qt3DRender::QStencilOperation(pass);
			Qt3DRender::QStencilOperationArguments* face = op->front();

			face->setDepthTestFailureOperation(depthFail);
			face->setStencilTestFailureOperation(stencilFail);
			face->setAllTestsPassOperation(allPass);

			pass->addRenderState(op);
		}
	}


	void UEffect::setPassStencilFunction(const QString& passName, Qt3DRender::QStencilTestArguments::StencilFunction func, int reference, int comparisonMask)
	{
		Qt3DRender::QRenderPass* pass = traitRenderPass(passName);
		if (pass)
		{
			Qt3DRender::QStencilTest* state = new Qt3DRender::QStencilTest(pass);
			Qt3DRender::QStencilTestArguments* arg = state->front();
			arg->setComparisonMask(comparisonMask);
			arg->setReferenceValue(reference);
			arg->setStencilFunction(func);

			pass->addRenderState(state);
		}
	}
}