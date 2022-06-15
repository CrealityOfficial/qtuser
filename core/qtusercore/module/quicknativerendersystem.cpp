#include "quicknativerendersystem.h"

namespace qtuser_core
{
	QuickNativeRenderSystem::QuickNativeRenderSystem(QObject* parent)
		:QObject(parent)
		, m_updater(nullptr)
	{
	}

	void QuickNativeRenderSystem::setQuickNativeUpdater(QuickNativeUpdater* updater)
	{
		m_updater = updater;
	}

	void QuickNativeRenderSystem::invokeUpdate()
	{
		if (m_updater)
			m_updater->invokeUpdate();
	}
}