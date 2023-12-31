﻿#include "systemutil.h"

#include <QtCore/QOperatingSystemVersion>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include <QtCore/QStandardPaths>
#include <QtGui/QSurface>
#include <QtGui/QSurfaceFormat>
#include <QtCore/QDateTime>
#include <QSettings>
#include <QtCore/QCryptographicHash>
#include <QtCore/QMimeData>
#include <QtGui/QClipboard>

#include "qtusercore/module/glcompatibility.h"
#include "qtusercore/string/resourcesfinder.h"
#include "ccglobal/log.h"

#ifdef _WINDOWS

#include <Windows.h>
#include <psapi.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>

#include <DbgHelp.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "dbghelp.lib")

#elif defined(__linux__)
#include <linux/limits.h>
#elif defined(__APPLE__)
#include <sys/syslimits.h>
#endif
#include "buildinfo.h"
#include "qtusercore/string/resourcesfinder.h"

#define DEBUG_FUNCTION 0

enum SYSTEM_TYPE
{
	WINDOWS = 1,
	LINUX,
	MAC,

	UNKNOWN = 100
};

SYSTEM_TYPE gSystemType = UNKNOWN;

void initSystemUtil()
{
	QString system_type = QSysInfo::productType();
	system_type = system_type.toLower();
	if (system_type.indexOf("windows") >= 0)
	{
		gSystemType = WINDOWS;
	}
}

void showDetailSystemInfo()
{
	qDebug() << "--------------------------------showDetailSystemInfo----------------------------";
	qDebug() << "buildAbi: " << QSysInfo::buildAbi();
	qDebug() << "buildCpuArchitecture: " << QSysInfo::buildCpuArchitecture();
	qDebug() << "currentCpuArchitecture: " << QSysInfo::currentCpuArchitecture();
	qDebug() << "kernelType: " << QSysInfo::kernelType();
	qDebug() << "kernelVersion: " << QSysInfo::kernelVersion();
	qDebug() << "machineHostName: " << QSysInfo::machineHostName();
	qDebug() << "prettyProductName: " << QSysInfo::prettyProductName();
	qDebug() << "productType: " << QSysInfo::productType();
	qDebug() << "productVersion: " << QSysInfo::productVersion();

	QOperatingSystemVersion version = QOperatingSystemVersion::current();
	qDebug() << version.name() << version.majorVersion() << version.minorVersion() << version.microVersion();
	showSysMemory();

	QString appLocation = qtuser_core::getOrCreateAppDataLocation();
	qDebug() << "WriteAppData: " << appLocation;
	qDebug() << "--------------------------------showDetailSystemInfo----------------------------";
}


void showSysMemory()
{
#ifdef _WINDOWS
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	int msize = 1024 * 1024;

	qDebug() << "Memory Use: " << pmc.WorkingSetSize / msize << "M/" << pmc.PeakWorkingSetSize / msize << "M + "
		<< pmc.PagefileUsage / msize << "M/" << pmc.PeakPagefileUsage / msize << "M";

#else

#endif
}

int currentProcessMemory()
{
	int memory = 0;
#ifdef _WINDOWS
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	int msize = 1024;
	memory = pmc.WorkingSetSize / msize;
#else

#endif
	return memory;
}

void printCallStack()
{
#ifdef _WINDOWS
	unsigned int   i;
	void* stack[100];
	unsigned short frames;
	SYMBOL_INFO* symbol;
	HANDLE         process;

	process = GetCurrentProcess();

	SymInitialize(process, NULL, TRUE);

	frames = CaptureStackBackTrace(0, 100, stack, NULL);
	symbol = (SYMBOL_INFO*)calloc(sizeof(SYMBOL_INFO) + 256 * sizeof(char), 1);
	symbol->MaxNameLen = 255;
	symbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	for (i = 0; i < frames; i++)
	{
		SymFromAddr(process, (DWORD64)(stack[i]), 0, symbol);

		printf("%d: %s - 0x%zd\n", (int)(frames - i - 1), symbol->Name, (size_t)symbol->Address); 
	}

	free(symbol);
#else

#endif
}

QString getCanWriteFolder()
{
	return qtuser_core::getOrCreateAppDataLocation("");
}

