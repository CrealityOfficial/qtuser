#include "pickfaceentity.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/effect/ueffect.h"
#include <Qt3DRender/QBuffer>

#include <Qt3DRender/QBlendEquationArguments>
#include <Qt3DRender/QDepthTest>
#include <Qt3DRender/QNoDepthMask>
#include <Qt3DRender/QRenderPass>

namespace qtuser_3d
{
	PickFaceEntity::PickFaceEntity(Qt3DCore::QNode* parent)
		:BasicEntity(parent)
	{
		m_stateParameter = createParameter("state", 0.0f);
		m_colorParameter = createParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 0.2f));
		m_vertexBaseParameter = createParameter("vertexBase", QPoint(0, 0));
		qtuser_3d::UEffect* effect = (qtuser_3d::UEffect*)EFFECTCREATE("pure.alpha.rt_pickFace.pick", this);
		effect->setPassBlend("pure.alpha.rt");
		effect->setPassCullFace("pure.alpha.rt");

		Qt3DRender::QNoDepthMask* mask = new Qt3DRender::QNoDepthMask(this);
		QList<Qt3DRender::QRenderPass*> passes
			= effect->findChildren<Qt3DRender::QRenderPass*>(QString(), Qt::FindChildrenRecursively);
		if (passes.size() > 0)
		{
			passes.at(0)->addRenderState(mask);
		}
		setEffect(effect);
		setObjectName("PickFaceEntity");

	}

	PickFaceEntity::~PickFaceEntity()
	{

	}

	void PickFaceEntity::updateData(const std::vector<QVector3D>& vertexData)
	{
		if (vertexData.size() > 0)
		{
			m_geometry = TrianglesCreateHelper::create((int)vertexData.size(), (float*)&vertexData.at(0), nullptr, nullptr, 0, nullptr);
			setGeometry(m_geometry);
		}
	}


	void PickFaceEntity::setColor(const QVector4D& color)
	{
		m_colorParameter->setValue(color);
	}

	void PickFaceEntity::setState(float state)
	{
		m_stateParameter->setValue(state);
	}

	void PickFaceEntity::setVertexBase(QPoint vertexBase)
	{
		m_vertexBaseParameter->setValue(vertexBase);
	}

}
