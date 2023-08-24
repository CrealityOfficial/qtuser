#ifndef QTUSER_3D_PURERENDERPASS_1679990543678_H
#define QTUSER_3D_PURERENDERPASS_1679990543678_H
#include <QtGui/QVector4D>
#include "qtuser3d/refactor/xrenderpass.h"

namespace qtuser_3d
{
	class QTUSER_3D_API PickfaceRenderPass : public qtuser_3d::XRenderPass
	{
	public:
		PickfaceRenderPass(Qt3DCore::QNode* parent = nullptr);
		virtual ~PickfaceRenderPass();


	protected:

	};
}

#endif // QTUSER_3D_PURERENDERPASS_1679990543678_H