void redirectIo()
{
#if defined(WIN32) && defined(_DEBUG)
	HANDLE hStdHandle;
	int fdConsole;
	FILE* fp;
	AllocConsole();
	hStdHandle = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);
	fdConsole = _open_osfhandle((intptr_t)hStdHandle, _O_TEXT);
	fp = _fdopen(fdConsole, "w");
	*stdout = *fp;
	setvbuf(stdout, NULL, _IONBF, 0);
	std::ios::sync_with_stdio();
#endif
}

QString mkMutiDir(const QString& path)
{
	QDir dir(path);
	if (dir.exists(path) || path.isEmpty()) {
		return path;
	}
	QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));
	QString dirName = path.mid(path.lastIndexOf('/') + 1);
	QDir parentPath(parentDir);
	if (!dirName.isEmpty())
		parentPath.mkpath(dirName);
	return parentDir + "/" + dirName;
}

void mkMutiDirFromFileName(const QString& fileName)
{
	if (fileName.isEmpty())
		return;

	int index = fileName.lastIndexOf('/');
	if (index >= 0)
	{
		QString path = fileName.mid(0, index);
		mkMutiDir(path);
	}
}

bool clearPath(const QString& path)
{
	if (path.isEmpty())
		return false;

	QDir dir(path);
	if (!dir.exists())
		return false;

	dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	QFileInfoList fileList = dir.entryInfoList();

	for(const QFileInfo& file : fileList)
	{
		if (file.isFile())
		{
			file.dir().remove(file.fileName());
		}
		else
		{
			clearPath(file.absoluteFilePath());
			file.dir().rmdir(file.absoluteFilePath());
		}
	}

	return true;
}

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
#ifdef QT_NO_DEBUG
	QString text = QString("[%3]").arg(msg);
#else
	#if DEBUG_FUNCTION
		QString text = QString("[FILE %1, FUNCTION %2][%3]").arg(context.file).arg(context.function).arg(msg);
	#else
		QString text = QString("[%3]").arg(msg);
	#endif
#endif
	switch (type)//log 信息类型
	{
	case QtDebugMsg:
		LOGD("%s", text.toLocal8Bit().constData());
		break;
	case QtInfoMsg:
		LOGI("%s", text.toLocal8Bit().constData());
		break;
	case QtWarningMsg:
		LOGW("%s", text.toLocal8Bit().constData());
		break;
	case QtCriticalMsg:
		LOGM("%s", text.toLocal8Bit().constData());
		break;
	case QtFatalMsg:
		LOGC("%s", text.toLocal8Bit().constData());
		break;
	default:
		LOGV("%s", text.toLocal8Bit().constData());
		break;
	}
}

namespace qtuser_core
{
	void initializeLog(int argc, char* argv[])
	{
#ifdef QT_NO_DEBUG

		qApp->setOrganizationName(ORGANIZATION);
		qApp->setOrganizationDomain("FDM");
#ifdef Q_OS_LINUX
		qApp->setApplicationName(
			QStringLiteral(BUNDLE_NAME).replace(QStringLiteral("_"), QStringLiteral(" ")));
#else
		qApp->setApplicationName(QStringLiteral(PROJECT_NAME));
#endif

		QString logDirectory = qtuser_core::getOrCreateAppDataLocation("Log");

		auto func = [](const char* name)->std::string {
			QString  dataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd");
			dataTime += QString(".cxlog");
			return dataTime.toLocal8Bit().data();
		};

		LOGNAMEFUNC(func);
		LOGDIR(logDirectory.toLocal8Bit().data());
#else
		LOGCONSOLE();
#endif
		LOGLEVEL(1);

		qInstallMessageHandler(outputMessage);

		qDebug() << QString("----------> START LOG <-----------");
	}

	void uninitializeLog()
	{
		qDebug() << QString("----------> END LOG <-----------");
		LOGEND();
	}
	
	void setDefaultBeforApp()
	{
#ifdef Q_OS_OSX
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

		//dynamic plugin
		QStringList dynamicPathList = QCoreApplication::libraryPaths();

#ifdef Q_OS_OSX
		qDebug() << "OS OSX pre setDynamicLoadPath";
#elif defined Q_OS_WIN32
		qDebug() << "OS WIN32 pre setDynamicLoadPath";
#elif defined Q_OS_LINUX
		qDebug() << "OS LINUX pre setDynamicLoadPath";

		if (qEnvironmentVariableIsSet("APPDIR"))
		{
			QString appdir = qEnvironmentVariable("APPDIR");
			qDebug() << "Linux get the APPDIR : " << appdir;
			dynamicPathList << appdir + "/plugins";
		}
#endif

		qDebug() << "Pre Dynamic import paths:";
		qDebug() << dynamicPathList;
		QCoreApplication::setLibraryPaths(dynamicPathList);
	}

