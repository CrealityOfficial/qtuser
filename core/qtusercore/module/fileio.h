#ifndef _CREATIVE_KERNEL_FILEIO_1590379536521_H
#define _CREATIVE_KERNEL_FILEIO_1590379536521_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>

namespace qtuser_core
{
	QTUSER_CORE_API void writeTextFile(const QString& fileName, const QString& content);
	QTUSER_CORE_API void writeTextFile(const QString& fileName, const QByteArray& content);
}
#endif // _CREATIVE_KERNEL_FILEIO_1590379536521_H
