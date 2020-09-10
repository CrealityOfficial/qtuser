#ifndef QTUSER_3D_SUPPORTENTITY_1595210470841_H
#define QTUSER_3D_SUPPORTENTITY_1595210470841_H
#include "qtuser3d/entity/pickableentity.h"
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QBuffer>
#include <Qt3DRender/QAttribute>

namespace qtuser_3d
{
	class QTUSER_3D_API SupportEntity : public PickableEntity
	{
		Q_OBJECT
	public:
		SupportEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~SupportEntity();

		void setPositionBufferData(const QByteArray& bytes);
		void setNormalBufferData(const QByteArray& bytes);
		void setFlagsBufferData(const QByteArray& bytes);
		void setCount(uint count);
		void updateFlagBufferData(int offset, const QByteArray& bytes);
	protected:
		Qt3DRender::QGeometry* m_geometry;

		Qt3DRender::QAttribute* m_positionAttribute;
		Qt3DRender::QAttribute* m_normalAttribute;
		Qt3DRender::QAttribute* m_flagAttribute;

		Qt3DRender::QBuffer* m_positionBuffer;
		Qt3DRender::QBuffer* m_normalBuffer;
		Qt3DRender::QBuffer* m_flagBuffer;
	};
}
#endif // QTUSER_3D_SUPPORTENTITY_1595210470841_H