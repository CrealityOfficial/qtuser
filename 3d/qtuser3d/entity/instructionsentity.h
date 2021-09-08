#ifndef QTUSER_3D_INSTRUCTIONS_ENTITY_1595053218074_H
#define QTUSER_3D_INSTRUCTIONS_ENTITY_1595053218074_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class PureColorEntity;
	class QTUSER_3D_API InstructionsEntity : public Qt3DCore::QEntity
	{
		Q_OBJECT
	public:
		InstructionsEntity(Qt3DCore::QNode* parent = nullptr, int type = 7);
		virtual ~InstructionsEntity();

		void translate(QVector3D v);
		void updateGlobal(Box3D& box);
		void updateGlobal2(Box3D& box);

	protected:
		PureColorEntity* m_xAxis;
		PureColorEntity* m_yAxis;
		PureColorEntity* m_zAxis;
	};
}
#endif // QTUSER_3D_INSTRUCTIONS_ENTITY_1595053218074_H