#include "facepickable.h"
#include "qtuser3d/entity/pickfaceentity.h"

namespace qtuser_3d
{
	FacePickable::FacePickable(trimesh::TriMesh* mesh,QObject* parent)
		:Pickable(parent)
		, m_mesh(mesh)
	{
		m_face = new PickFaceEntity(nullptr);
		std::vector<QVector3D> vertexData;
		for (size_t i = 0; i < mesh->vertices.size(); i++)
		{
			QVector3D newPosition = QVector3D(mesh->vertices[i].x, mesh->vertices[i].y, mesh->vertices[i].z);
			vertexData.push_back(newPosition);
		}
		m_face->updateData(vertexData);
		m_mesh->normals[0];

		setEnableSelect(false);
	}

	FacePickable::~FacePickable()
	{
		m_face->setParent((Qt3DCore::QNode*)nullptr);
		delete m_face;
	}

	int FacePickable::primitiveNum()
	{
		return m_mesh->faces.size();
	}


	void FacePickable::setEntity(PickFaceEntity* faceEntity)
	{
		m_face = faceEntity;
	}


	trimesh::vec3 FacePickable::gNormal()
	{
		return m_mesh->normals.at(0);
	}

	qtuser_3d::PickFaceEntity* FacePickable::getEntity()
	{
		if (m_face)
		{
			return m_face;
		}
		return nullptr;
	}

	void FacePickable::onStateChanged(ControlState state)
	{
		if (m_face)
		{
			m_face->setState((float)state);
			if (state == ControlState::hover)
			{
				m_face->setColor(QVector4D(1.0f, 0.824f, 0.0f, 1.0f));
			}
			else
			{
				m_face->setColor(QVector4D(1.0f, 1.0f, 1.0f, 0.4f));
			}
			
		}
	}

	void FacePickable::faceBaseChanged(int faceBase)
	{
		if (m_face)
		{
			QPoint vertexBase(0,0);
			vertexBase.setX(faceBase * 3);
			vertexBase.setY(m_noPrimitive ? 1 : 0);
			m_face->setVertexBase(vertexBase);
		}
	}
}
