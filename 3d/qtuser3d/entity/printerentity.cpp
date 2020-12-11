#include "qtuser3d/entity/printerentity.h"
#include "qtuser3d/entity/boxentity.h"
#include "qtuser3d/entity/printerskirtentity.h"
#include "qtuser3d/entity/printergrid.h"
#include "qtuser3d/entity/printertext.h"
#include "qtuser3d/entity/axisentity.h"
#include "qtuser3d/entity/faceentity.h"

namespace qtuser_3d
{
	PrinterEntity::PrinterEntity(Qt3DCore::QNode* parent)
		:Qt3DCore::QEntity(parent)
	{
	}

	PrinterEntity::~PrinterEntity()
	{
	}

	//void PrinterEntity::onCameraChanged(qtuser_3d::ScreenCamera* screenCamera)
	//{
	//}

	//void PrinterEntity::updateBox(const qtuser_3d::Box3D& box)
	//{
	//}

	//void PrinterEntity::onModelChanged(Box3D basebox, bool bleft, bool bright, bool bfront, bool bback, bool bup, bool bdown)
	//{
	//}

	//void PrinterEntity::showPrinterEntity(bool isShow)
	//{
	//}

	
}
