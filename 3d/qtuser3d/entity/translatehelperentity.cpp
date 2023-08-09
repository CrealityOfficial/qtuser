#include "qtuser3d/entity/translatehelperentity.h"
#include "qtuser3d/entity/manipulateentity.h"
#include "qtuser3d/module/manipulatepickable.h"
#include "qtuser3d/camera/cameracontroller.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/geometry/bufferhelper.h"
#include "qtuser3d/geometry/geometrycreatehelper.h"


namespace qtuser_3d
{
	TranslateHelperEntity::TranslateHelperEntity(Qt3DCore::QNode* parent, int type, IndicatorType shapetype)
		: XEntity(parent)
		, m_fixSize(0)
		, m_scale(70.0)
		, m_initAxisDir(0.0, 1.0, 0.0)
		, m_pXArrowEntity(nullptr)
		, m_pYArrowEntity(nullptr)
		, m_pZArrowEntity(nullptr)
		, m_pXArrowPickable(nullptr)
		, m_pYArrowPickable(nullptr)
		, m_pZArrowPickable(nullptr)
		, m_pXYPlaneEntity(nullptr)
		, m_pYZPlaneEntity(nullptr)
		, m_pZXPlaneEntity(nullptr)
		, m_pXYPlanePickable(nullptr)
		, m_pYZPlanePickable(nullptr)
		, m_pZXPlanePickable(nullptr)
		, m_pXYZCubeEntity(nullptr)
		, m_pXYZCubePickable(nullptr)
		, m_pCameraController(nullptr)
	{
		m_transform = new Qt3DCore::QTransform(this);
		addComponent(m_transform);

		initAxis(type, shapetype);
		initPlane(type);
		initCenterCube(type);

	}

	TranslateHelperEntity::~TranslateHelperEntity()
	{
		if (m_pXArrowEntity && m_pXArrowEntity->parent())
			delete m_pXArrowEntity;
		if (m_pYArrowEntity && m_pYArrowEntity->parent())
			delete m_pYArrowEntity;
		if (m_pZArrowEntity && m_pZArrowEntity->parent())
			delete m_pZArrowEntity;
	}

	void TranslateHelperEntity::setCameraController(CameraController* controller)
	{
		m_pCameraController = controller;

		slotCameraChanged(m_pCameraController->getViewPosition(), m_pCameraController->getViewupVector());
	}

	void TranslateHelperEntity::attach()
	{
		if (m_pCameraController)
		{
			connect(m_pCameraController, &CameraController::signalCameraChaged, this, &TranslateHelperEntity::slotCameraChanged);
			slotCameraChanged(m_pCameraController->getViewPosition(), m_pCameraController->getViewupVector());
		}

		//addSpaceTracer(this);
	}

	void TranslateHelperEntity::detach()
	{
		if (m_pCameraController)
			disconnect(m_pCameraController, &CameraController::signalCameraChaged, this, &TranslateHelperEntity::slotCameraChanged);

		//removeSpaceTracer(this);
	}

	Pickable* TranslateHelperEntity::xArrowPickable()
	{
		return m_pXArrowPickable;
	}

	Pickable* TranslateHelperEntity::yArrowPickable()
	{
		return m_pYArrowPickable;
	}

	Pickable* TranslateHelperEntity::zArrowPickable()
	{
		return m_pZArrowPickable;
	}

	Pickable* TranslateHelperEntity::xyPlanePickable()
	{
		return m_pXYPlanePickable;
	}

	Pickable* TranslateHelperEntity::yzPlanePickable()
	{
		return m_pYZPlanePickable;
	}

	Pickable* TranslateHelperEntity::zxPlanePickable()
	{
		return m_pZXPlanePickable;
	}

	Pickable* TranslateHelperEntity::xyzCubePickable()
	{
		return m_pXYZCubePickable;
	}

