#ifndef QTUSER_3D_RENDERPASSMANAGER_1594867313654_H
#define QTUSER_3D_RENDERPASSMANAGER_1594867313654_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtusercore/module/singleton.h"
#include <Qt3DRender/QRenderPass>

namespace qtuser_3d
{
	class XRenderPass;

	class QTUSER_3D_API RenderPassManager : public QObject
	{
		Q_OBJECT
		SINGLETON_DECLARE(RenderPassManager)
	public:
		virtual ~RenderPassManager();

		Qt3DCore::QNode* root();

		Qt3DRender::QRenderPass* renderPass(const QString& name);
		static Qt3DRender::QRenderPass* create(const QString& name, Qt3DCore::QNode* parent = nullptr);

		qtuser_3d::XRenderPass* xrenderPass(const QString& name);
		static qtuser_3d::XRenderPass* xcreate(const QString& name, Qt3DCore::QNode* parent = nullptr);
	protected:
		Qt3DCore::QNode* m_root;
		QMap<QString, Qt3DRender::QRenderPass*> m_renderPasses;
		QMap<QString, qtuser_3d::XRenderPass*> m_xrenderPasses;
	};
	
	SINGLETON_EXPORT(QTUSER_3D_API, RenderPassManager)
}

#define RENDERPASSROOT qtuser_3d::getRenderPassManager()->root()

#define RENDERPASS(x) qtuser_3d::getRenderPassManager()->renderPass(x)

#define RENDERPASSCREATE(x, y) qtuser_3d::getRenderPassManager()->create(x, y)

#define XRENDERPASS(x) qtuser_3d::getRenderPassManager()->xrenderPass(x)
#define XRENDERPASSCREATE(x,y) qtuser_3d::getRenderPassManager()->xcreate(x,y)

#endif // QTUSER_3D_RENDERPASSMANAGER_1594867313654_H