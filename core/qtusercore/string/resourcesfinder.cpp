#include "resourcesfinder.h"

namespace qtuser_core
{
	ResourcesFinder ResourcesFinder::m_resourcesFinder;
	ResourcesFinder::ResourcesFinder(QObject* parent)
		:QObject(parent)
	{
	}

	ResourcesFinder::~ResourcesFinder()
	{
	}

	ResourcesFinder& ResourcesFinder::instance()
	{
		return m_resourcesFinder;
	}

	void ResourcesFinder::addSearchPath(const QString& path)
	{

	}
}
