#ifndef _RENDERHELPER_PRINTERENTITY_1588651416007_H
#define _RENDERHELPER_PRINTERENTITY_1588651416007_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QEntity>

#include "qtuser3d/math/box3d.h"
#include "faceentity.h"
#include "imageentity.h"


namespace qtuser_3d
{
	class BoxEntity;
	class PrinterGrid;
	class PrinterSkirtEntity;
	class PrinterText;
	class AxisEntity;
	class QTUSER_3D_API PrinterEntity: public Qt3DCore::QEntity
	{
	public:
		PrinterEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterEntity();

		void updateBox(const qtuser_3d::Box3D& box);

		void enableSkirt(bool enable);
		void visibleSubGrid(bool visible);

		void updateFace(Box3D& box,faceType type);
		void setVisibility(int type, bool visibility);

		void showPrinterEntity(bool isShow);

	protected:
		BoxEntity* m_boxEntity;
		PrinterGrid* m_printerGrid;
		PrinterSkirtEntity* m_printerSkirt;
		PrinterText* m_printerText;
		AxisEntity* m_axisEntity;
		FaceEntity* m_faceEntity;

		ImageEntity* m_imageEntity;
	};
}
#endif // _RENDERHELPER_PRINTERENTITY_1588651416007_H
