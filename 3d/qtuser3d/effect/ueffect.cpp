#include "ueffect.h"
#include "qtuser3d/renderpass/renderpassmanager.h"
#include <Qt3DRender/QTechnique>

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

	QVector<Qt3DRender::QRenderPass*> UEffect::renderPasses()
	{
		QVector<Qt3DRender::QRenderPass*> passes = techniques()[0]->renderPasses();
		return passes;
	}

	void UEffect::addRenderState(Qt3DRender::QRenderState* state)
	{
		if (!state)
			return;

		QVector<Qt3DRender::QRenderPass*> passes = renderPasses();
		for (Qt3DRender::QRenderPass* pass : passes)
			pass->addRenderState(state);
	}
}