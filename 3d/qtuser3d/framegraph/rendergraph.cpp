#include "rendergraph.h"
#include "qtuser3d/scene/sceneoperatemode.h"

namespace qtuser_3d
{
	RenderGraph::RenderGraph(Qt3DCore::QNode* parent)
		:Qt3DCore::QNode(parent)
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
}

