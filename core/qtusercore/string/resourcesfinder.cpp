#include "resourcesfinder.h"
#include "qtusercore/module/systemutil.h"
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtCore/QCoreApplication>
#include "buildinfo.h"

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

		QString version;
		version.sprintf("%d.%d", PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR);
		QString folderDirectory = directory + QString("/") + version + "/" + folder;
		folderDirectory = mkMutiDir(folderDirectory);
		return folderDirectory;
	}

	QString getResourcesFolder(const QString& folder)
	{
		QString rFolder;
#ifdef Q_OS_OSX
		rFolder = QCoreApplication::applicationDirPath() + "/../Resources/resources/";
#else
		rFolder = QCoreApplication::applicationDirPath() + "/resources/";
#endif

		rFolder += folder;
		return rFolder;
	}

	bool copyDir(const QString& source, const QString& destination, bool override)
	{
		QDir directory(source);
		if (!directory.exists())
		{
			return false;
		}
		QString srcPath = QDir::toNativeSeparators(source);
		if (!srcPath.endsWith(QDir::separator()))
			srcPath += QDir::separator();
		QString dstPath = QDir::toNativeSeparators(destination);
		if (!dstPath.endsWith(QDir::separator()))
			dstPath += QDir::separator();
		bool error = false;
		QStringList fileNames = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
		for (QStringList::size_type i = 0; i != fileNames.size(); ++i)
		{
			QString fileName = fileNames.at(i);
			QString srcFilePath = srcPath + fileName;
			QString dstFilePath = dstPath + fileName;
			QFileInfo fileInfo(srcFilePath);
			if (fileInfo.isFile() || fileInfo.isSymLink())
			{
				if (override)
				{
					QFile::setPermissions(dstFilePath, QFile::WriteOwner);
				}
				QFile::copy(srcFilePath, dstFilePath);
			}
			else if (fileInfo.isDir())
			{
				QDir dstDir(dstFilePath);
				dstDir.mkpath(dstFilePath);
				if (!copyDir(srcFilePath, dstFilePath, override))
				{
					error = true;
				}
			}
		}
		return !error;
	}

	bool copyFileToPath(const QString& sourceDir, const QString& toDir, bool deleteFileIfExist)
	{
		QDir m_createfile;
		// toDir.replace("\\","/");
		if (sourceDir == toDir)
		{
			return true;
		}

		if (!QFile::exists(sourceDir))
		{
			return false;
		}

		bool exist = m_createfile.exists(toDir);
		if (exist)
		{
			if (deleteFileIfExist)
			{
				m_createfile.remove(toDir);
			}
		}

		if (!QFile::copy(sourceDir, toDir))
		{
			return false;
		}
		return true;
	}
}
