#ifndef QTUSER_3D_XEFFECT_1679973441776_H
#define QTUSER_3D_XEFFECT_1679973441776_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/refactor/xrenderpass.h"
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QRenderState>

namespace qtuser_3d
{
	class QTUSER_3D_API XEffect : public Qt3DRender::QEffect
	{
		Q_OBJECT
	public:
		XEffect(Qt3DCore::QNode* parent = nullptr);
		virtual ~XEffect();

		Qt3DRender::QParameter* setParameter(const QString& name, const QVariant& value);
		int renderPassCount();
		void addRenderState(int index, Qt3DRender::QRenderState* state);
	protected:
		void addRenderPass(XRenderPass* pass);
	protected:
		Qt3DRender::QTechnique* m_technique;
	};
}

#endif // QTUSER_3D_XEFFECT_1679973441776_H