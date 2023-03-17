#ifndef QTUSER_3D_ALONEPOINTENTITY_1594872195512_H
#define QTUSER_3D_ALONEPOINTENTITY_1594872195512_H
#include "qtuser3d/entity/basicentity.h"
#include <Qt3DRender/QPointSize>
namespace qtuser_3d
{
	class QTUSER_3D_API AlonePointEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		AlonePointEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~AlonePointEntity();

		void updateGlobal(QVector3D& point);
		void updateLocal(QVector3D& point, const QMatrix4x4& parentMatrix);
		void setColor(const QVector4D& color);
		void setPointSize(float size);	
		void setShader(const QString& name);
	protected:
		void selectEffect();
	protected:
		Qt3DRender::QParameter* m_colorParameter;

		Qt3DRender::QPointSize* m_pointSizeState;
		Qt3DRender::QEffect* m_usedEffect;
		float m_pointSize;
	};
}
#endif // QTUSER_3D_ALONEPOINTENTITY_1594872195512_H