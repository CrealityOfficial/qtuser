#include "qtuserqml/main/renderinterface.h"

#include "qtuserqml/gl/glquickitem.h"

#include "qtuserqml/main/rendercenter.h"

void registerResidentNode(Qt3DCore::QNode* node)
{
	getRenderCenter()->registerResidentNode(node);
}

void unRegisterResidentNode(Qt3DCore::QNode* node)
{
	getRenderCenter()->unRegisterResidentNode(node);
}

void renderRenderGraph(qtuser_3d::RenderGraph* graph)
{
	getRenderCenter()->renderRenderGraph(graph);
}

void registerRenderGraph(qtuser_3d::RenderGraph* graph)
{
	getRenderCenter()->registerRenderGraph(graph);
}

void renderOneFrame()
{
	getRenderCenter()->renderOneFrame();
}

void setContinousRender()
{
	getRenderCenter()->glQuickItem()->setAlways(true);
}

void setCommandRender()
{
	getRenderCenter()->glQuickItem()->setAlways(false);
}