#ifndef QTUSER_3D_PRIMITIVESHAPEUTIL_1595572391411_H
#define QTUSER_3D_PRIMITIVESHAPEUTIL_1595572391411_H
#include "qtuser3d/qtuser3dexport.h"
#include <QtGui/QMatrix4x4>

namespace qtuser_3d
{
	QTUSER_3D_API QMatrix4x4 layMatrixFromPositionNormal(const QVector3D& position, const QVector3D& normal, const QVector3D& scale, const QVector3D& originNormal);
	QTUSER_3D_API QMatrix4x4 layArrowMatrix(const QVector3D& position, const QVector3D& normal, const QVector3D& scale);
	QTUSER_3D_API QMatrix4x4 layArrowMatrix2(const QVector3D& position, const QVector3D& normal, const QVector3D& size, float ratios = 60.0f);
}
#endif // QTUSER_3D_PRIMITIVESHAPEUTIL_1595572391411_H