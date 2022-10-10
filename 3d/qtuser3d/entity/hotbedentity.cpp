#include "hotbedentity.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/geometry/trianglescreatehelper.h"
#include "faces.h"


namespace qtuser_3d
{
	HotbedEntity::HotbedEntity(Qt3DCore::QNode* parent)
		: BasicEntity(parent)
	{
		//setFaceNum(faceNum);
	}

	HotbedEntity::~HotbedEntity()
	{
	}

	void HotbedEntity::drawFace(bedType _bedType)
	{
		//setFaceNum(faceNum);
		m_bedType = _bedType;
		float minX = 0;
		float minY = 0;
		float minZ = 0;
		if (_bedType == bedType::CR_GX)
		{
			for (int n = 0; n < 9; n++)
			{
				Faces* aface = new Faces(this);
				aface->setColor(QVector4D(0.0f, 0.0f, 0.5f, 0.5f));
				m_bedFaces.push_back(aface);
			}

			int vertexNum = 4;
			int triangleNum = 2;
			signed long long unit = 78.300;
			signed long long small_unit = 70.850;

			QVector<QVector3D> positions4;
			QVector<unsigned> indices4;
			positions4.push_back(QVector3D(minX, minY, minZ));//0
			positions4.push_back(QVector3D(small_unit, minY, minZ));//4
			positions4.push_back(QVector3D(small_unit, small_unit, minZ));//5
			positions4.push_back(QVector3D(minX, small_unit, minZ));//1
			indices4.push_back(0); indices4.push_back(1); indices4.push_back(2);//D4
			indices4.push_back(0); indices4.push_back(2); indices4.push_back(3);
			Qt3DRender::QGeometry* geometry = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions4.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices4.at(0));
			m_bedFaces[0]->setGeometry(geometry);
			setVisibility(0, false);

			QVector<QVector3D> positions5;
			QVector<unsigned> indices5;
			positions5.push_back(QVector3D(minX, small_unit, minZ));//1
			positions5.push_back(QVector3D(small_unit, small_unit, minZ));//5
			positions5.push_back(QVector3D(small_unit, small_unit + unit, minZ));//6
			positions5.push_back(QVector3D(minX, small_unit+ unit, minZ));//2
			indices5.push_back(0); indices5.push_back(1); indices5.push_back(2);//D5
			indices5.push_back(0); indices5.push_back(2); indices5.push_back(3);
			Qt3DRender::QGeometry* geometry1 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions5.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices5.at(0));
			m_bedFaces[1]->setGeometry(geometry1);
			setVisibility(1, false);

