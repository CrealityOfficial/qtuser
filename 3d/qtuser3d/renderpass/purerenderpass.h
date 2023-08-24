#ifndef QTUSER_3D_PURERENDERPASS_1679990543678_H
#define QTUSER_3D_PURERENDERPASS_1679990543678_H
#include <QtGui/QVector4D>
#include "qtuser3d/refactor/xrenderpass.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PureRenderPass : public qtuser_3d::XRenderPass
	{
	public:
		PureRenderPass(Qt3DCore::QNode* parent = nullptr);
		virtual ~PureRenderPass();

		void setColor(const QVector4D& color);
	protected:
		Qt3DRender::QParameter* m_color;
	};
}

#endif // QTUSER_3D_PURERENDERPASS_1679990543678_H