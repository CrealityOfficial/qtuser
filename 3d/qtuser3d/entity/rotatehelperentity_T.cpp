#include "qtuser3d/entity/rotatehelperentity_T.h"

#include "qtuser3d/module/manipulatecallback.h"

#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/entity/piefadeentity.h"
#include "qtuser3d/module/manipulatepickable.h"

#include "qtuser3d/module/facepicker.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/math/angles.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"

#include <Qt3DExtras/QTorusMesh>
#include <QtCore/qmath.h>
#include <Qt3DCore/QTransform>
#include <QtGui/QTransform>

#include "qtuser3d/module/pickable.h"
namespace qtuser_3d
{
	RotateHelperEntity_T::RotateHelperEntity_T(Qt3DCore::QNode* parent)
		: XEntity(parent)
		, m_rotatingFlag(false)
		, m_lastestRotAngles(0)
		, m_initRotateDirAngles(0)
		, m_rotateAxis(0.0, 0.0, 1.0)
		, m_originRotateAxis(0.0, 0.0, 1.0)
		, m_initRotateDir(-1.0, 0.0, 0.0)
		, m_originInitRotateDir(-1.0, 0.0, 0.0)
		, m_ringColor(1.0f, 0.0f, 0.0f, 1.0f)
		, m_dialColor(1.0f, 0.0f, 0.0f, 0.2f)
		, m_degreeColor(1.0f, 1.0f, 1.0f, 1.0f)
		, m_handlerColor(1.0f, 0.0f, 0.0f, 1.0f)
		, m_handlerPickedColor(1.0f, 0.79f, 0.0f, 1.0f)
		, m_scale(17.0f, 17.0f, 17.0f)
		, m_ringRadius(3)
		, m_ringMinorRadius(0.016)
		, m_dialRadius(3)
		, m_degreeRadius(2.5)
		, m_markOffset(0.2)
		, m_handlerOffset(3.36)
		, m_fixSize(false)
		, m_pRingEntity(nullptr)
		, m_pHandlerEntity(nullptr)
		, m_pHandlerPickable(nullptr)
		, m_pDialEntity(nullptr)
		, m_pDegreeEntity(nullptr)
		, m_pTipEntity(nullptr)
		, m_pGlobalTransform(nullptr)
		, m_pRotateTransform(nullptr)
		, m_pNoRotateTransform(nullptr)
		, m_pRotateGroup(nullptr)
		, m_pNoRotateGroup(nullptr)
		, m_pPickSource(nullptr)
		, m_pScreenCamera(nullptr)
		, m_pRotateCallback(nullptr)
	{
		m_pGlobalTransform = new Qt3DCore::QTransform(this);
		addComponent(m_pGlobalTransform);

		m_pNoRotateGroup = new XEntity(this);
		m_pNoRotateTransform = new Qt3DCore::QTransform(m_pNoRotateGroup);
		m_pNoRotateGroup->addComponent(m_pNoRotateTransform);

		m_pRotateGroup = new XEntity(this);
		m_pRotateTransform = new Qt3DCore::QTransform(m_pRotateGroup);
		m_pRotateGroup->addComponent(m_pRotateTransform);

		m_initQuaternion = QQuaternion::rotationTo(m_originRotateAxis, m_rotateAxis);

		initRing();
		initHandler();
		initDial();
		//initTip();
	}

	RotateHelperEntity_T::~RotateHelperEntity_T()
	{
		if (m_pRingEntity->parent() == nullptr)
			delete m_pRingEntity;

		//if (m_pRingPickable->parent() == nullptr)
		//	delete m_pRingPickable;

		if (m_pHandlerEntity->parent() == nullptr)
			delete m_pHandlerEntity;

		if (m_pHandlerPickable->parent() == nullptr)
			delete m_pHandlerPickable;

		if (m_pDialEntity->parent() == nullptr)
			delete m_pDialEntity;
	}

