#include "qtuser3d/entity/RotateHelperEntity_T.h"

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
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QConeMesh>
#include <QtCore/qmath.h>
#include <Qt3DCore/QTransform>
#include <QtGui/QTransform>

#include "qtuser3d/module/pickable.h"
namespace qtuser_3d
{
	struct vec3d
	{
		float x;
		float y;
		float z;
	};
	struct vec3i
	{
		int x;
		int y;
		int z;
	};

	RotateHelperEntity_T::RotateHelperEntity_T(Qt3DCore::QNode* parent)
		:Qt3DCore::QEntity(parent)
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
		, m_handlerPickedColor(-0.2f, 0.3f, 0.2f, 0.0f)
		, m_pPickSource(nullptr)
		, m_pScreenCamera(nullptr)
		, m_pRotateCallback(nullptr)
		, m_scale(25.0f, 25.0f, 25.0f)
		, m_ringRadius(2)
		, m_ringMinorRadius(0.02)
		, m_dialRadius(2)
		, m_degreeRadius(1.5)
		, m_markOffset(0.2)
		, m_handlerOffset(2.16)
		, m_fixSize(false)
	{
		m_pGlobalTransform = new Qt3DCore::QTransform(this);
		addComponent(m_pGlobalTransform);

		m_pRotateGroup = new QEntity(this);
		m_pRotateTransform = new Qt3DCore::QTransform(m_pRotateGroup);
		m_pRotateGroup->addComponent(m_pRotateTransform);

		m_pNoRotateGroup = new QEntity(this);
		m_pNoRotateTransform = new Qt3DCore::QTransform(m_pNoRotateGroup);
		m_pNoRotateGroup->addComponent(m_pNoRotateTransform);

		m_initQuaternion = QQuaternion::rotationTo(m_originRotateAxis, m_rotateAxis);

		initRing();
		initHandler();
		initDial();
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

		// 旋转轨初始化
		m_pRingEntity = new PieFadeEntity(m_pRotateGroup, 1);
		m_pRingEntity->setObjectName("RotateHelperEntity_T.ringEntity");
		m_pRingEntity->setPose(m);
		m_pRingEntity->setColor(m_ringColor);
		m_pRingEntity->setChangeColor(m_ringColor);
		m_pRingEntity->setMethod(1);
		m_pRingEntity->setRotMode(0);
		m_pRingEntity->setRotInitDir(m_initRotateDir);
		m_pRingEntity->setRotAxis(m_rotateAxis);

		Qt3DExtras::QTorusMesh* torusMesh = new Qt3DExtras::QTorusMesh(this);
		torusMesh->setRadius(m_ringRadius);
		torusMesh->setMinorRadius(m_ringMinorRadius);
		torusMesh->setRings(100);
		m_pRingEntity->replaceGeometryRenderer(torusMesh);

		//if (type == 1)
		//{
		//	float sf[3] = { 0, 1, 2 };
		//	m_pRingPickable->setStateFactor(sf);
		//}

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

		// 旋转手柄初始化
		m_pHandlerEntity = new ManipulateEntity(m_pRotateGroup, 0);
		m_pHandlerEntity->setObjectName("RotateHelperEntity_T.handleEntity");
		m_pHandlerEntity->setPose(m);
		m_pHandlerEntity->setColor(m_handlerColor);
		m_pHandlerEntity->setChangeColor(m_handlerPickedColor);
		m_pHandlerEntity->setMethod(1);

		// 构建手柄模型数据
		double bottomRadius = m_ringRadius / 30.0;
		double coneHeight = m_ringRadius * 3.0 / 25.0;
		double cylinderHeight = m_ringRadius * 3.0 / 25.0;
		double gapLength = m_ringRadius / 40.0;
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

		std::vector<vec3d> positions;
		std::vector<vec3d> normals;
		std::vector<vec3i> indices;

		// 中间圆柱
		{
			int topCenterIndex = positions.size();
			QVector3D topCenter(0.0f, 0.0f, (float)(cylinderHeight / 2.0));
			positions.push_back({ topCenter.x(), topCenter.y(), topCenter.z() });
			normals.push_back({ 0.0f, 0.0f, 1.0f });
			int bottomCenterIndex = positions.size();
			QVector3D bottomCenter(0.0f, 0.0f, -(float)(cylinderHeight / 2.0));
			positions.push_back({ bottomCenter.x(), bottomCenter.y(), bottomCenter.z() });
			normals.push_back({ 0.0f, 0.0f, -1.0f });

			std::vector<int> topVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				topVIndices.push_back(positions.size());
				QVector3D topVertex = topCenter + roundDirs[i] * bottomRadius;
				positions.push_back({ topVertex.x(), topVertex.y(), topVertex.z() });
				normals.push_back({ roundDirs[i].x(), roundDirs[i].y(), roundDirs[i].z() });
			}

			std::vector<int> bottomVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				bottomVIndices.push_back(positions.size());
				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				positions.push_back({ bottomVertex.x(), bottomVertex.y(), bottomVertex.z() });
				normals.push_back({ roundDirs[i].x(), roundDirs[i].y(), roundDirs[i].z() });
			}

