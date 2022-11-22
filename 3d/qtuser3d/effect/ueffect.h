#ifndef QTUSER_3D_UEFFECT_1595133304260_H
#define QTUSER_3D_UEFFECT_1595133304260_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QRenderPass>
#include <Qt3DRender/QCullFace>
#include <Qt3DRender/QBlendEquationArguments>
#include <QtCore/QMap>

namespace qtuser_3d
{
	class QTUSER_3D_API UEffect : public Qt3DRender::QEffect
	{
		Q_OBJECT
	public:
		UEffect(Qt3DCore::QNode* parent = nullptr);
		virtual ~UEffect();

		Qt3DRender::QParameter* createParameter(const QString& name, const QVariant& value);
		void destroyParameter(Qt3DRender::QParameter* parameter);

		void addRenderPass(QString& name);
		void removeRenderPass(QString& name);
		
		void registerRenderPass(const QString& name, Qt3DRender::QRenderPass* pass);
		void unregisterRenderPass(const QString& name);
		Qt3DRender::QRenderPass* traitRenderPass(const QString& name);

		void setPassCullFace(const QString& passName, Qt3DRender::QCullFace::CullingMode cullingMode = Qt3DRender::QCullFace::NoCulling);
		void setPassBlend(const QString& passName, Qt3DRender::QBlendEquationArguments::Blending source = Qt3DRender::QBlendEquationArguments::SourceAlpha,
			Qt3DRender::QBlendEquationArguments::Blending destination = Qt3DRender::QBlendEquationArguments::OneMinusSourceAlpha);
	protected:
		QMap<QString, Qt3DRender::QRenderPass*> m_attachedPasses;
	};
}
#endif // QTUSER_3D_UEFFECT_1595133304260_H