			QVector<QVector3D> positions6;
			QVector<unsigned> indices6;
			positions6.push_back(QVector3D(minX, small_unit+ unit, minZ));//2
			positions6.push_back(QVector3D(small_unit, small_unit + unit, minZ));//6
			positions6.push_back(QVector3D(small_unit, small_unit * 2 + unit, minZ));//7
			positions6.push_back(QVector3D(minX, small_unit*2 + unit, minZ));//3
			indices6.push_back(0); indices6.push_back(1); indices6.push_back(2);//D6
			indices6.push_back(0); indices6.push_back(2); indices6.push_back(3);
			Qt3DRender::QGeometry* geometry2 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions6.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices6.at(0));
			m_bedFaces[2]->setGeometry(geometry2);
			setVisibility(2, false);

			QVector<QVector3D> positions3;
			QVector<unsigned> indices3;
			positions3.push_back(QVector3D(small_unit, minY, minZ));//4
			positions3.push_back(QVector3D(small_unit+ unit, minY, minZ));//8
			positions3.push_back(QVector3D(small_unit + unit, small_unit, minZ));//9
			positions3.push_back(QVector3D(small_unit, small_unit, minZ));//5
			indices3.push_back(0); indices3.push_back(1); indices3.push_back(2);//D3
			indices3.push_back(0); indices3.push_back(2); indices3.push_back(3);
			Qt3DRender::QGeometry* geometry3 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions3.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices3.at(0));
			m_bedFaces[3]->setGeometry(geometry3);
			setVisibility(3, false);

			QVector<QVector3D> positions0;
			QVector<unsigned> indices0;
			positions0.push_back(QVector3D(small_unit, small_unit, minZ));//5
			positions0.push_back(QVector3D(small_unit + unit, small_unit, minZ));//9
			positions0.push_back(QVector3D(small_unit + unit, small_unit + unit, minZ));//10
			positions0.push_back(QVector3D(small_unit, small_unit + unit, minZ));//6
			indices0.push_back(0); indices0.push_back(1); indices0.push_back(2);//D0
			indices0.push_back(0); indices0.push_back(2); indices0.push_back(3);
			Qt3DRender::QGeometry* geometry4 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions0.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices0.at(0));
			m_bedFaces[4]->setGeometry(geometry4);
			setVisibility(4, false);

			QVector<QVector3D> positions7;
			QVector<unsigned> indices7;
			positions7.push_back(QVector3D(small_unit, small_unit + unit, minZ));//6
			positions7.push_back(QVector3D(small_unit + unit, small_unit + unit, minZ));//10
			positions7.push_back(QVector3D(small_unit + unit, small_unit * 2 + unit, minZ));//11
			positions7.push_back(QVector3D(small_unit, small_unit * 2 + unit, minZ));//7
			indices7.push_back(0); indices7.push_back(1); indices7.push_back(2);//D7
			indices7.push_back(0); indices7.push_back(2); indices7.push_back(3);
			Qt3DRender::QGeometry* geometry5 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions7.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices7.at(0));
			m_bedFaces[5]->setGeometry(geometry5);
			setVisibility(5, false);

			QVector<QVector3D> positions2;
			QVector<unsigned> indices2;
			positions2.push_back(QVector3D(small_unit+ unit, minY, minZ));//8
			positions2.push_back(QVector3D(small_unit * 2 + unit, minY, minZ));//12
			positions2.push_back(QVector3D(small_unit * 2 + unit, small_unit, minZ));//13
			positions2.push_back(QVector3D(small_unit + unit, small_unit, minZ));//9
			indices2.push_back(0); indices2.push_back(1); indices2.push_back(2);//D2
			indices2.push_back(0); indices2.push_back(2); indices2.push_back(3);
			Qt3DRender::QGeometry* geometry6 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions2.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices2.at(0));
			m_bedFaces[6]->setGeometry(geometry6);
			setVisibility(6, false);

			QVector<QVector3D> positions1;
			QVector<unsigned> indices1;
			positions1.push_back(QVector3D(small_unit + unit, small_unit, minZ));//9
			positions1.push_back(QVector3D(small_unit * 2 + unit, small_unit, minZ));//13
			positions1.push_back(QVector3D(small_unit * 2 + unit, small_unit + unit, minZ));//14
			positions1.push_back(QVector3D(small_unit + unit, small_unit + unit, minZ));//10
			indices1.push_back(0); indices1.push_back(1); indices1.push_back(2);//D1
			indices1.push_back(0); indices1.push_back(2); indices1.push_back(3);
			Qt3DRender::QGeometry* geometry7 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions1.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices1.at(0));
			m_bedFaces[7]->setGeometry(geometry7);
			setVisibility(7, false);


			QVector<QVector3D> positions8;
			QVector<unsigned> indices8;
			positions8.push_back(QVector3D(small_unit + unit, small_unit + unit, minZ));//10
			positions8.push_back(QVector3D(small_unit * 2 + unit, small_unit + unit, minZ));//14
			positions8.push_back(QVector3D(small_unit * 2 + unit, small_unit * 2 + unit, minZ));//15
			positions8.push_back(QVector3D(small_unit + unit, small_unit * 2 + unit, minZ));//11
			indices8.push_back(0); indices8.push_back(1); indices8.push_back(2);//D8
			indices8.push_back(0); indices8.push_back(2); indices8.push_back(3);
			Qt3DRender::QGeometry* geometry8 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions8.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices8.at(0));
			m_bedFaces[8]->setGeometry(geometry8);
			setVisibility(8, false);

			m_isHots.resize(9);
			m_hotZone.push_back(Box3D(QVector3D(0, 0, -1.0), QVector3D(small_unit, small_unit, 250.0)));//D4
			m_hotZone.push_back(Box3D(QVector3D(0, small_unit, -1.0), QVector3D(small_unit, small_unit + unit, 250.0)));//D5
			m_hotZone.push_back(Box3D(QVector3D(0, small_unit + unit, -1.0), QVector3D(small_unit, small_unit * 2 + unit, 250.0)));//D6
			m_hotZone.push_back(Box3D(QVector3D(small_unit, 0, -1.0), QVector3D(small_unit + unit, small_unit, 250.0)));//D3
			m_hotZone.push_back(Box3D(QVector3D(small_unit, small_unit, -1.0), QVector3D(small_unit + unit, small_unit + unit, 250.0)));//D0
			m_hotZone.push_back(Box3D(QVector3D(small_unit, small_unit + unit, -1.0), QVector3D(small_unit + unit, small_unit * 2 + unit, 250.0)));//D7
			m_hotZone.push_back(Box3D(QVector3D(small_unit + unit, 0, -1.0), QVector3D(small_unit * 2 + unit, small_unit, 250.0)));//D2
			m_hotZone.push_back(Box3D(QVector3D(small_unit + unit, small_unit, -1.0), QVector3D(small_unit * 2 + unit, small_unit + unit, 250.0)));//D1
			m_hotZone.push_back(Box3D(QVector3D(small_unit + unit, small_unit + unit, -1.0), QVector3D(small_unit * 2 + unit, small_unit * 2 + unit, 250.0)));//D8
		}
		else if (_bedType == bedType::CR_GX01)
		{
			for (int n = 0; n < 9; n++)
			{
				Faces* aface = new Faces(this);
				aface->setColor(QVector4D(0.0f, 0.0f, 0.5f, 0.5f));
				m_bedFaces.push_back(aface);
			}

			int vertexNum = 4;
			int triangleNum = 2;
			signed long long unit = 42.50; 
			signed long long unit2 = 72.50;

			QVector<QVector3D> positions0;
			QVector<unsigned> indices0;
			positions0.push_back(QVector3D(0.0, 0.0, minZ));//2
			positions0.push_back(QVector3D(unit, minY, minZ));//6
			positions0.push_back(QVector3D(unit, 235.0, minZ));//7
			positions0.push_back(QVector3D(minX, 235.0, minZ));//3
			indices0.push_back(0); indices0.push_back(1); indices0.push_back(2);//D0-0
			indices0.push_back(0); indices0.push_back(2); indices0.push_back(3);
			Qt3DRender::QGeometry* geometry0 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions0.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices0.at(0));
			m_bedFaces[0]->setGeometry(geometry0);
			setVisibility(0, false);

			QVector<QVector3D> positions1;
			QVector<unsigned> indices1;
			positions1.push_back(QVector3D(unit, 0.0, 0.0));
			positions1.push_back(QVector3D(235.0, 0.0, 0.0));
			positions1.push_back(QVector3D(235.0, unit, 0.0));
			positions1.push_back(QVector3D(unit, unit, 0.0));
			indices1.push_back(0); indices1.push_back(1); indices1.push_back(2);//D0
			indices1.push_back(0); indices1.push_back(2); indices1.push_back(3);
			Qt3DRender::QGeometry* geometry1 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions1.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices1.at(0));
			m_bedFaces[1]->setGeometry(geometry1);
			setVisibility(1, false);

			QVector<QVector3D> positions2;
			QVector<unsigned> indices2;
			positions2.push_back(QVector3D(235.0- unit, unit, 0.0));
			positions2.push_back(QVector3D(235.0, unit, 0.0));
			positions2.push_back(QVector3D(235.0, 235.0, 0.0));
			positions2.push_back(QVector3D(235.0 - unit, 235.00, 0.0));
			indices2.push_back(0); indices2.push_back(1); indices2.push_back(2);//D1
			indices2.push_back(0); indices2.push_back(2); indices2.push_back(3);
			Qt3DRender::QGeometry* geometry2 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions2.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices2.at(0));
			m_bedFaces[2]->setGeometry(geometry2);
			setVisibility(2, false);

			QVector<QVector3D> positions3;
			QVector<unsigned> indices3;
			positions3.push_back(QVector3D(unit, 235.0-unit, 0.0));//2
			positions3.push_back(QVector3D(235.0 - unit, 235.0 - unit, 0.0));//6
			positions3.push_back(QVector3D(235.0 - unit, 235.0, 0.0));//7
			positions3.push_back(QVector3D(unit, 235.0, 0.0));//3
			indices3.push_back(0); indices3.push_back(1); indices3.push_back(2);//D6
			indices3.push_back(0); indices3.push_back(2); indices3.push_back(3);
			Qt3DRender::QGeometry* geometry3 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions3.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices3.at(0));
			m_bedFaces[3]->setGeometry(geometry3);
			setVisibility(3, false);


			QVector<QVector3D> positions4;
			QVector<unsigned> indices4;
			positions4.push_back(QVector3D(unit,  unit, 0.0));//2
			positions4.push_back(QVector3D(72.50, unit, 0.0));//6
			positions4.push_back(QVector3D(72.50, 235.0-unit, 0.0));//7
			positions4.push_back(QVector3D(unit, 235.0 - unit, 0.0));//3
			indices4.push_back(0); indices4.push_back(1); indices4.push_back(2);//D6
			indices4.push_back(0); indices4.push_back(2); indices4.push_back(3);
			Qt3DRender::QGeometry* geometry4 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions4.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices4.at(0));
			m_bedFaces[4]->setGeometry(geometry4);
			setVisibility(4, false);

			QVector<QVector3D> positions5;
			QVector<unsigned> indices5;
			positions5.push_back(QVector3D(72.50, unit, 0.0));//2
			positions5.push_back(QVector3D(235.0-unit, unit, 0.0));//6
			positions5.push_back(QVector3D(235.0 - unit, 72.50, 0.0));//7
			positions5.push_back(QVector3D(72.50, 72.50, 0.0));//3
			indices5.push_back(0); indices5.push_back(1); indices5.push_back(2);//D6
			indices5.push_back(0); indices5.push_back(2); indices5.push_back(3);
			Qt3DRender::QGeometry* geometry5 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions5.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices5.at(0));
			m_bedFaces[5]->setGeometry(geometry5);
			setVisibility(5, false);

			QVector<QVector3D> positions6;
			QVector<unsigned> indices6;
			positions6.push_back(QVector3D(235.0-72.5, 72.5, 0.0));//2
			positions6.push_back(QVector3D(235.0- unit, 72.5, 0.0));//6
			positions6.push_back(QVector3D(235.0- unit, 235.0 - unit, 0.0));//7
			positions6.push_back(QVector3D(235.0-72.5, 235.0 - unit, 0.0));//3
			indices6.push_back(0); indices6.push_back(1); indices6.push_back(2);//D6
			indices6.push_back(0); indices6.push_back(2); indices6.push_back(3);
			Qt3DRender::QGeometry* geometry6 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions6.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices6.at(0));
			m_bedFaces[6]->setGeometry(geometry6);
			setVisibility(6, false);

			QVector<QVector3D> positions7;
			QVector<unsigned> indices7;
			positions7.push_back(QVector3D(72.5, 235.0 - 72.5, 0.0));//2
			positions7.push_back(QVector3D(235.0 - 72.5, 235.0 - 72.5, 0.0));//6
			positions7.push_back(QVector3D(235.0 - 72.5, 235.0- unit, 0.0));//7
			positions7.push_back(QVector3D(72.5, 235.0- unit, 0.0));//3
			indices7.push_back(0); indices7.push_back(1); indices7.push_back(2);//D6
			indices7.push_back(0); indices7.push_back(2); indices7.push_back(3);
			Qt3DRender::QGeometry* geometry7 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions7.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices7.at(0));
			m_bedFaces[7]->setGeometry(geometry7);
			setVisibility(7, false);

			QVector<QVector3D> positions8;//center
			QVector<unsigned> indices8;
			positions8.push_back(QVector3D(72.5, 72.5, minZ));
			positions8.push_back(QVector3D(235-72.5, 72.5, minZ));
			positions8.push_back(QVector3D(235-72.5, 235 - 72.5, minZ));
			positions8.push_back(QVector3D(72.5, 235 - 72.5, minZ));
			indices8.push_back(0); indices8.push_back(1); indices8.push_back(2);//D6
			indices8.push_back(0); indices8.push_back(2); indices8.push_back(3);
			Qt3DRender::QGeometry* geometry8 = qtuser_3d::TrianglesCreateHelper::create(vertexNum, (float*)&positions8.at(0), nullptr, nullptr, triangleNum, (unsigned*)&indices8.at(0));
			m_bedFaces[8]->setGeometry(geometry8);
			setVisibility(8, false);

			m_isHots.resize(9);
			m_hotZone.push_back(Box3D(QVector3D(minX, minY, -1.0), QVector3D(unit, 235.0, 230)));//D0
			m_hotZone.push_back(Box3D(QVector3D(unit, 0.0, -1.0), QVector3D(235.0, unit, 230)));//D1
			m_hotZone.push_back(Box3D(QVector3D(235.0 - unit, unit, -1.0), QVector3D(235.0, 235.0, 230)));//D2
			m_hotZone.push_back(Box3D(QVector3D(unit, 235.0 - unit, -1.0), QVector3D(235.0 - unit, 235.0, 230)));//D3
			m_hotZone.push_back(Box3D(QVector3D(unit, unit, -1.0), QVector3D(72.50, 235.0 - unit, 230)));//D4
			m_hotZone.push_back(Box3D(QVector3D(72.5, unit, -1.0), QVector3D(235.0 - unit, 72.5, 230)));//D5
			m_hotZone.push_back(Box3D(QVector3D(235.0 - 72.5, 72.5, -1.0), QVector3D(235.0 - 42.5, 235.0 - 42.5, 230)));//D6
			m_hotZone.push_back(Box3D(QVector3D(72.5, 235.0 - 72.5, -1.0), QVector3D(235.0 - 72.5, 235.0 - 42.50, 230)));//D7
			m_hotZone.push_back(Box3D(QVector3D(30.0 + unit, 30.0 + unit, -1.0), QVector3D(30.0 + unit + 90.0, 30.0 + unit + 90.0, 230)));//D8
		}
	}


	void HotbedEntity::checkBed(QList<Box3D>& boxes)
	{
		if (m_bedType == bedType::CR_GX)
		{
			for (int n = 0; n < m_isHots.size(); n++)
			{
				m_isHots[n] = false;
			}

			for (Box3D& abox3d : boxes)
			{
				for (int n = 0; n < m_hotZone.size();n++)
				{
					if (abox3d.intersects(m_hotZone[n]))
					{
						m_isHots[n] = true;
					}
				}
			}

			for (int n = 0; n < m_isHots.size(); n++)
			{
				setVisibility(n, m_isHots[n]);
			}
		}
		else if (m_bedType == bedType::CR_GX01)
		{
			for (int n = 0; n < m_isHots.size(); n++)
			{
				m_isHots[n] = false;
			}

			for (Box3D& abox3d : boxes)
			{
				for (int n = 0; n < m_hotZone.size(); n++)
				{
					if (abox3d.intersects(m_hotZone[n]))
					{
						if (n>=0 && n<4)
						{
							m_isHots[0] = true;
							m_isHots[1] = true;
							m_isHots[2] = true;
							m_isHots[3] = true;
						} 
						else if (n == 8)
						{
							m_isHots[8] = true;
						}
						else
						{
							m_isHots[4] = true;
							m_isHots[5] = true;
							m_isHots[6] = true;
							m_isHots[7] = true;
						}
					}
				}
			}
			for (int n = 0; n < m_isHots.size(); n++)
			{
				setVisibility(n, m_isHots[n]);
			}
		}
	}

	void HotbedEntity::setColor(const QVector4D& color)
	{
		for (Faces* aface: m_bedFaces)
		{
			aface->setColor(color);
		}
	}

	//void HotbedEntity::setBedType(bedType _bedType)
	//{
	//	m_bedFaces.clear();
	//	m_isHots.clear();
	//	m_hotZone.clear();
	//	for (int n=0;n<m_faceNum;n++)
	//	{
	//		Faces* aface = new Faces(this);
	//		aface->setColor(QVector4D(0.0f, 0.0f, 0.5f, 0.5f));
	//		m_bedFaces.push_back(aface);
	//	}
	//}


	void HotbedEntity::clearData()
	{
		for (int n = 0; n < m_isHots.size(); n++)
		{
			setVisibility(n, false);
		}
		m_bedFaces.clear();
		m_isHots.clear();
		m_hotZone.clear();
		//setFaceNum(0);
	}

	void HotbedEntity::setVisibility(int faceIndex, bool visibility)
	{
		for (int n=0;n< m_bedFaces.size();n++)
		{
			if (n == faceIndex)
			{
				m_bedFaces[n]->setEnabled(visibility ? true : false);
			}
		}
	}


}