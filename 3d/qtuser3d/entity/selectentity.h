#ifndef QTUSER_3D_SELECT_ENTITY_H
#define QTUSER_3D_SELECT_ENTITY_H

#include "qtuser3d/refactor/xentity.h"
#include <QtGui/QVector3D>
#include <vector>

namespace qtuser_3d
{
	
	class QTUSER_3D_API SelectEntity : public XEntity
	{
		Q_OBJECT
	public:
		SelectEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~SelectEntity();

		void updateData(const std::vector<QVector3D>& vertexData);
		void setColor(QVector4D color);
	private:
		Qt3DRender::QParameter* m_pColorParam;
	};
}
#endif // QTUSER_3D_BASICENTITY_1594569444448_H