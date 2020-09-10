#ifndef _qtuser_3d_SPACE3D_1588841647525_H
#define _qtuser_3d_SPACE3D_1588841647525_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/math/ray.h"
#include <QtGui/QVector2D>
#include <QtGui/QMatrix4x4>

namespace qtuser_3d
{
	QTUSER_3D_API QVector3D point3DFromVector2D(const QVector2D& point, const QVector2D& center, const QVector2D& size, bool skipz);

	QTUSER_3D_API QVector3D point3DFromVector2D(const QVector2D& point, const QVector2D& center, float width, float height, bool skipz);

	QTUSER_3D_API void boxFittingBox(const Box3D& baseBounding, const Box3D& initBox, QVector3D& translate, QVector3D& scale);

	QTUSER_3D_API Box3D transformBox(const QMatrix4x4& matrix, const Box3D& box);

	QTUSER_3D_API bool lineCollidePlane(const QVector3D& planeCenter, const QVector3D& planeDir, const Ray& ray, QVector3D& collide);
}
#endif // _qtuser_3d_SPACE3D_1588841647525_H
