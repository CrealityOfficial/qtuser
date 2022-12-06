#ifndef QTUSER_3D_ATTRIBUTE_1658112808251_H
#define QTUSER_3D_ATTRIBUTE_1658112808251_H
#include "qtuser3d/qtuser3dexport.h"
#include <QtCore/QByteArray>

namespace qtuser_3d
{
	struct AttributeShade
	{
		AttributeShade()
			: count(0)
			, stride(3)
		{

		}

		AttributeShade(int _count, int _stride, const QByteArray& _bytes)
			: count(_count)
			, stride(_stride)
			, bytes(_bytes)
		{

		}

		int count;
		int stride;
		QByteArray bytes;
		QString name;
	};
}

#endif // QTUSER_3D_ATTRIBUTE_1658112808251_H