	void setDefaultAfterApp()
	{
#ifdef Q_OS_LINUX
		QSurfaceFormat format;
		format.setVersion(3, 3);
		format.setProfile(QSurfaceFormat::CoreProfile);
		QSurfaceFormat::setDefaultFormat(format);
#endif
#ifdef Q_OS_OSX
		QSurfaceFormat format;
		format.setVersion(3, 3);
		format.setProfile(QSurfaceFormat::CoreProfile);
		format.setDepthBufferSize(24);
		format.setStencilBufferSize(8);
		format.setSamples(4);
		QSurfaceFormat::setDefaultFormat(format);
#endif

		//dynamic plugin
		QStringList dynamicPathList = QCoreApplication::libraryPaths();

		QString applicationDir = QCoreApplication::applicationDirPath();
		qDebug() << "applicationDir: " << applicationDir;

		if(!dynamicPathList.contains(applicationDir))
			dynamicPathList << applicationDir;
#ifdef Q_OS_OSX
		qDebug() << "OS OSX setDynamicLoadPath";
		#ifdef QT_DEBUG
        dynamicPathList << QCoreApplication::applicationDirPath() + "/../../../";
    #endif
		dynamicPathList << QCoreApplication::applicationDirPath() + "/../Frameworks";
#elif defined Q_OS_WIN32
		qDebug() << "OS WIN32 setDynamicLoadPath";
#elif defined Q_OS_LINUX
		qDebug() << "OS LINUX setDynamicLoadPath";
		dynamicPathList << applicationDir + "/lib/";
#endif

		qDebug() << "Dynamic import paths:";
		qDebug() << dynamicPathList;
		QCoreApplication::setLibraryPaths(dynamicPathList);
	}

	void removeFile(const QString& fileName)
	{
		QFile file(fileName);
		if (file.exists())
			file.remove();
	}

	SystemUtil::SystemUtil(QObject* parent)
	{

	}

	SystemUtil::~SystemUtil()
	{

	}

	int SystemUtil::getDiskTotalSpace(const QString& driver)
	{
		return 4096;
	}

	int SystemUtil::getDiskFreeSpace(const QString& driver)
	{
		return 4096;
	}

	QString calculateFileMD5(const QString& fileName)
	{
		QFile file(fileName);
		QString Md5Str = "";
		if (file.open(QIODevice::ReadOnly))
		{
			QByteArray fileArray = file.readAll();
			QByteArray md5 = QCryptographicHash::hash(fileArray, QCryptographicHash::Md5);
			Md5Str = md5.toHex().toUpper();
		}
		else
		{
			qDebug() << QString("calculateFileMD5 openFile [%1] Error.").arg(fileName);
		}
		file.close();
		return Md5Str;
	}
	QString calculateFileBase64(const QString& fileName)
	{
		QFile file(fileName);
		QString strBase64 = "";
		if (file.open(QIODevice::ReadOnly))
		{
			QByteArray fileArray = file.readAll();
			QByteArray md5 = QCryptographicHash::hash(fileArray, QCryptographicHash::Md5);
			strBase64 = md5.toBase64().toLower();
		}
		else
		{
			qDebug() << QString("calculateFileBase64 openFile [%1] Error.").arg(fileName);
		}
		file.close();
		return strBase64;
	}
	//16进制的md5值转化为Base64值
	QString convertBase64(const QString& md5)
	{
		//按照Utf-8编码转换，可以转换中文
		QByteArray fromHexMd5 = QByteArray::fromHex(md5.toUtf8());
		QString contentMD5 = "";
		contentMD5 = fromHexMd5.toBase64();
		return contentMD5;
	}

	void copyString2Clipboard(const QString& str)
	{
		QClipboard* clipboard = QApplication::clipboard();
		QMimeData* mimeData = new QMimeData();
		mimeData->setText(str);
		clipboard->setMimeData(mimeData);
	}

	int getSystemMaxPath()
	{
#ifdef _WINDOWS
		return MAX_PATH;
#else
		return PATH_MAX;
#endif
	}
}