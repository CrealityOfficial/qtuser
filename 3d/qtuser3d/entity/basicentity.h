#ifndef QTUSER_3D_BASICENTITY_1594569444448_H
#define QTUSER_3D_BASICENTITY_1594569444448_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QGeometry>
#include <Qt3DRender/QEffect>

#include <Qt3DRender/QBlendEquationArguments>
namespace qtuser_3d
{
	class QTUSER_3D_API BasicEntity : public Qt3DCore::QEntity
	{
		Q_OBJECT
	public:
		BasicEntity(Qt3DCore::QNode* parent = nullptr);
		virtual ~BasicEntity();

		Qt3DRender::QParameter* createParameter(const QString& name, const QVariant& value);
		void setParameter(const QString& name, const QVariant& value);
		void destroyParameter(Qt3DRender::QParameter* parameter);

		void setEffect(Qt3DRender::QEffect* effect);
		Qt3DRender::QEffect* selectEffect(Qt3DRender::QEffect* effect);

		void setPose(const QMatrix4x4& matrix);
		QMatrix4x4 pose() const;
		void setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles);
		Qt3DRender::QGeometry* geometry();
		void replaceGeometryRenderer(Qt3DRender::QGeometryRenderer* geometryRenderer);

		Qt3DRender::QBlendEquationArguments* blendArguments();

		int traitPositionCount();
		void fillPositionData(char* buffer, int len);
	protected:
		Qt3DRender::QAttribute* positionAttribute();
	protected:
		Qt3DRender::QMaterial* m_material;
		Qt3DCore::QTransform* m_transform;
		Qt3DRender::QGeometryRenderer* m_geometryRenderer;
	};
}
#endif // QTUSER_3D_BASICENTITY_1594569444448_H