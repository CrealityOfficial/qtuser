#ifndef QTUSER_3D_SCENEOPERATEMODE_1592203699385_H
#define QTUSER_3D_SCENEOPERATEMODE_1592203699385_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/event/eventhandlers.h"

namespace qtuser_3d
{
	class QTUSER_3D_API SceneOperateMode : public QObject
		, public qtuser_3d::EventCheckHandler
	{
		Q_OBJECT
	public:
		SceneOperateMode(QObject* parent = nullptr);
		virtual ~SceneOperateMode();

		virtual void onAttach() = 0;
		virtual void onDettach() = 0;
	};
}
#endif // QTUSER_3D_SCENEOPERATEMODE_1592203699385_H