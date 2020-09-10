#ifndef QTUSER_3D_LINEENTITY_1594892276547_H
#define QTUSER_3D_LINEENTITY_1594892276547_H
#include "qtuser3d/entity/basicentity.h"
#include <QtCore/QVector>
#include <QtGui/QColor>
#include <QtGui/QVector3D>

namespace qtuser_3d
{
	class QTUSER_3D_API LineEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		LineEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~LineEntity();

		void updateGeometry(QVector<QVector3D>& positions, QVector<QVector4D>& colors, bool loop = false);
		void updateGeometry(QVector<QVector3D>& positions, bool loop = false);
		void updateGeometry(int pointsNum, float* positions, float* color = nullptr, bool loop = false);
		void updateGeometry(Qt3DRender::QGeometry* geometry, bool color = false, bool loop = false);
		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
		bool m_usePure;
	};
}
#endif // QTUSER_3D_LINEENTITY_1594892276547_H