	void TranslateHelperEntity::setXVisibility(bool visibility)
	{
		if(m_pXArrowEntity)
			visibility ? m_pXArrowEntity->setParent(this) : m_pXArrowEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void TranslateHelperEntity::setYVisibility(bool visibility)
	{
		if(m_pYArrowEntity)
			visibility ? m_pYArrowEntity->setParent(this) : m_pYArrowEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void TranslateHelperEntity::setZVisibility(bool visibility)
	{
		if(m_pZArrowEntity)
			visibility ? m_pZArrowEntity->setParent(this) : m_pZArrowEntity->setParent((Qt3DCore::QNode*)nullptr);
	}

	void TranslateHelperEntity::setDirColor(QVector4D v4, int nDir)
	{
		if (nDir == 0 && m_pXArrowEntity)
		{   //x 轴
			m_pXArrowEntity->setColor(v4);
		}
		else if (nDir == 1 && m_pYArrowEntity)
		{
			m_pYArrowEntity->setColor(v4);
		
		}
		else if (nDir == 2 && m_pZArrowEntity)
		{
			m_pZArrowEntity->setColor(v4);

		}
	}

	void TranslateHelperEntity::initAxis(int helperType , IndicatorType shapeType)
	{
		Qt3DRender::QGeometry* geometry = nullptr;
		if (shapeType == IndicatorType::ARROW)
		{
			geometry = PRIMITIVESHAPE("arrow");
		}
		else if (shapeType == IndicatorType::CUBE)
		{
			geometry = PRIMITIVESHAPE("scaleindicator");
		}

		if (helperType & HELPERTYPE_AXIS_X)
		{
			QMatrix4x4 xMatrix;
			//xMatrix.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
			xMatrix.rotate(QQuaternion::rotationTo(m_initAxisDir, QVector3D(1.0, 0.0, 0.0)));
			xMatrix.scale(m_scale, m_scale, m_scale);
			//xMatrix.scale(15.0f, 15.0f, 15.0f);

			m_pXArrowEntity = new qtuser_3d::ManipulateEntity(this, false, true, false, true);
			m_pXArrowEntity->setObjectName("TranslateHelperEntity.xArrowEntity");
			m_pXArrowEntity->setPose(xMatrix);
			m_pXArrowEntity->setColor(QVector4D(1.0f, 0.1098f, 0.1098f, 1.0f));
			m_pXArrowEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pXArrowEntity->setChangeColorEnabled(1);
			m_pXArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
			//m_pXArrowEntity->setLightEnable(true);

			m_pXArrowPickable = new ManipulatePickable(this);
			m_pXArrowPickable->setPickableEntity(m_pXArrowEntity);
			m_pXArrowPickable->setEnableSelect(false);
		}
		if (helperType & HELPERTYPE_AXIS_Y)
		{
			QMatrix4x4 yMatrix;
			yMatrix.scale(m_scale, m_scale, m_scale);
			//yMatrix.scale(15.0f, 15.0f, 15.0f);
			m_pYArrowEntity = new qtuser_3d::ManipulateEntity(this, false, true, false, true);
			m_pYArrowEntity->setObjectName("TranslateHelperEntity.yArrowEntity");
			m_pYArrowEntity->setPose(yMatrix);
			m_pYArrowEntity->setColor(QVector4D(0.247f, 0.933f, 0.1098f, 1.0f));
			m_pYArrowEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pYArrowEntity->setChangeColorEnabled(1);
			m_pYArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
			//m_pYArrowEntity->setLightEnable(true);

			m_pYArrowPickable = new ManipulatePickable(this);
			m_pYArrowPickable->setPickableEntity(m_pYArrowEntity);
			m_pYArrowPickable->setEnableSelect(false);
		}
		if (helperType & HELPERTYPE_AXIS_Z)
		{
			QMatrix4x4 zMatrix;
			zMatrix.rotate(QQuaternion::rotationTo(m_initAxisDir, QVector3D(0.0, 0.0, 1.0)));
			zMatrix.scale(m_scale, m_scale, m_scale);
			//zMatrix.scale(15.0f, 15.0f, 15.0f);
			m_pZArrowEntity = new qtuser_3d::ManipulateEntity(this, false, true, false, true);
			m_pZArrowEntity->setObjectName("TranslateHelperEntity.zArrowEntity");
			m_pZArrowEntity->setPose(zMatrix);
			m_pZArrowEntity->setColor(QVector4D(0.4117f, 0.243f, 1.0f, 1.0f));
			m_pZArrowEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pZArrowEntity->setChangeColorEnabled(1);
			m_pZArrowEntity->setGeometry(geometry, Qt3DRender::QGeometryRenderer::Triangles);
			//m_pZArrowEntity->setLightEnable(true);

			m_pZArrowPickable = new ManipulatePickable(this);
			m_pZArrowPickable->setPickableEntity(m_pZArrowEntity);
			m_pZArrowPickable->setEnableSelect(false);
		}
	}

	void TranslateHelperEntity::initPlane(int helperType)
	{
		// 在 XOY 平面的第一象限构建 平面操作器 模型数据
		double length = 0.3;
		double axisOffset = 0.14;

		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<int> indices;
		QVector3D initPlaneDirX(1.0, 0.0, 0.0);
		QVector3D initPlaneDirY(0.0, 1.0, 0.0);

		// 平面
		{
			std::vector<int> sideVIndices;
			{
				sideVIndices.push_back(positions.size() / 3);
				positions.push_back(axisOffset);
				positions.push_back(axisOffset);
				positions.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(1.0);

				sideVIndices.push_back(positions.size() / 3);
				positions.push_back(axisOffset + length);
				positions.push_back(axisOffset);
				positions.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(1.0);

				sideVIndices.push_back(positions.size() / 3);
				positions.push_back(axisOffset + length);
				positions.push_back(axisOffset + length);
				positions.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(1.0);

				sideVIndices.push_back(positions.size() / 3);
				positions.push_back(axisOffset);
				positions.push_back(axisOffset + length);
				positions.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(0.0);
				normals.push_back(1.0);
			}

			{
				indices.push_back(sideVIndices[0]);
				indices.push_back(sideVIndices[3]);
				indices.push_back(sideVIndices[1]);

				indices.push_back(sideVIndices[3]);
				indices.push_back(sideVIndices[2]);
				indices.push_back(sideVIndices[1]);
			}
		}

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0], qtuser_3d::AttribueSlot::Position, positions.size() / 3);
		Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0], qtuser_3d::AttribueSlot::Normal, normals.size() / 3);
		Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0], indices.size());
		Qt3DRender::QGeometry* planeGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, indexAttribute);

		if (helperType & HELPERTYPE_PLANE_XY)
		{
			QMatrix4x4 xMatrix;
			xMatrix.rotate(m_initXYPlaneQ);
			xMatrix.scale(m_scale, m_scale, m_scale);

			m_pXYPlaneEntity = new qtuser_3d::ManipulateEntity(this, true, true);
			m_pXYPlaneEntity->setObjectName("TranslateHelperEntity.xyPlaneEntity");
			m_pXYPlaneEntity->setPose(xMatrix);
			m_pXYPlaneEntity->setColor(QVector4D(0.4117f, 0.243f, 1.0f, 0.5f));
			m_pXYPlaneEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pXYPlaneEntity->setChangeColorEnabled(1);
			m_pXYPlaneEntity->setGeometry(planeGeo, Qt3DRender::QGeometryRenderer::Triangles);

			m_pXYPlanePickable = new ManipulatePickable(this);
			m_pXYPlanePickable->setPickableEntity(m_pXYPlaneEntity);
			m_pXYPlanePickable->setEnableSelect(false);
		}
		if (helperType & HELPERTYPE_PLANE_YZ)
		{
			QMatrix4x4 yMatrix;
			m_initYZPlaneQ = QQuaternion::rotationTo(initPlaneDirX, QVector3D(0.0, 0.0, 1.0)) * QQuaternion::rotationTo(initPlaneDirY, QVector3D(0.0, 1.0, 0.0));
			yMatrix.rotate(m_initYZPlaneQ);
			yMatrix.scale(m_scale, m_scale, m_scale);
			m_pYZPlaneEntity = new qtuser_3d::ManipulateEntity(this, true, true);
			m_pYZPlaneEntity->setObjectName("TranslateHelperEntity.yzPlaneEntity");
			m_pYZPlaneEntity->setPose(yMatrix);
			m_pYZPlaneEntity->setColor(QVector4D(1.0f, 0.1098f, 0.1098f, 0.5f));
			m_pYZPlaneEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pYZPlaneEntity->setChangeColorEnabled(1);
			m_pYZPlaneEntity->setGeometry(planeGeo, Qt3DRender::QGeometryRenderer::Triangles);

			m_pYZPlanePickable = new ManipulatePickable(this);
			m_pYZPlanePickable->setPickableEntity(m_pYZPlaneEntity);
			m_pYZPlanePickable->setEnableSelect(false);
		}
		if (helperType & HELPERTYPE_PLANE_ZX)
		{
			QMatrix4x4 zMatrix;
			m_initZXPlaneQ = QQuaternion::rotationTo(initPlaneDirX, QVector3D(1.0, 0.0, 0.0)) * QQuaternion::rotationTo(initPlaneDirY, QVector3D(0.0, 0.0, 1.0));
			zMatrix.rotate(m_initZXPlaneQ);
			zMatrix.scale(m_scale, m_scale, m_scale);
			m_pZXPlaneEntity = new qtuser_3d::ManipulateEntity(this, true, true);
			m_pZXPlaneEntity->setObjectName("TranslateHelperEntity.zxPlaneEntity");
			m_pZXPlaneEntity->setPose(zMatrix);
			m_pZXPlaneEntity->setColor(QVector4D(0.247f, 0.933f, 0.1098f, 0.5f));
			m_pZXPlaneEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pZXPlaneEntity->setChangeColorEnabled(1);
			m_pZXPlaneEntity->setGeometry(planeGeo, Qt3DRender::QGeometryRenderer::Triangles);

			m_pZXPlanePickable = new ManipulatePickable(this);
			m_pZXPlanePickable->setPickableEntity(m_pZXPlaneEntity);
			m_pZXPlanePickable->setEnableSelect(false);
		}
	}

	void TranslateHelperEntity::initCenterCube(int helperType)
	{
		// 构建 中心操作器 模型数据
		double length = 0.2;
		double halfLength = length / 2.0;

		std::vector<float> positions;
		std::vector<float> normals;
		std::vector<int> indices;

		// 平面
		{
			std::vector<int> sideVIndices;
			{
				for (int i = -1; i < 2; i = i + 2)
				{
					for (int j = -1; j < 2; j = j + 2)
					{
						for (int k = -1; k < 2; k = k + 2)
						{
							sideVIndices.push_back(positions.size() / 3);
							positions.push_back(0.0 + i * halfLength);
							positions.push_back(0.0 + j * halfLength);
							positions.push_back(0.0 + k * halfLength);
							normals.push_back(0.0 + i * halfLength);
							normals.push_back(0.0 + j * halfLength);
							normals.push_back(0.0 + k * halfLength);
						}
					}
				}
			}

			{
				indices.push_back(sideVIndices[0]);
				indices.push_back(sideVIndices[1]);
				indices.push_back(sideVIndices[2]);

				indices.push_back(sideVIndices[3]);
				indices.push_back(sideVIndices[2]);
				indices.push_back(sideVIndices[1]);

				indices.push_back(sideVIndices[4]);
				indices.push_back(sideVIndices[6]);
				indices.push_back(sideVIndices[5]);

				indices.push_back(sideVIndices[6]);
				indices.push_back(sideVIndices[7]);
				indices.push_back(sideVIndices[5]);

				indices.push_back(sideVIndices[0]);
				indices.push_back(sideVIndices[4]);
				indices.push_back(sideVIndices[1]);

				indices.push_back(sideVIndices[4]);
				indices.push_back(sideVIndices[5]);
				indices.push_back(sideVIndices[1]);

				indices.push_back(sideVIndices[1]);
				indices.push_back(sideVIndices[5]);
				indices.push_back(sideVIndices[3]);

				indices.push_back(sideVIndices[5]);
				indices.push_back(sideVIndices[7]);
				indices.push_back(sideVIndices[3]);

				indices.push_back(sideVIndices[6]);
				indices.push_back(sideVIndices[2]);
				indices.push_back(sideVIndices[7]);

				indices.push_back(sideVIndices[2]);
				indices.push_back(sideVIndices[3]);
				indices.push_back(sideVIndices[7]);

				indices.push_back(sideVIndices[0]);
				indices.push_back(sideVIndices[2]);
				indices.push_back(sideVIndices[4]);

				indices.push_back(sideVIndices[4]);
				indices.push_back(sideVIndices[2]);
				indices.push_back(sideVIndices[6]);
			}
		}

		Qt3DRender::QAttribute* positionAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&positions[0], qtuser_3d::AttribueSlot::Position, positions.size() / 3);
		Qt3DRender::QAttribute* normalAttribute = qtuser_3d::BufferHelper::CreateVertexAttribute((const char*)&normals[0], qtuser_3d::AttribueSlot::Normal, normals.size() / 3);
		Qt3DRender::QAttribute* indexAttribute = qtuser_3d::BufferHelper::CreateIndexAttribute((const char*)&indices[0], indices.size());
		Qt3DRender::QGeometry* cubeGeo = qtuser_3d::GeometryCreateHelper::createEx(nullptr, positionAttribute, normalAttribute, indexAttribute);

		if (helperType & HELPERTYPE_CUBE_XYZ)
		{
			QMatrix4x4 mt;
			mt.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
			mt.scale(m_scale, m_scale, m_scale);
			//xMatrix.scale(15.0f, 15.0f, 15.0f);

			m_pXYZCubeEntity = new qtuser_3d::ManipulateEntity(this, true, true);
			m_pXYZCubeEntity->setObjectName("TranslateHelperEntity.xyzCubeEntity");
			m_pXYZCubeEntity->setPose(mt);
			m_pXYZCubeEntity->setColor(QVector4D(1.0f, 1.0f, 1.0f, 0.1f));
			m_pXYZCubeEntity->setChangeColor(QVector4D(1.0f, 0.79f, 0.0f, 1.0f));
			m_pXYZCubeEntity->setChangeColorEnabled(1);
			m_pXYZCubeEntity->setGeometry(cubeGeo, Qt3DRender::QGeometryRenderer::Triangles);

			m_pXYZCubePickable = new ManipulatePickable(this);
			m_pXYZCubePickable->setPickableEntity(m_pXYZCubeEntity);
			m_pXYZCubePickable->setEnableSelect(false);
		}
	}

	void TranslateHelperEntity::slotCameraChanged(QVector3D position, const QVector3D upVector)
	{
		QVector3D viewDir = position;
		QVector3D newXDir(position.x(), 0.0, 0.0);
		QVector3D newYDir(0.0, position.y(), 0.0);
		QVector3D newZDir(0.0, 0.0, position.z());
		newXDir.normalize();
		newYDir.normalize();
		newZDir.normalize();

		if (newXDir.length() == 0.0)
		{
			newXDir.setX(1.0);
		}

		if (newYDir.length() == 0.0)
		{
			newYDir.setY(1.0);
		}

		if (newZDir.length() == 0.0)
		{
			newZDir.setZ(1.0);
		}

		QVector3D initAxisDir = m_initAxisDir;
		if (m_pXArrowEntity)
		{
			QMatrix4x4 newMt;
			newMt.scale(QVector3D(m_scale, m_scale, m_scale));
			newMt.rotate(QQuaternion::rotationTo(initAxisDir, newXDir));
			m_pXArrowEntity->setPose(newMt);
		}

		if (m_pYArrowEntity)
		{
			QMatrix4x4 newMt;
			newMt.scale(QVector3D(m_scale, m_scale, m_scale));
			newMt.rotate(QQuaternion::rotationTo(initAxisDir, newYDir));
			m_pYArrowEntity->setPose(newMt);
		}

		if (m_pZArrowEntity)
		{
			QMatrix4x4 newMt;
			newMt.scale(QVector3D(m_scale, m_scale, m_scale));
			newMt.rotate(QQuaternion::rotationTo(initAxisDir, newZDir));
			m_pZArrowEntity->setPose(newMt);
		}

		if (m_pXYPlaneEntity)
		{
			QMatrix4x4 newMt;
			newMt.scale(QVector3D(m_scale, m_scale, m_scale));
			newMt.rotate(QQuaternion::rotationTo(QVector3D(1.0, 1.0, 0.0), newXDir + newYDir) * m_initXYPlaneQ);
			m_pXYPlaneEntity->setPose(newMt);
		}

		if (m_pYZPlaneEntity)
		{
			QMatrix4x4 newMt;
			newMt.scale(QVector3D(m_scale, m_scale, m_scale));
			newMt.rotate(QQuaternion::rotationTo(QVector3D(0.0, 1.0, 1.0), newZDir + newYDir) * m_initYZPlaneQ);
			m_pYZPlaneEntity->setPose(newMt);
		}

		if (m_pZXPlaneEntity)
		{
			QMatrix4x4 newMt;
			newMt.scale(QVector3D(m_scale, m_scale, m_scale));
			newMt.rotate(QQuaternion::rotationTo(QVector3D(1.0, 0.0, 1.0), newXDir + newZDir) * m_initZXPlaneQ);
			m_pZXPlaneEntity->setPose(newMt);
		}

		if (m_pXYZCubeEntity)
		{
			QMatrix4x4 mt;
			mt.rotate(-90.0f, 0.0f, 0.0f, 1.0f);
			mt.scale(m_scale, m_scale, m_scale);
			m_pXYZCubeEntity->setPose(mt);
		}
	}

	void TranslateHelperEntity::setScale(float scaleRate)
	{
		m_transform->setScale(scaleRate);
	}

	void TranslateHelperEntity::setEntitiesLocalScale(float scale)
	{
		m_scale = scale;
		slotCameraChanged(m_pCameraController->getViewPosition(), m_pCameraController->getViewupVector());
	}

	void TranslateHelperEntity::setRotation(QQuaternion rotQ)
	{
		m_initRotationQ = rotQ;
		m_transform->setRotation(m_initRotationQ);
	}

	void TranslateHelperEntity::updateBox(const Box3D& box)
	{
		QVector3D center = box.center();
		QMatrix4x4 matrix;
		matrix.translate(center);
		m_center = center;

		float len = 1.0f;
		if (m_fixSize == 1)
		{
			QVector3D sz = box.size();
			double maxlen = sz.x() > sz.y() ? sz.x() : sz.y();
			maxlen = maxlen > sz.z() ? maxlen : sz.z();
			if (maxlen > 80)
			{
				len = maxlen / 80;
			}
		}
		else if (m_fixSize == 2)
		{
			QVector3D sz = box.size();
			double maxlen = sz.x() > sz.y() ? sz.x() : sz.y();
			maxlen = maxlen > sz.z() ? maxlen : sz.z();
			if (maxlen * 1.1 < 44 && maxlen > 0.00000001)
			{
				len = maxlen * 1.1 / 44;
			}
		}

		matrix.scale(len);
		m_transform->setMatrix(matrix);

		qDebug() << "far plane" << m_pCameraController->screenCamera()->camera()->farPlane();
	}

	void TranslateHelperEntity::setFixSize(int fixSize)
	{
		m_fixSize = fixSize;
	}

	QVector3D TranslateHelperEntity::center()
	{
		return m_center;
	}
}
