#ifndef QTUSER_3D_PRINTERSKIRTENTITY_1594887310960_H
#define QTUSER_3D_PRINTERSKIRTENTITY_1594887310960_H
#include "qtuser3d/entity/basicentity.h"
#include "qtuser3d/math/box3d.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PrinterSkirtDecorEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		PrinterSkirtDecorEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterSkirtDecorEntity();

		void updateBoundingBox(const Box3D& box);
		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_colorParameter;
	};


	class QTUSER_3D_API PrinterSkirtEntity : public BasicEntity
	{
		Q_OBJECT
	public:
		PrinterSkirtEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterSkirtEntity();

		void updateBoundingBox(const Box3D& box);
		void setColor(const QVector4D& color);
		void setHighlight(bool highlight);

	protected:
		PrinterSkirtDecorEntity *m_outerEntity;
		PrinterSkirtDecorEntity *m_verticalEntity;
		PrinterSkirtDecorEntity* m_innerHighlightEntity;
	};
}
#endif // QTUSER_3D_PRINTERSKIRTENTITY_1594887310960_H