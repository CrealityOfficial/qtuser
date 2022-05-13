#ifndef QTUSER_3D_EFFECTMANAGER_1594867313653_H
#define QTUSER_3D_EFFECTMANAGER_1594867313653_H
#include "qtuser3d/qtuser3dexport.h"
#include "qtuser3d/effect/ueffect.h"
#include "qtusercore/module/singleton.h"
#include <Qt3DRender/QEffect>

namespace qtuser_3d
{
	class QTUSER_3D_API EffectManager : public QObject
	{
		Q_OBJECT
		SINGLETON_DECLARE(EffectManager)
	public:
		virtual ~EffectManager();

		Qt3DCore::QNode* root();

		void registerEffect(const QString& name, Qt3DRender::QEffect* effect);
		Qt3DRender::QEffect* effect(const QString& name);
		static Qt3DRender::QEffect* create(const QString& name, Qt3DCore::QNode* parent = nullptr);
	protected:
		Qt3DCore::QNode* m_root;
		QMap<QString, Qt3DRender::QEffect*> m_effects;
	};
	
	SINGLETON_EXPORT(QTUSER_3D_API, EffectManager)
}

#define EFFECTROOT qtuser_3d::getEffectManager()->root()

#define EFFECT(x)  qtuser_3d::getEffectManager()->effect(x)

#define EFFECTCREATE(x, y) qtuser_3d::getEffectManager()->create(x, y)
#define EFFECTCREATESTATUS(x, y, s) qtuser_3d::getEffectManager()->create(x, y, s)

#endif // QTUSER_3D_EFFECTMANAGER_1594867313653_H