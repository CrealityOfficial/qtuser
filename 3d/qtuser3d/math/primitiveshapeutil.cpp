#include "primitiveshapeutil.h"
#include "qtuser3d/math/angles.h"
#include "qtuser3d/math/const.h"

namespace qtuser_3d
{
	QMatrix4x4 layMatrixFromPositionNormal(const QVector3D& position, const QVector3D& normal, const QVector3D& scale, const QVector3D& originNormal)
	{
		QMatrix4x4 matrix;
		matrix.translate(position);

		float angle = angleOfVector3D2(normal, originNormal);
		QVector3D axis = QVector3D::crossProduct(originNormal, normal);
		axis.normalize();
		if (abs(axis.x()) < 0.00001 && abs(axis.y()) < 0.00001 && abs(axis.z()) < 0.00001)
		{
			axis.setX(1);
		}

		matrix.rotate(QQuaternion::fromAxisAndAngle(axis, angle));
		matrix.scale(scale);

		QMatrix4x4 rotateMatrix;
		rotateMatrix.rotate(QQuaternion::fromAxisAndAngle(axis, angle));

		QVector3D newNormal = rotateMatrix * originNormal;

		QVector3D useNormal = normal + newNormal;
		
		if (abs(useNormal.x()) < 0.00001 && abs(useNormal.y()) < 0.00001 && abs(useNormal.z()) < 0.00001)
		{
			matrix.setToIdentity();
			matrix.translate(position);
			matrix.rotate(QQuaternion::fromAxisAndAngle(-axis, angle));
			matrix.scale(scale);
		}

		return matrix;
	}

	QMatrix4x4 layArrowMatrix(const QVector3D& position, const QVector3D& normal, const QVector3D& scale)
	{
		QVector3D originNormal(0.0f, 1.0f, 0.0f);
		return layMatrixFromPositionNormal(position, normal, scale, originNormal);
	}

	QMatrix4x4 layArrowMatrix2(const QVector3D& position, const QVector3D& normal, const QVector3D& size, float ratio)
	{
		float s = 1.0f;
		QVector3D scope = size;
		s = scope.x() > scope.y() ? scope.x() : scope.y();
		s = s > scope.z() ? s : scope.z();

		if (ratio <= 0.0f) ratio = 60.0f;
		s /= 60.0f;

		//QVector3D scale(60.0f, 10.0f, 60.0f);
		QVector3D scale(s, s, s);
		return layArrowMatrix(position, normal, scale);
	}
}