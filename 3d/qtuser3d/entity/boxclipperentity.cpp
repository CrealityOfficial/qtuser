#include "boxclipperentity.h"

namespace qtuser_3d
{
	BoxClipperEntity::BoxClipperEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_boxClipParameter = new Qt3DRender::QParameter(m_material);
		m_boxClipParameter->setName(QString("clip[0]"));
		m_material->addParameter(m_boxClipParameter);
	}

	BoxClipperEntity::~BoxClipperEntity()
	{

	}

	void BoxClipperEntity::updateClipperBox(const QVector3D& cmin, const QVector3D& cmax)
	{
		QVariantList values;
		values << cmin << cmax;
		m_boxClipParameter->setValue(values);
	}
}