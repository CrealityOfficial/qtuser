#include "renderitemwrapper.h"
#include "qtuser3d/event/eventsubdivide.h"

#define IMPL_NULL_CHECK(x)  void RenderItemWrapper::add##x(qtuser_3d::x* handler){\
									qtuser_3d::EventSubdivide* divide = m_item->eventSubdivide();\
									if(divide) divide->add##x(handler);} \
void RenderItemWrapper::remove##x(qtuser_3d::x* handler) { \
		qtuser_3d::EventSubdivide* divide = m_item->eventSubdivide(); \
		if (divide) divide->remove##x(handler); }\
void RenderItemWrapper::close##x##s() { \
		qtuser_3d::EventSubdivide* divide = m_item->eventSubdivide(); \
		if (divide) divide->close##x##s(); }

namespace qtuser_qml
{
	RenderItemWrapper::RenderItemWrapper(GLQuickItem* item, QObject* parent)
		: QObject(parent)
		, m_item(item)
	{
		if(m_item)
			m_item->setAlways(true);
	}

	RenderItemWrapper::~RenderItemWrapper()
	{

	}

	void RenderItemWrapper::setItem(GLQuickItem* item)
	{
		m_item = item;
		if (m_item)
			m_item->setAlways(true);
	}

	void RenderItemWrapper::uninitialize()
	{
		if (m_item)
		{
			m_item->eventSubdivide()->closeHandlers();
			m_item->unRegisterAll();
		}
	}

	void RenderItemWrapper::registerResidentNode(Qt3DCore::QNode* node)
	{
		if(m_item)
			m_item->registerResidentNode(node);
	}

	void RenderItemWrapper::unRegisterResidentNode(Qt3DCore::QNode* node)
	{
		if (m_item)
			m_item->unRegisterResidentNode(node);
	}

	void RenderItemWrapper::renderRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_item)
		{
			m_item->renderRenderGraph(graph);
			m_item->requestUpdate();
		}
	}

	void RenderItemWrapper::registerRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_item)
		{
			m_item->registerRenderGraph(graph);
		}
	}

	void RenderItemWrapper::unRegisterRenderGraph(qtuser_3d::RenderGraph* graph)
	{
		if (m_item)
		{
			m_item->unRegisterRenderGraph(graph);
		}
	}

	void RenderItemWrapper::renderOneFrame()
	{
		if (m_item)
		{
			m_item->requestUpdate();
		}
	}

	GLQuickItem* RenderItemWrapper::glQuickItem()
	{
		return m_item;
	}

	IMPL_NULL_CHECK(ResizeEventHandler)
	IMPL_NULL_CHECK(HoverEventHandler)
	IMPL_NULL_CHECK(WheelEventHandler)
	IMPL_NULL_CHECK(RightMouseEventHandler)
	IMPL_NULL_CHECK(MidMouseEventHandler)
	IMPL_NULL_CHECK(LeftMouseEventHandler)
	IMPL_NULL_CHECK(KeyEventHandler)
}