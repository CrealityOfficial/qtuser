#include "xeffect.h"
#include "qtuser3d/utils/techniquecreator.h"

namespace qtuser_3d
{
	XEffect::XEffect(Qt3DCore::QNode* parent)
		: QEffect(parent)
	{
		m_technique = qtuser_3d::TechniqueCreator::createOpenGLBase(this);
		addTechnique(m_technique);
	}

	XEffect::~XEffect()
	{

	}

	Qt3DRender::QParameter* XEffect::setParameter(const QString& name, const QVariant& value)
	{
		Qt3DRender::QParameter* parameter = nullptr;
		QVector<Qt3DRender::QParameter*> parameters = this->parameters();

		for (Qt3DRender::QParameter* param : parameters)
		{
			if (name == param->name())
			{
				param->setValue(value);
				parameter = param;
				break;
			}
		}

		if (!parameter)
		{
			parameter = new Qt3DRender::QParameter(name, value, this);
			addParameter(parameter);
		}

		return parameter;
	}

	void XEffect::addRenderPass(XRenderPass* pass)
	{
		if(pass)
			m_technique->addRenderPass(pass);
	}

	int XEffect::renderPassCount()
	{
		return m_technique->renderPasses().count();
	}

	XRenderPass* XEffect::findRenderPass(int index)
	{
		if (index < 0 || index >= renderPassCount())
		{
			qDebug() << QString("XEffect::findRenderPass invalid index [%1]").arg(index);
			return nullptr;
		}

		XRenderPass* pass = qobject_cast<XRenderPass*>(m_technique->renderPasses().at(index));

		if (!pass)
		{
			qDebug() << QString("qobject_cast<XRenderPass*> error. not XRenderPass. ");
			return nullptr;
		}

		return pass;
	}

	void XEffect::addRenderState(int index, Qt3DRender::QRenderState* state)
	{
		XRenderPass* pass = findRenderPass(index);
		if (!pass)
			return;

		pass->addRenderState(state);
	}

	void XEffect::addPassFilter(int index, const QString& filter)
	{
		XRenderPass* pass = findRenderPass(index);
		if (!pass)
			return;

		Qt3DRender::QFilterKey* filterKey = new Qt3DRender::QFilterKey();
		filterKey->setName(filter);
		filterKey->setValue(0);
		pass->addFilterKey(filterKey);
	}

	void XEffect::setPassCullFace(int index, Qt3DRender::QCullFace::CullingMode cullingMode)
	{
		XRenderPass* pass = findRenderPass(index);
		if (pass)
		{
			//add render state
			Qt3DRender::QCullFace* cullFace = new Qt3DRender::QCullFace(pass);
			cullFace->setMode(cullingMode);
			pass->addRenderState(cullFace);
		}
	}

	void XEffect::setPassBlend(int index, Qt3DRender::QBlendEquationArguments::Blending source,
		Qt3DRender::QBlendEquationArguments::Blending destination)
	{
		XRenderPass* pass = findRenderPass(index);
		if (pass)
		{
			Qt3DRender::QBlendEquationArguments* blend = new Qt3DRender::QBlendEquationArguments(pass);
			blend->setSourceRgba(source);
			blend->setDestinationRgba(destination);
			pass->addRenderState(blend);
		}
	}

	void XEffect::setPassStencilMask(int index, int mask)
	{
		XRenderPass* pass = findRenderPass(index);
		if (pass)
		{
			Qt3DRender::QStencilMask* maskItem = new Qt3DRender::QStencilMask(pass);
			maskItem->setFrontOutputMask(mask);
			pass->addRenderState(maskItem);
		}
	}

	void XEffect::setPassStencilOperation(int index,
		Qt3DRender::QStencilOperationArguments::Operation depthFail,
		Qt3DRender::QStencilOperationArguments::Operation stencilFail,
		Qt3DRender::QStencilOperationArguments::Operation allPass)
	{
		XRenderPass* pass = findRenderPass(index);
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


	void XEffect::setPassStencilFunction(int index, Qt3DRender::QStencilTestArguments::StencilFunction func, int reference, int comparisonMask)
	{
		XRenderPass* pass = findRenderPass(index);
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

	void XEffect::setPassDepthTest(int index, Qt3DRender::QDepthTest::DepthFunction depthFunc)
	{
		XRenderPass* pass = findRenderPass(index);
		if (pass)
		{
			Qt3DRender::QDepthTest* state = new Qt3DRender::QDepthTest(pass);
			state->setDepthFunction(depthFunc);

			pass->addRenderState(state);
		}
	}

	void XEffect::setPassNoDepthMask(int index)
	{
		XRenderPass* pass = findRenderPass(index);
		if (pass)
		{
			Qt3DRender::QNoDepthMask* state = new Qt3DRender::QNoDepthMask(pass);
			pass->addRenderState(state);
		}
	}
}