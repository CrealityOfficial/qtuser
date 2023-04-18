#include "xrenderpass.h"
#include "qtuser3d/utils/shaderprogrammanager.h"

#include <QtCore/QDebug>

namespace qtuser_3d
{
	XRenderPass::XRenderPass(const QString& shaderName, Qt3DCore::QNode* parent)
		: QRenderPass(parent)
	{
		Qt3DRender::QShaderProgram* program = CREATE_SHADER(shaderName);

		if(program)
			setShaderProgram(program);

		if (!program)
			qDebug() << QString("XRenderPass load shader [%1] error.").arg(shaderName);
	}

	XRenderPass::~XRenderPass()
	{

	}

	Qt3DRender::QParameter* XRenderPass::setParameter(const QString& name, const QVariant& value)
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

	void XRenderPass::addFilterKeyMask(const QString& key, int mask)
	{
		Qt3DRender::QFilterKey* filterKey = new Qt3DRender::QFilterKey(this);
		filterKey->setName(key);
		filterKey->setValue(mask);
		addFilterKey(filterKey);
	}
}