#ifndef QTUSER_CORE_SYSTEM_UTIL_H
#define QTUSER_CORE_SYSTEM_UTIL_H

#include "qtusercore/qtusercoreexport.h"
#include <QString>

class QTUSER_CORE_API SystemUtil
{
public:
	enum SYSTEM_TYPE
	{
		WINDOWS = 1,
		LINUX,
		MAC,

		UNKNOWN = 100
	};

protected:
	SYSTEM_TYPE m_systemType;

public:
	SystemUtil();

	SYSTEM_TYPE getSystemType();

	void showDetailSystemInfo();

	void showSysMemory();

};


QTUSER_CORE_API extern SystemUtil gblSystemUtil;



#endif // creative_kernel_MATRIX_UTIL_H