#include "resourcesfinder.h"
#include "qtusercore/module/systemutil.h"
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>

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
	
	QStringList dynamicLoadFilters(const QString& prefix)
	{
		QStringList filters;
		QString systemPrefix;
		QString postFix;

#ifdef Q_OS_OSX
		systemPrefix = "lib";
		postFix = "*.dylib";
#elif defined Q_OS_WIN32
		postFix = "*.dll";
#elif defined Q_OS_LINUX
		systemPrefix = "lib";
		postFix = "*.so";
#endif
		QString filter = systemPrefix + prefix + postFix;
		filters << filter;
		return filters;
	}

	QString getOrCreateAppDataLocation(const QString& folder)
	{
		QString directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
		QDir dir(directory);
		if (!dir.exists())
			dir.mkdir(directory);

		QString folderDirectory = directory + QString("/") + folder;
		folderDirectory = mkMutiDir(folderDirectory);
		return folderDirectory;
	}
}
