#ifndef QTUSER_3D_UEFFECT_1595133304260_H
#define QTUSER_3D_UEFFECT_1595133304260_H
#include "qtuser3d/qtuser3dexport.h"
#include <Qt3DRender/QEffect>
#include <Qt3DRender/QParameter>
#include <Qt3DRender/QRenderPass>

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

		QVector<Qt3DRender::QRenderPass*> renderPasses();
		void addRenderState(Qt3DRender::QRenderState* state);
	protected:
		QMap<QString, Qt3DRender::QRenderPass*> m_attachedPasses;
	};
}
#endif // QTUSER_3D_UEFFECT_1595133304260_H