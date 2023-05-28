#include "pickfaceentity.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include "qtuser3d/refactor/xeffect.h"

namespace qtuser_3d
{
	PickFaceEntity::PickFaceEntity(Qt3DCore::QNode* parent)
		:XEntity(parent)
	{
		setObjectName("PickFaceEntity");

		m_stateParameter = setParameter("state", 0.0f);
		m_colorParameter = setParameter("color", QVector4D(1.0f, 1.0f, 1.0f, 0.2f));
		m_vertexBaseParameter = setParameter("vertexBase", QPoint(0, 0));

		qtuser_3d::XRenderPass* alphaPass = new qtuser_3d::XRenderPass("pure", this);
		alphaPass->addFilterKeyMask("alpha", 0);
		alphaPass->setPassBlend();
		alphaPass->setPassCullFace();
		alphaPass->setPassNoDepthMask();

		qtuser_3d::XRenderPass* pickPass = new qtuser_3d::XRenderPass("pickFace", this);
		pickPass->addFilterKeyMask("pick", 0);
		pickPass->setPassBlend(Qt3DRender::QBlendEquationArguments::One, Qt3DRender::QBlendEquationArguments::Zero);
		pickPass->setPassCullFace();

		qtuser_3d::XEffect* effect = new qtuser_3d::XEffect(this);
		effect->addRenderPass(alphaPass);
		effect->addRenderPass(pickPass);
		setEffect(effect);
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
