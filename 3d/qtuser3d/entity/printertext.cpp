#include "printertext.h"
#include <QtCore/qmath.h>
#include <QtGui/QVector4D>
#include "qtuser3d/entity/textmeshentity.h"

namespace qtuser_3d
{
	PrinterText::PrinterText(Qt3DCore::QNode* parent)
		:QEntity(parent)
	{
	}

	PrinterText::~PrinterText()
	{
	}

	void PrinterText::updateLen(Box3D& box, float gap, int major)
	{
		qDeleteAll(m_majors);
		qDeleteAll(m_minors);

		int maxLen = box.size().x();

		m_majors.clear();
		m_minors.clear();

		float minX = box.min.x();
		float maxX = box.max.x();

		float skirtGap = 14.0f / 220.0 * (maxX - minX);
		int startX = 0;
		int endX = qFloor(maxLen / gap);

		float majorSize = 4.0f;
		float minorSize = 2.0f;
		
		QVector4D color = QVector4D(0.180f, 0.541f, 0.968f, 1.0f);
		for (int i = startX; i <= endX; ++i)
		{
			if (i % 4 == 0)
			{
				QMatrix4x4 majorMatrix;
				TextMeshEntity* textEntity = new TextMeshEntity(this);
				textEntity->setColor(color);
				QString text = QString::number(i * (int)gap);
				int textLen = text.length();
				textEntity->setText(text);
				majorMatrix.translate(QVector3D((float)i * gap - textLen * majorSize / 3.0f, -(skirtGap + majorSize), 0.0f));
				majorMatrix.scale(majorSize);
				textEntity->setPose(majorMatrix);
				m_majors.push_back(textEntity);
			}
		}
		for (int i = startX; i <= endX; ++i)
		{
			if (i % 4 != 0)
			{
				QMatrix4x4 matrix;
				TextMeshEntity* textEntity = new TextMeshEntity(this);
				textEntity->setColor(color);
				QString text = QString::number(i * (int)gap);
				int textLen = text.length();
				textEntity->setText(text);
				matrix.translate(QVector3D((float)i * gap - textLen * minorSize / 3.0f, -(skirtGap + minorSize), 0.0f));
				matrix.scale(minorSize);
				textEntity->setPose(matrix);
				m_minors.push_back(textEntity);
			}
		}
	}
}
