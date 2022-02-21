#include "colorutil.h"

namespace qtuser_core
{
	QVector4D rgbIntColor2QVector4DColor(int color)
	{
		int b = color & 0xff;
		color = color >> 8;
		int g = color & 0xff;
		color = color >> 8;
		int r = color;

		return QVector4D((float)r / 256, (float)g / 256, (float)b / 256, 1.0);
	}

	int qvector4DColor2QRgbIntColor(QVector4D color)
	{
		int r = qRound(color.x() * 255);
		int g = qRound(color.y() * 255);
		int b = qRound(color.y() * 255);

		return ((r << 16) & 0xff0000) | ((g << 8) & 0x00ff00) | (b & 0xff);
	}
}