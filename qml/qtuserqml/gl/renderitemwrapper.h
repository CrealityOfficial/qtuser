#ifndef QTUSER_QML_RENDERITEMWRAPPER_1679650158559_H
#define QTUSER_QML_RENDERITEMWRAPPER_1679650158559_H
#include "qtuserqml/gl/glquickitem.h"
#include "qtuser3d/event/eventhandlers.h"

namespace qtuser_qml
{
	class QTUSER_QML_API RenderItemWrapper : public QObject
	{
	public:
		RenderItemWrapper(GLQuickItem* item, QObject* parent = nullptr);
		virtual ~RenderItemWrapper();

		void uninitialize();

		void registerResidentNode(Qt3DCore::QNode* node);
		void unRegisterResidentNode(Qt3DCore::QNode* node);
		void renderRenderGraph(qtuser_3d::RenderGraph* graph);
		void registerRenderGraph(qtuser_3d::RenderGraph* graph);
		void unRegisterRenderGraph(qtuser_3d::RenderGraph* graph);
		void renderOneFrame();

		void addResizeEventHandler(qtuser_3d::ResizeEventHandler* handler);
		void removeResizeEventHandler(qtuser_3d::ResizeEventHandler* handler);
		void closeResizeEventHandlers();

		void addHoverEventHandler(qtuser_3d::HoverEventHandler* handler);
		void removeHoverEventHandler(qtuser_3d::HoverEventHandler* handler);
		void closeHoverEventHandlers();

		void addWheelEventHandler(qtuser_3d::WheelEventHandler* handler);
		void removeWheelEventHandler(qtuser_3d::WheelEventHandler* handler);
		void closeWheelEventHandlers();

		void addRightMouseEventHandler(qtuser_3d::RightMouseEventHandler* handler);
		void removeRightMouseEventHandler(qtuser_3d::RightMouseEventHandler* handler);
		void closeRightMouseEventHandlers();

		void addMidMouseEventHandler(qtuser_3d::MidMouseEventHandler* handler);
		void removeMidMouseEventHandler(qtuser_3d::MidMouseEventHandler* handler);
		void closeMidMouseEventHandlers();

		void addLeftMouseEventHandler(qtuser_3d::LeftMouseEventHandler* handler);
		void removeLeftMouseEventHandler(qtuser_3d::LeftMouseEventHandler* handler);
		void closeLeftMouseEventHandlers();

		void addKeyEventHandler(qtuser_3d::KeyEventHandler* handler);
		void removeKeyEventHandler(qtuser_3d::KeyEventHandler* handler);
		void closeKeyEventHandlers();

		GLQuickItem* glQuickItem();

	protected:
		GLQuickItem* m_item;
	};
}

#endif // QTUSER_QML_RENDERITEMWRAPPER_1679650158559_H