			for (int i = 0; i < bottomVIndices.size(); i++)
			{
				int iNext = i < bottomVIndices.size() - 1 ? i + 1 : 0;
				indices.push_back({ topVIndices[i], topVIndices[iNext], topCenterIndex });
				indices.push_back({ topVIndices[i], bottomVIndices[i], topVIndices[iNext] });
				indices.push_back({ bottomVIndices[i], bottomVIndices[iNext], topVIndices[iNext] });
				indices.push_back({ bottomVIndices[i], bottomCenterIndex, bottomVIndices[iNext] });
			}
		}

		// 上圆锥
		{
			int topCenterIndex = positions.size();
			positions.push_back({ 0.0f, 0.0f, (float)(coneHeight + gapLength + cylinderHeight / 2.0) });
			normals.push_back({0.0f, 0.0f, 1.0f});
			int bottomCenterIndex = positions.size();
			QVector3D bottomCenter(0.0f, 0.0f, (float)(gapLength + cylinderHeight / 2.0));
			positions.push_back({ bottomCenter.x(), bottomCenter.y(), bottomCenter.z() });
			normals.push_back({0.0f, 0.0f, -1.0f});
			
			std::vector<int> bottomVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				bottomVIndices.push_back(positions.size());
				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				positions.push_back({ bottomVertex.x(), bottomVertex.y(), bottomVertex.z() });
				normals.push_back({roundDirs[i].x(), roundDirs[i].y(), roundDirs[i].z()});
			}

			for (int i = 0; i < bottomVIndices.size(); i++)
			{
				int iNext = i < bottomVIndices.size() - 1 ? i + 1 : 0;
				indices.push_back({ bottomVIndices[i], bottomVIndices[iNext], topCenterIndex });
				indices.push_back({bottomVIndices[i], bottomCenterIndex, bottomVIndices[iNext] });
			}
		}

		// 下圆锥
		{
			int topCenterIndex = positions.size();
			positions.push_back({ 0.0f, 0.0f, - (float)(coneHeight + gapLength + cylinderHeight / 2.0) });
			normals.push_back({ 0.0f, 0.0f, -1.0f });
			int bottomCenterIndex = positions.size();
			QVector3D bottomCenter(0.0f, 0.0f, - (float)(gapLength + cylinderHeight / 2.0));
			positions.push_back({ bottomCenter.x(), bottomCenter.y(), bottomCenter.z() });
			normals.push_back({ 0.0f, 0.0f, 1.0f });

			std::vector<int> bottomVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				bottomVIndices.push_back(positions.size());
				QVector3D bottomVertex = bottomCenter + roundDirs[i] * bottomRadius;
				positions.push_back({ bottomVertex.x(), bottomVertex.y(), bottomVertex.z() });
				normals.push_back({ roundDirs[i].x(), roundDirs[i].y(), roundDirs[i].z() });
			}

			for (int i = 0; i < bottomVIndices.size(); i++)
			{
				int iNext = i < bottomVIndices.size() - 1 ? i + 1 : 0;
				indices.push_back({ bottomVIndices[i], topCenterIndex, bottomVIndices[iNext] });
				indices.push_back({ bottomVIndices[i], bottomVIndices[iNext], bottomCenterIndex });
			}
		}

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0], qtuser_3d::AttribueSlot::Position, 3 * positions.size());
		Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0], qtuser_3d::AttribueSlot::Normal, 3 * normals.size());
		Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0], 3 * indices.size());
		Qt3DRender::QGeometry* handlerGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, indexAttribute);
		m_pHandlerEntity->setGeometry(handlerGeo);

		m_pHandlerPickable = new ManipulatePickable(this);
		m_pHandlerPickable->setPickableEntity(m_pHandlerEntity);
		m_pHandlerPickable->setShowEntity(m_pHandlerEntity);
	}

	void RotateHelperEntity_T::initDial()
	{
		QMatrix4x4 m;
		m.scale(m_scale);

		// 刻度盘初始化
		m_pDialEntity = new PieFadeEntity(nullptr, 0);
		m_pDialEntity->setObjectName("RotateHelperEntity_T.dialEntity");
		m_pDialEntity->setPose(m);
		m_pDialEntity->setColor(m_dialColor);
		m_pDialEntity->setChangeColor(m_dialColor);
		m_pDialEntity->setMethod(1);
		m_pDialEntity->setRotMode(1);
		m_pDialEntity->setRotRadians(0);
		m_pDialEntity->setRotInitDir(m_initRotateDir);
		m_pDialEntity->setRotAxis(m_rotateAxis);

		// 构建刻度盘模型数据
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

		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<int> indices;

		// 圆盘
		{
			int centerIndex = positions.size() / 3;
			QVector3D roundCenter(0.0, 0.0, 0.0);
			positions.push_back(0.0f);
			positions.push_back(0.0f);
			positions.push_back(0.0f);
			normals.push_back(0.0f);
			normals.push_back(0.0f);
			normals.push_back(1.0f);

			std::vector<int> roundEdgeVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				roundEdgeVIndices.push_back(positions.size() / 3);
				QVector3D edgeVertex = roundCenter + roundDirs[i] * radius;
				positions.push_back(edgeVertex.x());
				positions.push_back(edgeVertex.y());
				positions.push_back(edgeVertex.z());
				normals.push_back(0.0f);
				normals.push_back(0.0f);
				normals.push_back(1.0f);
			}

			for (int i = 0; i < roundEdgeVIndices.size(); i++)
			{
				int iNext = i < roundEdgeVIndices.size() - 1 ? i + 1 : 0;
				indices.push_back(roundEdgeVIndices[i]);
				indices.push_back(centerIndex);
				indices.push_back(roundEdgeVIndices[iNext]);

				indices.push_back(roundEdgeVIndices[i]);
				indices.push_back(roundEdgeVIndices[iNext]);
				indices.push_back(centerIndex);
			}
		}

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0], qtuser_3d::AttribueSlot::Position, positions.size());
		Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0], qtuser_3d::AttribueSlot::Normal, normals.size());
		Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0], indices.size());
		Qt3DRender::QGeometry* dialGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, indexAttribute);
		m_pDialEntity->setGeometry(dialGeo);


		// 刻度初始化
		m_pDegreeEntity = new ManipulateEntity(nullptr, 0);
		m_pDegreeEntity->setObjectName("RotateHelperEntity_T.degreeEntity");
		m_pDegreeEntity->setPose(m);
		m_pDegreeEntity->setColor(m_degreeColor);
		m_pDegreeEntity->setChangeColor(m_degreeColor);
		m_pDegreeEntity->setMethod(1);

		// 构建刻度模型数据
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

		positions.clear();
		//normals.clear();
		indices.clear();

		{
			QVector3D roundCenter(0.0, 0.0, 0.0);
			std::vector<int> roundEdgeVIndices;
			for (int i = 0; i < roundDirs.size(); i++)
			{
				QVector3D edgeVertex = roundCenter + roundDirs[i] * degreeRadius;
				QVector3D markVertex = roundCenter + roundDirs[i] * (degreeRadius + (i % 5 == 0 ? 2 * markOffset : markOffset));

				roundEdgeVIndices.push_back(positions.size() / 3);
				positions.push_back(edgeVertex.x());
				positions.push_back(edgeVertex.y());
				positions.push_back(edgeVertex.z());

				roundEdgeVIndices.push_back(positions.size() / 3);
				positions.push_back(markVertex.x());
				positions.push_back(markVertex.y());
				positions.push_back(markVertex.z());

				indices.push_back(roundEdgeVIndices[i * 2]);
				indices.push_back(roundEdgeVIndices[i * 2 + 1]);
			}

			for (int i = 0; i < roundEdgeVIndices.size(); i = i + 2)
			{
				int iNext = i < roundEdgeVIndices.size() - 2 ? i + 2 : 0;
				indices.push_back(roundEdgeVIndices[i]);
				indices.push_back(roundEdgeVIndices[iNext]);
			}
		}

		positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0], qtuser_3d::AttribueSlot::Position, positions.size());
		//normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0], qtuser_3d::AttribueSlot::Normal, 3 * normals.size());
		indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0], indices.size());
		Qt3DRender::QGeometry* degreeGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, /*normalAttribute, */indexAttribute);
		m_pDegreeEntity->setGeometry(degreeGeo, Qt3DRender::QGeometryRenderer::Lines);


		// 拾取器初始化
		//m_pDialPickable = new ManipulatePickable(this);
		//m_pDialPickable->setPickableEntity(m_pHandleEntity);
		//m_pDialPickable->setShowEntity(m_pHandleEntity);
	}

	void RotateHelperEntity_T::setVisibility(bool visibility)
	{
		visibility ? m_pRingEntity->setParent(m_pRotateGroup) : m_pRingEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pHandlerEntity->setParent(m_pRotateGroup) : m_pHandlerEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pDialEntity->setParent(m_pRotateGroup) : m_pDialEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pDegreeEntity->setParent(m_pNoRotateGroup) : m_pDegreeEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void RotateHelperEntity_T::setHandlerVisibility(bool visibility)
	{
		visibility ? m_pRingEntity->setParent(m_pRotateGroup) : m_pRingEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pHandlerEntity->setParent(m_pRotateGroup) : m_pHandlerEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void RotateHelperEntity_T::setDialVisibility(bool visibility)
	{
		visibility ? m_pDialEntity->setParent(m_pRotateGroup) : m_pDialEntity->setParent((Qt3DCore::QNode*)nullptr);
		visibility ? m_pDegreeEntity->setParent(m_pNoRotateGroup) : m_pDegreeEntity->setParent((Qt3DCore::QNode*)nullptr);
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

	void RotateHelperEntity_T::setRotateInitAngle(double angle)
	{
		m_initRotateDirAngles = angle;

		QQuaternion initRotDir = QQuaternion::fromAxisAndAngle(m_rotateAxis, m_initRotateDirAngles);
		QQuaternion initRotation = QQuaternion::rotationTo(m_originRotateAxis, m_rotateAxis);
		m_initQuaternion = initRotDir * initRotation;
		m_pRotateTransform->setRotation(m_initQuaternion);
		m_pNoRotateTransform->setRotation(m_initQuaternion);

		m_initRotateDir = m_initQuaternion * m_originInitRotateDir;

		if (m_pRingEntity)
		{
			m_pRingEntity->setRotInitDir(m_initRotateDir);
		}

		if (m_pDialEntity)
		{
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

		m_pRotateTransform->setTranslation(m_center);
		m_pRotateTransform->setScale(len);

		m_pNoRotateTransform->setTranslation(m_center);
		m_pNoRotateTransform->setScale(len);

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
			m_pDialEntity->setRotRadians(0);
		}
	}

	void RotateHelperEntity_T::onLeftMouseButtonMove(QMouseEvent* event)
	{
		if (m_rotatingFlag)
			perform(event->pos(), false);
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

			double angleCC, angleC;
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
				if (axisInvertCos > 0)
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

		//m_pGlobalTransform->setRotation(q * m_initQuaternion);
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
