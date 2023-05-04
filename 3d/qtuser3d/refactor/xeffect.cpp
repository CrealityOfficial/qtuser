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

	void XEffect::addRenderState(int index, Qt3DRender::QRenderState* state)
	{
		if (index < 0 || index >= renderPassCount())
		{
			qDebug() << QString("XEffect::addRenderState invalid index [%1]").arg(index);
			return;
		}

		m_technique->renderPasses().at(index)->addRenderState(state);
	}
}