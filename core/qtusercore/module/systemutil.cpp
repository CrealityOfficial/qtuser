#include "systemutil.h"

#include <QSysInfo>
#include <QDebug>


#ifdef _WINDOWS

#include <Windows.h>
#include <psapi.h>

#pragma comment(lib, "psapi.lib")

#endif


SystemUtil::SystemUtil()
	: m_systemType(UNKNOWN)
{
	QString system_type = QSysInfo::productType();
	system_type = system_type.toLower();
	if (system_type.indexOf("windows") >= 0)
	{
		m_systemType = WINDOWS;
	}
}


SystemUtil::SYSTEM_TYPE SystemUtil::getSystemType()
{
	return m_systemType;
}

void SystemUtil::showDetailSystemInfo()
{
	qDebug() << "buildAbi: " << QSysInfo::buildAbi();
	qDebug() << "buildCpuArchitecture: " << QSysInfo::buildCpuArchitecture();
	qDebug() << "currentCpuArchitecture: " << QSysInfo::currentCpuArchitecture();
	qDebug() << "kernelType: " << QSysInfo::kernelType();
	qDebug() << "kernelVersion: " << QSysInfo::kernelVersion();
	qDebug() << "machineHostName: " << QSysInfo::machineHostName();
	qDebug() << "prettyProductName: " << QSysInfo::prettyProductName();
	qDebug() << "productType: " << QSysInfo::productType();
	qDebug() << "productVersion: " << QSysInfo::productVersion();
	if (m_systemType == WINDOWS)
	{
		qDebug() << "Windows Version: " << QSysInfo::windowsVersion();
	}
	else if (m_systemType == LINUX)
	{
		//
	}
	else if (m_systemType == MAC)
	{
		qDebug() << "Mac Version: " << QSysInfo::macVersion();
	}

}

#ifdef _WINDOWS

void SystemUtil::showSysMemory()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));

	int msize = 1024 * 1024;

	qDebug() << "memory use: " << pmc.WorkingSetSize / msize << "M/" << pmc.PeakWorkingSetSize / msize << "M + "
		<< pmc.PagefileUsage / msize << "M/" << pmc.PeakPagefileUsage / msize << "M";

}

#else

void SystemUtil::showSysMemory()
{
}

#endif


QTUSER_CORE_API SystemUtil gblSystemUtil;

