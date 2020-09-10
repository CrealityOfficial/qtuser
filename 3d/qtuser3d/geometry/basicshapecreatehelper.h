#ifndef _BASIC_SHAPE_CREATE_HELPER_H__
#define _BASIC_SHAPE_CREATE_HELPER_H__
#include "qtuser3d/geometry/geometrycreatehelper.h"
#include <vector>

namespace qtuser_3d
{
	class QTUSER_3D_API BasicShapeCreateHelper : public GeometryCreateHelper
	{
		Q_OBJECT
	public:
		BasicShapeCreateHelper(QObject* parent = nullptr);
		virtual ~BasicShapeCreateHelper();

		static Qt3DRender::QGeometry* createCylinder(Qt3DCore::QNode* parent = nullptr);

		static Qt3DRender::QGeometry* createRectangle(float w, float h, Qt3DCore::QNode* parent = nullptr);

		static Qt3DRender::QGeometry* createPen(Qt3DCore::QNode* parent = nullptr);

	protected:
		static int createCylinderData(float r, float h, int seg, std::vector<float> &datas);

		static int createPenData(float r, float headh, float bodyh, int seg, std::vector<float>& vertexDatas, std::vector<float>& normalDatas);

	private:
		static int addFaceDataWithQVector3D(const QVector3D& v1, const QVector3D& v2, const QVector3D& v3, const QVector3D& n, std::vector<float>& vertexDatas, std::vector<float>& normalDatas);

	};
}
#endif // QTUSER_3D_TRIANGLESCREATEHELPER_1594889735714_H