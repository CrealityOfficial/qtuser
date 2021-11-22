#ifndef QTUSER_CORE_SYSTEM_UTIL_H
#define QTUSER_CORE_SYSTEM_UTIL_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QString>

QTUSER_CORE_API void initSystemUtil();
QTUSER_CORE_API void showDetailSystemInfo();
QTUSER_CORE_API void showSysMemory();

QTUSER_CORE_API void printCallStack();

QTUSER_CORE_API QString getCanWriteFolder();

QTUSER_CORE_API void redirectIo();
QTUSER_CORE_API QString mkMutiDir(const QString path);
QTUSER_CORE_API QString getUrlAddress(QString type);
#endif // creative_kernel_MATRIX_UTIL_H