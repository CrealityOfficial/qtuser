#ifndef CREATIVE_KERNEL_RENDERCENTER_1594435187692_H
#define CREATIVE_KERNEL_RENDERCENTER_1594435187692_H
#include "qtusercore/module/singleton.h"
#include <Qt3DCore/QNode>

class GLQuickItem;
namespace qtuser_3d
{
	class EventSubdivide;
	class RenderGraph;
}

class RenderCenter : public QObject
{
	Q_OBJECT
	SINGLETON_DECLARE(RenderCenter)
public:
	virtual ~RenderCenter();

	void setGLQuickItem(GLQuickItem* item);
	GLQuickItem* glQuickItem();

	void uninitialize();

	qtuser_3d::EventSubdivide* eventSubdivide();

	void registerResidentNode(Qt3DCore::QNode* node);
	void unRegisterResidentNode(Qt3DCore::QNode* node);
	void renderRenderGraph(qtuser_3d::RenderGraph* graph);
	void registerRenderGraph(qtuser_3d::RenderGraph* graph);
	void unRegisterRenderGraph(qtuser_3d::RenderGraph* graph);
	void renderOneFrame();
protected:
	GLQuickItem* m_glQuickItem;
	qtuser_3d::EventSubdivide* m_eventSubdivide;
};

SINGLETON_EXPORT(, RenderCenter)
#endif // CREATIVE_KERNEL_RENDERCENTER_1594435187692_H