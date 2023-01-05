#include "rendergraph.h"
#include "qtuser3d/scene/sceneoperatemode.h"

namespace qtuser_3d
{
	RenderGraph::RenderGraph(Qt3DCore::QNode* parent)
		:Qt3DCore::QNode(parent)
		, m_operateMode(nullptr)
	{
	}
	
	RenderGraph::~RenderGraph()
	{
	}

	Qt3DCore::QEntity* RenderGraph::sceneGraph()
	{
		return nullptr;
	}

	Qt3DRender::QFrameGraphNode* RenderGraph::frameGraph()
	{
		return nullptr;
	}

	void RenderGraph::begineRender()
	{

	}

	void RenderGraph::endRender()
	{

	}

	void RenderGraph::updateRenderSize(QSize size)
	{

	}

	void RenderGraph::onRegistered()
	{

	}

	void RenderGraph::onUnRegistered()
	{

	}

	void RenderGraph::setOperateMode(SceneOperateMode* operateMode)
	{
		if (m_operateMode == operateMode)
			return;

		if (m_operateMode)
		{
			m_operateMode->onDettach();
		}

		m_operateMode = operateMode;

		if (m_operateMode)
		{
			m_operateMode->onAttach();
		}
	}

	void RenderGraph::reinOperationMode()
	{
		if (m_operateMode)
		{
			m_operateMode->onDettach();
			m_operateMode->onAttach();
		}
	}
}

