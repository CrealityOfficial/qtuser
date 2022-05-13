#include "systemutil.h"

#include <QtCore/QOperatingSystemVersion>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include <QtCore/QStandardPaths>
#include <QtCore/QDateTime>
#include <QSettings>

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

#endif

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
	qDebug() << "--------------------------------";
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
	qDebug() << "--------------------------------\n";

	QString write_folder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	qDebug() << "write_folder = " << write_folder;
}


void showSysMemory()
{
#ifdef _WINDOWS
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	int msize = 1024 * 1024;

	qDebug() << "memory use: " << pmc.WorkingSetSize / msize << "M/" << pmc.PeakWorkingSetSize / msize << "M + "
		<< pmc.PagefileUsage / msize << "M/" << pmc.PeakPagefileUsage / msize << "M";

#else

#endif
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

		printf("%i: %s - 0x%11X\n", frames - i - 1, symbol->Name, symbol->Address);
	}

	free(symbol);
#else

#endif
}

QString getCanWriteFolder()
{
	return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
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
	if (dir.exists(path)) {
		return path;
	}
	QString parentDir = mkMutiDir(path.mid(0, path.lastIndexOf('/')));
	QString dirname = path.mid(path.lastIndexOf('/') + 1);
	QDir parentPath(parentDir);
	if (!dirname.isEmpty())
		parentPath.mkpath(dirname);
	return parentDir + "/" + dirname;
}

void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
#ifdef QT_NO_DEBUG
	QString text = QString("[%3]").arg(msg);
#else
	QString text = QString("[FILE %1, FUNCTION %2]\n[%3]").arg(context.file).arg(context.function).arg(msg);
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
		QFileInfo info(argv[0]);
		QString name = QString("/%1/Log/").arg(info.baseName());
		QString logDirectory = qtuser_core::getOrCreateAppDataLocation(name);
		qDebug() << logDirectory;

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
#ifdef Q_OS_WIN32
		QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#elif defined Q_OS_OSX
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
#ifdef Q_OS_OSX
		QSurfaceFormat format;
		format.setVersion(3, 2);
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

		dynamicPathList << applicationDir;
#ifdef Q_OS_OSX
		qDebug() << "OS OSX setDynamicLoadPath";
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
}