	void RotateHelperEntity_T::initRing()
	{
		QMatrix4x4 m;
		m.scale(m_scale);

		// ϸ����״��ת���ʼ��
		m_pRingEntity = new PieFadeEntity(m_pNoRotateGroup);
		m_pRingEntity->setObjectName("RotateHelperEntity_T.ringEntity");
		m_pRingEntity->setPose(m);
		m_pRingEntity->setColor(m_ringColor);
		m_pRingEntity->setRotMode(0);
		m_pRingEntity->setRotInitDir(m_initRotateDir);
		m_pRingEntity->setRotAxis(m_rotateAxis);
		//m_pRingEntity->setLigthEnable(true);

		Qt3DExtras::QTorusMesh* torusMesh = new Qt3DExtras::QTorusMesh(this);
		torusMesh->setRadius(m_ringRadius);
		torusMesh->setMinorRadius(m_ringMinorRadius);
		torusMesh->setRings(100);
		m_pRingEntity->replaceGeometryRenderer(torusMesh);

		//m_pRingPickable = new ManipulatePickable(this);
		//m_pRingPickable->setPickableEntity(m_pRingEntity);
		//m_pRingPickable->setShowEntity(m_pRingEntity);
	}

	void RotateHelperEntity_T::initHandler()
	{
		QMatrix4x4 m;
		m.scale(m_scale);
		m.translate(m_handlerOffset * m_initRotateDir);
		m.rotate(90.0, m_initRotateDir);

		// ��ת�ֱ���ʼ��
		m_pHandlerEntity = new ManipulateEntity(m_pRotateGroup, false, true, true, true);

		m_pHandlerEntity->setObjectName("RotateHelperEntity_T.handleEntity");
		m_pHandlerEntity->setPose(m);
		m_pHandlerEntity->setColor(m_handlerColor);
		m_pHandlerEntity->setChangeColor(m_handlerPickedColor);
		m_pHandlerEntity->setMethod(1);
		m_pHandlerEntity->setLightEnable(true);

		// �����ֱ�ģ������
		double bottomRadius = m_ringRadius / 27.0;
		double coneHeight = m_ringRadius * 2.0 / 24.0;
		double cylinderHeight = m_ringRadius * 1.8 / 24.0;
		double gapLength = m_ringRadius * 0.9 / 27.0;
		QVector3D origin(0.0, 0.0, 0.0);
	
		int split = 20;
		double roundDeltaRadians = 360 / split;
		QVector3D roundDir(1.0, 0.0, 0.0);
		QQuaternion roundDeltaQ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), roundDeltaRadians);
		std::vector<QVector3D> roundDirs;
		for (int i = 0; i < split; i++)
		{
			roundDirs.push_back(roundDir);
			roundDir = roundDeltaQ * roundDir;
		}

		std::vector<float> positions;
		std::vector<float> normals;

		// ��Բ׶
		{
			QVector3D topCenter(0.0f, 0.0f, (float)(coneHeight + gapLength + cylinderHeight / 2.0));
			QVector3D bottomCenter(0.0f, 0.0f, (float)(gapLength + cylinderHeight / 2.0));

			for (int i = 0; i < roundDirs.size(); i++)
			{
				int iNext = i < roundDirs.size() - 1 ? i + 1 : 0;

				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				QVector3D bottomVertexNext = bottomCenter + roundDirs[iNext] * bottomRadius;
				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(-1.0);
				positions.push_back(bottomVertex.x()); positions.push_back(bottomVertex.y());  positions.push_back(bottomVertex.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(-1.0);
				positions.push_back(bottomCenter.x()); positions.push_back(bottomCenter.y());  positions.push_back(bottomCenter.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(-1.0);
			}

			for (int i = 0; i < roundDirs.size(); i++)
			{
				int iNext = i < roundDirs.size() - 1 ? i + 1 : 0;

				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				QVector3D bottomVertexNext = bottomCenter + roundDirs[iNext] * bottomRadius;
				QVector3D sideNormal = ((bottomVertex - bottomCenter) + (topCenter - bottomCenter)).normalized();
				QVector3D sideNormalNext = ((bottomVertexNext - bottomCenter) + (topCenter - bottomCenter)).normalized();
				QVector3D topNormal = (sideNormal + sideNormalNext).normalized();
				positions.push_back(topCenter.x()); positions.push_back(topCenter.y());  positions.push_back(topCenter.z());
				normals.push_back(topNormal.x()); normals.push_back(topNormal.y()); normals.push_back(topNormal.z());
				positions.push_back(bottomVertex.x()); positions.push_back(bottomVertex.y());  positions.push_back(bottomVertex.z());
				normals.push_back(sideNormal.x()); normals.push_back(sideNormal.y()); normals.push_back(sideNormal.z());
				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(sideNormalNext.x()); normals.push_back(sideNormalNext.y()); normals.push_back(sideNormalNext.z());
			}
		}

		// �м�Բ��
		{
			QVector3D topCenter(0.0f, 0.0f, (float)(cylinderHeight / 2.0));
			QVector3D bottomCenter(0.0f, 0.0f, -(float)(cylinderHeight / 2.0));

			// ���µ���
			for (int i = 0; i < roundDirs.size(); i++)
			{
				int iNext = i < roundDirs.size() - 1 ? i + 1 : 0;

				QVector3D topVertex = topCenter + roundDirs[i] * bottomRadius;
				QVector3D topVertexNext = topCenter + roundDirs[iNext] * bottomRadius;
				positions.push_back(topCenter.x()); positions.push_back(topCenter.y());  positions.push_back(topCenter.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
				positions.push_back(topVertex.x()); positions.push_back(topVertex.y());  positions.push_back(topVertex.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
				positions.push_back(topVertexNext.x()); positions.push_back(topVertexNext.y());  positions.push_back(topVertexNext.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);

				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				QVector3D bottomVertexNext = bottomCenter + roundDirs[iNext] * bottomRadius;
				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(-1.0);
				positions.push_back(bottomVertex.x()); positions.push_back(bottomVertex.y());  positions.push_back(bottomVertex.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(-1.0);
				positions.push_back(bottomCenter.x()); positions.push_back(bottomCenter.y());  positions.push_back(bottomCenter.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(-1.0);
			}

			for (int i = 0; i < roundDirs.size(); i++)
			{
				int iNext = i < roundDirs.size() - 1 ? i + 1 : 0;

				QVector3D topVertex = topCenter + roundDirs[i] * bottomRadius;
				QVector3D topVertexNext = topCenter + roundDirs[iNext] * bottomRadius;
				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				QVector3D bottomVertexNext = bottomCenter + roundDirs[iNext] * bottomRadius;

				positions.push_back(topVertexNext.x()); positions.push_back(topVertexNext.y());  positions.push_back(topVertexNext.z());
				normals.push_back(roundDirs[iNext].x()); normals.push_back(roundDirs[iNext].y()); normals.push_back(roundDirs[iNext].z());
				positions.push_back(topVertex.x()); positions.push_back(topVertex.y());  positions.push_back(topVertex.z());
				normals.push_back(roundDirs[i].x()); normals.push_back(roundDirs[i].y()); normals.push_back(roundDirs[i].z());
				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(roundDirs[iNext].x()); normals.push_back(roundDirs[iNext].y()); normals.push_back(roundDirs[iNext].z());

				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(roundDirs[iNext].x()); normals.push_back(roundDirs[iNext].y()); normals.push_back(roundDirs[iNext].z());
				positions.push_back(topVertex.x()); positions.push_back(topVertex.y());  positions.push_back(topVertex.z());
				normals.push_back(roundDirs[i].x()); normals.push_back(roundDirs[i].y()); normals.push_back(roundDirs[i].z());
				positions.push_back(bottomVertex.x()); positions.push_back(bottomVertex.y());  positions.push_back(bottomVertex.z());
				normals.push_back(roundDirs[i].x()); normals.push_back(roundDirs[i].y()); normals.push_back(roundDirs[i].z());
			}
		}

		// ��Բ׶
		{
			QVector3D topCenter(0.0f, 0.0f, -(float)(coneHeight + gapLength + cylinderHeight / 2.0));
			QVector3D bottomCenter(0.0f, 0.0f, - (float)(gapLength + cylinderHeight / 2.0));

			for (int i = 0; i < roundDirs.size(); i++)
			{
				int iNext = i < roundDirs.size() - 1 ? i + 1 : 0;

				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				QVector3D bottomVertexNext = bottomCenter + roundDirs[iNext] * bottomRadius;
				positions.push_back(bottomVertex.x()); positions.push_back(bottomVertex.y());  positions.push_back(bottomVertex.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
				positions.push_back(bottomCenter.x()); positions.push_back(bottomCenter.y());  positions.push_back(bottomCenter.z());
				normals.push_back(0.0); normals.push_back(0.0); normals.push_back(1.0);
			}

			for (int i = 0; i < roundDirs.size(); i++)
			{
				int iNext = i < roundDirs.size() - 1 ? i + 1 : 0;
				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				QVector3D bottomVertexNext = bottomCenter + roundDirs[iNext] * bottomRadius;
				QVector3D sideNormal = ((bottomVertex - bottomCenter) + (topCenter - bottomCenter)).normalized();
				QVector3D sideNormalNext = ((bottomVertexNext - bottomCenter) + (topCenter - bottomCenter)).normalized();
				QVector3D topNormal = (sideNormal + sideNormalNext).normalized();
				positions.push_back(bottomVertex.x()); positions.push_back(bottomVertex.y());  positions.push_back(bottomVertex.z());
				normals.push_back(sideNormal.x()); normals.push_back(sideNormal.y()); normals.push_back(sideNormal.z());
				positions.push_back(topCenter.x()); positions.push_back(topCenter.y());  positions.push_back(topCenter.z());
				normals.push_back(topNormal.x()); normals.push_back(topNormal.y()); normals.push_back(topNormal.z());
				positions.push_back(bottomVertexNext.x()); positions.push_back(bottomVertexNext.y());  positions.push_back(bottomVertexNext.z());
				normals.push_back(sideNormalNext.x()); normals.push_back(sideNormalNext.y()); normals.push_back(sideNormalNext.z());
			}
		}

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0], qtuser_3d::AttribueSlot::Position, positions.size() / 3);
		Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0], qtuser_3d::AttribueSlot::Normal, normals.size() / 3);
		Qt3DRender::QGeometry* handlerGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute);
		m_pHandlerEntity->setGeometry(handlerGeo);

		m_pHandlerPickable = new ManipulatePickable(this);
		m_pHandlerPickable->setPickableEntity(m_pHandlerEntity);
		m_pHandlerPickable->setShowEntity(m_pHandlerEntity);
		m_pHandlerPickable->setEnableSelect(false);
	}

	void RotateHelperEntity_T::initDial()
	{
		QMatrix4x4 m;
		m.scale(m_scale);

		// �̶��̳�ʼ������͸�������Σ�
		m_pDialEntity = new PieFadeEntity(m_pRotateGroup);
		m_pDialEntity->setObjectName("RotateHelperEntity_T.dialEntity");
		m_pDialEntity->setPose(m);
		m_pDialEntity->setColor(m_dialColor);
		m_pDialEntity->setRotMode(1);
		m_pDialEntity->setRotRadians(0);
		m_pDialEntity->setRotInitDir(m_initRotateDir);
		m_pDialEntity->setRotAxis(m_rotateAxis);
		m_pDialEntity->setPassBlend();

		// �����̶���ģ������
		double radius = m_dialRadius;

		int split = 20;
		double roundDeltaRadians = 360 / split;
		QVector3D roundDir(1.0, 0.0, 0.0);
		QQuaternion roundDeltaQ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), roundDeltaRadians);
		std::vector<QVector3D> roundDirs;
		for (int i = 0; i < split; i++)
		{
			roundDirs.push_back(roundDir);
			roundDir = roundDeltaQ * roundDir;
		}

		std::vector<float> positionsDial;
		std::vector<float> normalsDial;
		std::vector<int> indicesDial;

		// Բ��
		{
			int centerIndex = positionsDial.size() / 3;
			QVector3D roundCenter(0.0, 0.0, 0.0);
			positionsDial.push_back(0.0f);
			positionsDial.push_back(0.0f);
			positionsDial.push_back(0.0f);
			normalsDial.push_back(0.0f);
			normalsDial.push_back(0.0f);
			normalsDial.push_back(1.0f);

			std::vector<int> roundEdgeVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				roundEdgeVIndices.push_back(positionsDial.size() / 3);
				QVector3D edgeVertex = roundCenter + roundDirs[i] * radius;
				positionsDial.push_back(edgeVertex.x());
				positionsDial.push_back(edgeVertex.y());
				positionsDial.push_back(edgeVertex.z());
				normalsDial.push_back(0.0f);
				normalsDial.push_back(0.0f);
				normalsDial.push_back(1.0f);
			}

			for (int i = 0; i < roundEdgeVIndices.size(); i++)
			{
				int iNext = i < roundEdgeVIndices.size() - 1 ? i + 1 : 0;
				indicesDial.push_back(roundEdgeVIndices[i]);
				indicesDial.push_back(centerIndex);
				indicesDial.push_back(roundEdgeVIndices[iNext]);

				indicesDial.push_back(roundEdgeVIndices[i]);
				indicesDial.push_back(roundEdgeVIndices[iNext]);
				indicesDial.push_back(centerIndex);
			}
		}

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positionsDial[0], qtuser_3d::AttribueSlot::Position, positionsDial.size() / 3);
		Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normalsDial[0], qtuser_3d::AttribueSlot::Normal, normalsDial.size() / 3);
		Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indicesDial[0], indicesDial.size());
		Qt3DRender::QGeometry* dialGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, indexAttribute);
		m_pDialEntity->setGeometry(dialGeo);


		// �̶ȳ�ʼ������ɫ��
		m_pDegreeEntity = new ManipulateEntity(nullptr, true, false, false);
		m_pDegreeEntity->setObjectName("RotateHelperEntity_T.degreeEntity");
		m_pDegreeEntity->setPose(m);
		m_pDegreeEntity->setColor(m_degreeColor);
		m_pDegreeEntity->setChangeColor(m_degreeColor);
		m_pDegreeEntity->setLightEnable(false);
		m_pDegreeEntity->setMethod(0);

		// �����̶�ģ������
		double degreeRadius = m_degreeRadius;
		double markOffset = m_markOffset;

		split = 60;
		roundDeltaRadians = 360 / split;
		roundDir = m_initRotateDir;
		roundDeltaQ = QQuaternion::fromAxisAndAngle(QVector3D(0.0, 0.0, 1.0), roundDeltaRadians);
		roundDirs.clear();
		for (int i = 0; i < split; i++)
		{
			roundDirs.push_back(roundDir);
			roundDir = roundDeltaQ * roundDir;
		}

		std::vector<float> positionsDegree;
		std::vector<float> normalsDegree;
		std::vector<int> indicesDegree;

		{
			// ���̶ȺͿ̶�����΢�Ĵ����Ա�����Ⱦ�ϵ��໥Ӱ��
			QVector3D roundCenter(0.0, 0.0, 0.001);
			std::vector<int> roundEdgeVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				QVector3D edgeVertex = roundCenter + roundDirs[i] * degreeRadius;
				QVector3D markVertex = roundCenter + roundDirs[i] * (degreeRadius + (i % 5 == 0 ? 2 * markOffset : markOffset));

				roundEdgeVIndices.push_back(positionsDegree.size() / 3);
				positionsDegree.push_back(edgeVertex.x());
				positionsDegree.push_back(edgeVertex.y());
				positionsDegree.push_back(edgeVertex.z());

				roundEdgeVIndices.push_back(positionsDegree.size() / 3);
				positionsDegree.push_back(markVertex.x());
				positionsDegree.push_back(markVertex.y());
				positionsDegree.push_back(markVertex.z());

				indicesDegree.push_back(roundEdgeVIndices[i * 2]);
				indicesDegree.push_back(roundEdgeVIndices[i * 2 + 1]);
			}

			for (int i = 0; i < roundEdgeVIndices.size(); i = i + 2)
			{
				int iNext = i < roundEdgeVIndices.size() - 2 ? i + 2 : 0;
				indicesDegree.push_back(roundEdgeVIndices[i]);
				indicesDegree.push_back(roundEdgeVIndices[iNext]);
			}
		}

		positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positionsDegree[0], qtuser_3d::AttribueSlot::Position, positionsDegree.size() / 3);
		//normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normalsDegree[0], qtuser_3d::AttribueSlot::Normal, 3 * normalsDegree.size());
		indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indicesDegree[0], indicesDegree.size());
		Qt3DRender::QGeometry* degreeGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, /*normalAttribute, */indexAttribute);
		m_pDegreeEntity->setGeometry(degreeGeo, Qt3DRender::QGeometryRenderer::Lines);


		// ʰȡ����ʼ��
		//m_pDialPickable = new ManipulatePickable(this);
		//m_pDialPickable->setPickableEntity(m_pHandleEntity);
		//m_pDialPickable->setShowEntity(m_pHandleEntity);
	}

	void RotateHelperEntity_T::initTip()
	{
		m_pTipEntity = new Qt3DExtras::QText2DEntity(m_pNoRotateGroup);
		m_pTipEntity->setFont(QFont("monospace", 5));
		m_pTipEntity->setText("TEST");
		m_pTipEntity->setColor(Qt::white);
		m_pTipEntity->setWidth(300);
		m_pTipEntity->setHeight(200);

		Qt3DCore::QTransform* trans = new Qt3DCore::QTransform;
		trans->setTranslation(QVector3D(100.0, 100.0, 100.0));
		m_pTipEntity->addComponent(trans);
	}

	void RotateHelperEntity_T::setVisibility(bool visibility)
	{
		visibility ? m_pDegreeEntity->setParent(m_pNoRotateGroup) : m_pDegreeEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pRingEntity->setParent(m_pRotateGroup) : m_pRingEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pHandlerEntity->setParent(m_pRotateGroup) : m_pHandlerEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pDialEntity->setParent(m_pRotateGroup) : m_pDialEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void RotateHelperEntity_T::setHandlerVisibility(bool visibility)
	{
		visibility ? m_pRingEntity->setParent(m_pRotateGroup) : m_pRingEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pHandlerEntity->setParent(m_pRotateGroup) : m_pHandlerEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void RotateHelperEntity_T::setDialVisibility(bool visibility)
	{
		visibility ? m_pDegreeEntity->setParent(m_pNoRotateGroup) : m_pDegreeEntity->setParent((Qt3DCore::QNode*)nullptr);
		//visibility ? m_pDialEntity->setParent(m_pRotateGroup) : m_pDialEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void RotateHelperEntity_T::setColor(QVector4D v4)
	{
		if (m_pRingEntity && m_pHandlerEntity)
		{
			m_pRingEntity->setColor(v4);
			m_pHandlerEntity->setColor(v4);
			v4.setW(0.2);
			m_pDialEntity->setColor(v4);
		}
	}
	void RotateHelperEntity_T::setRingColor(QVector4D color)
	{
		m_ringColor = color;
		if (m_pRingEntity)
			m_pRingEntity->setColor(m_ringColor);
	}

	void RotateHelperEntity_T::setDialColor(QVector4D color)
	{
		m_dialColor = color;
		if (m_pDialEntity)
			m_pDialEntity->setColor(m_dialColor);
	}

	void RotateHelperEntity_T::setHandlerColor(QVector4D color)
	{
		m_handlerColor = color;
		if (m_pHandlerEntity)
			m_pHandlerEntity->setColor(m_handlerColor);
	}

	void RotateHelperEntity_T::setHandlerPickedColor(QVector4D color)
	{
		m_handlerPickedColor = color;
		if (m_pHandlerEntity)
			m_pHandlerEntity->setColor(m_handlerPickedColor);
	}

	QVector3D RotateHelperEntity_T::center()
	{
		return m_center;
	}

	void RotateHelperEntity_T::setFixSize(bool fixSize)
	{
		m_fixSize = fixSize;
	}

	Pickable* RotateHelperEntity_T::getPickable()
	{
		return m_pHandlerPickable;
	}

	void RotateHelperEntity_T::setRotateAxis(QVector3D axis, double initAngle)
	{
		m_rotateAxis = axis;
		m_initRotateDirAngles = initAngle;

		QQuaternion initRotDir = QQuaternion::fromAxisAndAngle(m_rotateAxis, m_initRotateDirAngles);
		QQuaternion initRotation = QQuaternion::rotationTo(m_originRotateAxis, m_rotateAxis);
		m_initQuaternion = initRotDir * initRotation;
		m_pRotateTransform->setRotation(m_initQuaternion);
		m_pNoRotateTransform->setRotation(m_initQuaternion);

		m_initRotateDir = m_initQuaternion * m_originInitRotateDir;

		if (m_pRingEntity)
		{
			m_pRingEntity->setRotAxis(m_rotateAxis);
			m_pRingEntity->setRotInitDir(m_initRotateDir);
		}

		if (m_pDialEntity)
		{
			m_pDialEntity->setRotAxis(m_rotateAxis);
			m_pDialEntity->setRotInitDir(m_initRotateDir);
		}
	}

	void RotateHelperEntity_T::setPickSource(FacePicker* pickSource)
	{
		m_pPickSource = pickSource;
	}

	void RotateHelperEntity_T::setScreenCamera(ScreenCamera* camera)
	{
		m_pScreenCamera = camera;
	}

	void RotateHelperEntity_T::setRotateCallback(RotateCallback* callback)
	{
		m_pRotateCallback = callback;
	}

	void RotateHelperEntity_T::setScale(float scale)
	{
		m_pRotateTransform->setScale(m_initScaleRate * scale);
		m_pNoRotateTransform->setScale(m_initScaleRate * scale);
	}

	void RotateHelperEntity_T::onBoxChanged(Box3D box)
	{
		QVector3D center = box.center();
		m_center = center;

		float len = 1.0f;
		if (m_fixSize)
		{
			QVector3D sz = box.size();
			double maxlen = sz.x() > sz.y() ? sz.x() : sz.y();
			maxlen = maxlen > sz.z() ? maxlen : sz.z();
			if (maxlen * 1.1 < 50 && maxlen > 0.00000001)
			{
				len = maxlen * 1.1 / 50;
			}
		}

		m_initScaleRate = len;

		m_pRotateTransform->setTranslation(m_center);
		m_pRotateTransform->setScale(m_initScaleRate);

		m_pNoRotateTransform->setTranslation(m_center);
		m_pNoRotateTransform->setScale(m_initScaleRate);

		m_pRingEntity->setRotCenter(m_center);
		m_pDialEntity->setRotCenter(m_center);
	}

	void RotateHelperEntity_T::onLeftMouseButtonPress(QMouseEvent* event)
	{
		 QList<Pickable*> list = QList<Pickable*>({m_pHandlerPickable});
		Pickable* pickable = checkPickableFromList(m_pPickSource, event->pos(), list , nullptr);

		if (pickable == m_pHandlerPickable)
		{
			m_rotatingFlag = true;
			m_lastestRotAngles = 0;
			m_pRingEntity->setRotMode(1);

			m_spacePoint = calculateSpacePoint(event->pos());

			if (m_pRotateCallback)
				m_pRotateCallback->onStartRotate();

			//m_pTipEntity->setText("START");
		}
	}

	void RotateHelperEntity_T::onLeftMouseButtonRelease(QMouseEvent* event)
	{
		if (m_rotatingFlag)
		{
			perform(event->pos(), true);
			m_rotatingFlag = false;
			m_lastestRotAngles = 0;
			m_pRingEntity->setRotMode(0);
			m_pRingEntity->setRotRadians(0);
			m_pDialEntity->setRotRadians(0);

			//m_pTipEntity->setText("END");
		}
	}

	void RotateHelperEntity_T::onLeftMouseButtonMove(QMouseEvent* event)
	{
		if (m_rotatingFlag)
			perform(event->pos(), false);
	}

	void RotateHelperEntity_T::onWheelEvent(QWheelEvent* event)
	{
		//Qt3DRender::QCamera* mainCamera = m_pScreenCamera;
		//float curFovy = mainCamera->fieldOfView();
		//m_helperEntity->setScale(curFovy / m_originFovy);
	}

	QVector3D RotateHelperEntity_T::calculateSpacePoint(QPoint point)
	{
		QVector3D collide;
		QVector3D planeNormal = m_rotateAxis;
		QVector3D planePosition = m_center;

		cameraRayPoint(m_pScreenCamera, point, planePosition, planeNormal, collide);
		return collide;
	}

	QQuaternion RotateHelperEntity_T::process(QPoint point)
	{
		QVector3D p = calculateSpacePoint(point);

		QVector3D axis = m_rotateAxis;
		float angle = 0.0f;

		QVector3D delta;
		QVector3D oc0 = m_spacePoint - m_center;
		QVector3D oc1 = p - m_center;
		if (oc0.length() != 0.0f && oc1.length() != 0.0f)
		{
			oc0.normalize();
			oc1.normalize();

			double angleCC = 0.0, angleC = 0.0;
			if (oc0 == oc1)
			{
				angle = 0.0f;

				angleCC = 0.0;
				angleC = 0.0;
			}
			else if (oc0 == -oc1)
			{
				angle = 180.0f;

				angleCC = 180.0;
				angleC = -180.0;
			}
			else
			{
				axis = QVector3D::crossProduct(oc0, oc1);
				axis.normalize();
				angle = angleOfVector3D2(oc0, oc1);

				float axisInvertCos = QVector3D::dotProduct(m_rotateAxis, axis);
				if (axisInvertCos >= 0)
				{
					angleCC = angle;
					angleC = -(360 - angle);
				}
				else if (axisInvertCos < 0)
				{
					angleC = -angle;
					angleCC = 360 - angle;
				}
			}

			if (abs(angleCC - m_lastestRotAngles) <= abs(angleC - m_lastestRotAngles))
				m_lastestRotAngles = angleCC;
			else
				m_lastestRotAngles = angleC;

			m_pRingEntity->setRotRadians(qDegreesToRadians(m_lastestRotAngles));
			m_pDialEntity->setRotRadians(qDegreesToRadians(m_lastestRotAngles));
		}
		if (m_pRotateCallback)
		{
			m_pRotateCallback->setRotateAngle(axis, angle);
		}

		return QQuaternion::fromAxisAndAngle(axis, angle);
	}

	void RotateHelperEntity_T::perform(QPoint point, bool release)
	{
		QQuaternion q = process(point);

		m_pRotateTransform->setRotation(q * m_initQuaternion);

		if (m_pRotateCallback)
		{
			if (release)
			{
				m_pRotateCallback->onEndRotate(q);

				m_pRotateTransform->setRotation(m_initQuaternion);
			}
			else 
				m_pRotateCallback->onRotate(q);
		}
	}
}
