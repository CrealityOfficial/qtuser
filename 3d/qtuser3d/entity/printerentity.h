#ifndef _RENDERHELPER_PRINTERENTITY_1588651416007_H
#define _RENDERHELPER_PRINTERENTITY_1588651416007_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QEntity>
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/math/box3d.h"


namespace qtuser_3d
{
	// printer platform
	class QTUSER_3D_API PrinterEntity: public Qt3DCore::QEntity
	{
	public:
		PrinterEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~PrinterEntity();

		virtual void onCameraChanged(qtuser_3d::ScreenCamera* screenCamera) = 0;
		virtual void updateBox(const qtuser_3d::Box3D& box) = 0;

		virtual void onModelChanged(Box3D basebox, bool bleft, bool bright, bool bfront, bool bback, bool bup, bool bdown) = 0;

		virtual void showPrinterEntity(bool isShow) = 0;

		virtual void onThemeChanged(int themeType) = 0;

	};
}
#endif // _RENDERHELPER_PRINTERENTITY_1588651416007_H
