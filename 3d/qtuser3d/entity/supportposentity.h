#ifndef SUPPORT_POS_ENTITY_H__
#define SUPPORT_POS_ENTITY_H__
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/entity/pickableentity.h"
#include "qtuser3d/module/pickable.h"
#include "qtuser3d/module/node3d.h"


namespace qtuser_3d
{

	class QTUSER_3D_API SupportPosEntity : public PickableEntity
	{
		Q_OBJECT
	public:
		SupportPosEntity(QVector3D position, QObject* parent = nullptr);
		~SupportPosEntity();

		QVector3D position();

		int pointNum();

		int offset(int num);

		QPoint vertexBase() const;
		void changeFaceBase(int facebase);

	protected:
		int m_pointNum;
		QVector3D m_position;
	};

}


#endif
