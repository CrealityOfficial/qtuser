#include "textserializer.h"

namespace qtuser_3d
{
	QMatrix4x4 string2Matrix(const QString& str)
	{
        QStringList ls = str.split(" ");
        QMatrix4x4 m;
        if (ls.size() >= 16)
        {
            float fMat[16];
            memset(fMat, 0, 16 * 4);
            for (int i = 0; i < 16; i++)
            {
                fMat[i] = ls[i].toFloat();
            }
            m = QMatrix4x4(fMat);
        }

        return m.transposed();
	}
}
