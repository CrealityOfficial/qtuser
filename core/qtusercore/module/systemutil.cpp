#include "systemutil.h"

#include <QtCore/QOperatingSystemVersion>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QCoreApplication>
#include <QtCore/QStandardPaths>
#include <QtCore/QDateTime>
#include <QSettings>

#include "qtusercore/string/resourcesfinder.h"
#include "ccglobal/log.h"
#include"../../buildinfo.h"

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

		printf("%i: %s - 0x%0X\n", frames - i - 1, symbol->Name, symbol->Address);
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

QString mkMutiDir(const QString path)
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

/**
* 自动对区分不同区域的可获取的网络地址
* key, 网络地址类型
* return:   返回网络地址
*/
QString getUrlAddress(QString type)
{
	QSettings setting;
	setting.beginGroup("language_perfer_config");
	QString strLanguageType = setting.value("language_type", "en.ts").toString();
	setting.endGroup();

	QString qurl;
	const char* url = nullptr;

	bool isCN = false;
	if (strLanguageType == "zh_CN.ts")
	{
		isCN = true;
	}

	if (type == "modelStore")
	{
		if (isCN)
		{
			url = "https://www.crealitycloud.cn/model";
		}
		else
		{
			url = "https://www.crealitycloud.com/model";
		}
	}
	else if (type == "updateVersionCheck")
	{
		if (isCN)
		{
			url = "https://file-cdn.creality.com/ota-sz/";
		}
		else
		{
			url = "https://file2-cdn.creality.com/ota-sz/";
		}
	}
	else if (type == "feedBack")
	{
		if (isCN)
		{
			url = "https://support.qq.com/product/317172";
		}
		else
		{
			url = "https://www.crealitycloud.com/group-detail/61372a382685eb12cd004154";
		}
	}
	else if (type == "homePage")
	{
		if (isCN)
		{
			url = "https://www.cxsw3d.cn/";
		}
		else
		{
			url = "https://www.cxsw3d.com/";
		}
	}

	qurl = QString::fromLatin1(url);
	return qurl;
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
	//source源文件目录路径，destination目的文件目录，override文件存在是否覆盖
	bool copyDir(const QString& source, const QString& destination, bool override)
	{
		QDir directory(source);
		if (!directory.exists())
		{
			qDebug() << "不存在";
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

		cxlog::CXLog::Instance().setNameFunc(func);
		LOGDIR(logDirectory.toLocal8Bit().data());
#else
		cxlog::CXLog::Instance().setColorConsole();
#endif
		LOGLEVEL(1);

		qInstallMessageHandler(outputMessage);

		qDebug() << QString("----------> START LOG <-----------");
	}
	void initializeConfig(int argc, char* argv[])
	{
		QFileInfo info(argv[0]);
		QString name = QString("/%1/sliceconfig/").arg(info.baseName());
		QString configDirectory = qtuser_core::getOrCreateAppDataLocation(name);
		qDebug() << configDirectory;

		//copy config 存在不拷贝
		QDir directory(configDirectory+"default/");
		if (directory.exists())
		{
			return;
		}
#ifdef QT_NO_DEBUG

		#if defined (__APPLE__)
				int index = QCoreApplication::applicationDirPath().lastIndexOf("/");
				QFileInfo info(QCoreApplication::applicationDirPath().left(index) + "/Resources/resources/sliceconfig/");
		#else
				QFileInfo info(QCoreApplication::applicationDirPath() + "/resources/sliceconfig/");
		#endif
				QString srcDir = info.path() + "/default/";
				copyDir(srcDir, configDirectory, true);
#else
		QString workPath = QString(SOURCE_ROOT) + "/resources/sliceconfig";
		copyDir(workPath, configDirectory, true);
#endif
	}
	void uninitializeLog()
	{
		qDebug() << QString("----------> END LOG <-----------");
		cxlog::CXLog::Instance().EndLog();
	}
}