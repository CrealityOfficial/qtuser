#ifndef QTUSER_QUICK_QUICKNATIVERENDERSYSTEM_1651426521478_H
#define QTUSER_QUICK_QUICKNATIVERENDERSYSTEM_1651426521478_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>
#include <QtCore/QSize>

namespace qtuser_core
{
	class QuickNativeUpdater
	{
	public:
		virtual ~QuickNativeUpdater() {}
		virtual void invokeUpdate() = 0;   //from gui thread
	};

	class QTUSER_CORE_API QuickNativeRenderSystem : public QObject
	{
		Q_OBJECT
	public:
		QuickNativeRenderSystem(QObject* parent = nullptr);
		virtual ~QuickNativeRenderSystem() {}

		virtual void initializeFromRenderThread() = 0;
		virtual void unitializeFromRenderThread() = 0;
		virtual bool render() = 0;   // called from render thread, true need update
		virtual void synchronize() = 0;
		virtual void updateContextSize(const QSize& size) = 0;

		void setQuickNativeUpdater(QuickNativeUpdater* updater);
		void invokeUpdate();
	protected:
		QuickNativeUpdater* m_updater;
	};
}

#endif // QTUSER_QUICK_QUICKNATIVERENDERSYSTEM_1651426521478_H