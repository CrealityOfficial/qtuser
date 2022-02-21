#ifndef creative_kernel_COLOR_UTIL_H
#define creative_kernel_COLOR_UTIL_H
#include "qtusercore/qtusercoreexport.h"
#include <QtGui/QVector4D>

namespace qtuser_core
{
	QTUSER_CORE_API QVector4D rgbIntColor2QVector4DColor(int color);
	QTUSER_CORE_API int qvector4DColor2QRgbIntColor(QVector4D color);
}

#endif // creative_kernel_COLOR_UTIL_H