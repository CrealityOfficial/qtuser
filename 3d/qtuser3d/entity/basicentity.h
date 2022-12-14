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
#include "qtuser3d/geometry/attribute.h"
#include "qtuser3d/effect/ueffect.h"

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
		void setGeometry(Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles
			, int vCountPerPatch = 0);
		void setAttribute(AttributeShade* attribute1 = nullptr,
			AttributeShade* attribute2 = nullptr, AttributeShade* attribute3 = nullptr,
			AttributeShade* attribute4 = nullptr, AttributeShade* attribute5 = nullptr,
			Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles);

		Qt3DRender::QGeometry* geometry();
		void replaceGeometryRenderer(Qt3DRender::QGeometryRenderer* geometryRenderer);

		Qt3DRender::QBlendEquationArguments* blendArguments();

		int traitPositionCount();
		void fillPositionData(char* buffer, int len);

		void setPassCullFace(const QString& passName, Qt3DRender::QCullFace::CullingMode cullingMode = Qt3DRender::QCullFace::NoCulling);
		void setPassBlend(const QString& passName, Qt3DRender::QBlendEquationArguments::Blending source = Qt3DRender::QBlendEquationArguments::SourceAlpha,
			Qt3DRender::QBlendEquationArguments::Blending destination = Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);

		void setPassStencilMask(const QString& passName, int mask);
		void setPassStencilOperation(const QString& passName,
			Qt3DRender::QStencilOperationArguments::Operation depthFail,
			Qt3DRender::QStencilOperationArguments::Operation stencilFail,
			Qt3DRender::QStencilOperationArguments::Operation allPass);
		void setPassStencilFunction(const QString& passName, Qt3DRender::QStencilTestArguments::StencilFunction func, int reference, int comparisonMask);

		void setPassDepthTest(const QString& passName, Qt3DRender::QDepthTest::DepthFunction depthFunc = Qt3DRender::QDepthTest::Less);
		void setPassNoDepthMask(const QString& passName);
	protected:
		Qt3DRender::QAttribute* positionAttribute();
		UEffect* _effect();
	protected:
		Qt3DRender::QMaterial* m_material;
		Qt3DCore::QTransform* m_transform;
		Qt3DRender::QGeometryRenderer* m_geometryRenderer;
	};
}
#endif // QTUSER_3D_BASICENTITY_1594569444448_H