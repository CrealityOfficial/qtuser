#ifndef _QTUSER_3D_FACEPICKABLE_1639375311688_H
#define _QTUSER_3D_FACEPICKABLE_1639375311688_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/module/pickable.h"
#include "trimesh2/TriMesh.h"

namespace qtuser_3d
{
	class PickFaceEntity;
	class QTUSER_3D_API FacePickable : public Pickable
	{
		Q_OBJECT
	public:
		FacePickable(trimesh::TriMesh* mesh,QObject* parent = nullptr);
		virtual ~FacePickable();
		void setEntity(PickFaceEntity* faceEntity);
		trimesh::vec3 gNormal();
		PickFaceEntity* getEntity();
	protected:
		int primitiveNum() override;
		void onStateChanged(ControlState state) override;
		void faceBaseChanged(int faceBase) override;
	private:
		trimesh::TriMesh* m_mesh;
		PickFaceEntity* m_face;
	};
}
#endif // _QTUSER_3D_FACEPICKABLE_1639375311688_H
