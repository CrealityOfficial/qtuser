#ifndef QTUSER_QUICK_QUICKNATIVERENDERSYSTEM_1651426521478_H
#define QTUSER_QUICK_QUICKNATIVERENDERSYSTEM_1651426521478_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>
#include <QtCore/QSize>

namespace qtuser_core
{
	class QTUSER_CORE_API QuickNativeRenderSystem : public QObject
	{
		Q_OBJECT
	public:
		QuickNativeRenderSystem(QObject* parent = nullptr);
		virtual ~QuickNativeRenderSystem() {}

		virtual void initializeFromRenderThread() = 0;
		virtual void unitializeFromRenderThread() = 0;
		virtual void render() = 0;   // called from render thread
		virtual void synchronize() = 0;
		virtual void updateContextSize(const QSize& size) = 0;

	signals:
		void signalUpdate();
	};
}

#endif // QTUSER_QUICK_QUICKNATIVERENDERSYSTEM_1651426521478_H