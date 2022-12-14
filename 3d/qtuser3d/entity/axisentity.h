#ifndef QTUSER_3D_AXISENTITY_1595053218074_H
#define QTUSER_3D_AXISENTITY_1595053218074_H
#include "qtuser3d/entity/basicentity.h"

namespace qtuser_3d
{
	class PureColorEntity;
	class QTUSER_3D_API AxisEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		AxisEntity(Qt3DCore::QNode* parent = nullptr, int axistype = 0, QVector3D* s_use = nullptr);
		virtual ~AxisEntity();

		void translate(QVector3D v);

	protected:
		PureColorEntity* m_xAxis;
		PureColorEntity* m_yAxis;
		PureColorEntity* m_zAxis;
	};
}
#endif // QTUSER_3D_AXISENTITY_1595053218074_H