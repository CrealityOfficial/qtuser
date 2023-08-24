#ifndef QTUSER_3D_OVERLAYRENDERPASS_1679990543678_H
#define QTUSER_3D_OVERLAYRENDERPASS_1679990543678_H
#include <QtGui/QVector4D>
#include "qtuser3d/refactor/xrenderpass.h"

namespace qtuser_3d
{
	class QTUSER_3D_API OverlayRenderPass : public qtuser_3d::XRenderPass
	{
	public:
		OverlayRenderPass(Qt3DCore::QNode* parent = nullptr);
		virtual ~OverlayRenderPass();

		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_color;
	};
}

#endif // QTUSER_3D_OVERLAYRENDERPASS_1679990543678_H