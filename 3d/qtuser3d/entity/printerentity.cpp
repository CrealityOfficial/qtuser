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
		QVector4D blueColor = QVector4D(0.180f, 0.541f, 0.968f, 1.0f);
		m_boxEntity = new BoxEntity(this);
		m_boxEntity->setColor(blueColor);
		m_printerSkirt = new PrinterSkirtEntity(this);
		m_printerText = new PrinterText(this);
		m_printerGrid = new PrinterGrid(this);
		m_axisEntity = new AxisEntity(this);
		m_faceEntity = new FaceEntity(this);

		m_imageEntity = new ImageEntity(this);
	}

	PrinterEntity::~PrinterEntity()
	{
	}

	void PrinterEntity::updateBox(const qtuser_3d::Box3D& box)
	{
		qDebug() << "scope = " << box.min << box.max;
		qtuser_3d::Box3D globalBox = box;
		m_boxEntity->updateGlobal(globalBox);
		m_printerSkirt->updateBoundingBox(globalBox);
		m_printerGrid->updateBounding(globalBox);
		m_printerText->updateLen(globalBox, 10.0f, 4);
		//m_faceEntity->drawFace(globalBox);

		m_imageEntity->updateGlobal(globalBox);
	}

	void PrinterEntity::enableSkirt(bool enable)
	{
		//m_printerSkirt->setEnabled(enable);
	}

	void PrinterEntity::visibleSubGrid(bool visible)
	{
		m_printerGrid->visibleSubGrid(visible);
	}

	void PrinterEntity::updateFace(Box3D& box,faceType type)
	{
		m_faceEntity->updateFace(box,type);
	}

	void PrinterEntity::setVisibility(int type, bool visibility)
	{
		m_faceEntity->setVisibility(type, visibility);
	}
	
	
}
