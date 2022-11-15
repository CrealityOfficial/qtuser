#include "effectmanager.h"
#include "qtuser3d/utils/techniquecreator.h"
#include "qtuser3d/renderpass/renderpassmanager.h"
#include "qtuser3d/effect/ueffect.h"
#include <Qt3DRender/QDepthTest>
namespace qtuser_3d
{
	SINGLETON_IMPL(EffectManager)
	EffectManager::EffectManager()
		:QObject()
		, m_root(nullptr)
	{
		m_root = new Qt3DCore::QNode();
	}
	
	EffectManager::~EffectManager()
	{
		delete m_root;
	}

	Qt3DCore::QNode* EffectManager::root()
	{
		return m_root;
	}

	void EffectManager::registerEffect(const QString& name, Qt3DRender::QEffect* effect)
	{
		if (effect && (m_effects.find(name) == m_effects.end()))
		{
			effect->setParent(m_root);
			m_effects.insert(name, effect);
		}
	}

	Qt3DRender::QEffect* EffectManager::create(const QString& name, Qt3DCore::QNode* parent)
	{
		UEffect* effect = new UEffect(parent);
		Qt3DRender::QTechnique* technique = TechniqueCreator::createOpenGLBase(effect);
		effect->addTechnique(technique);
		QStringList renderPasses = name.split("_");
		for (QString& str : renderPasses)
		{
			Qt3DRender::QRenderPass* pass = parent ? RENDERPASSCREATE(str, technique) : RENDERPASS(str);
			if (pass)
			{
				effect->registerRenderPass(str, pass);
				technique->addRenderPass(pass);
			}
		}
		return effect;
	}

	Qt3DRender::QEffect* EffectManager::effect(const QString& name)
	{
		QMap<QString, Qt3DRender::QEffect*>::iterator it = m_effects.find(name);
		if (it != m_effects.end()) return (*it);

		Qt3DRender::QEffect* effect = create(name, nullptr);
		if (effect)
		{
			effect->setParent(m_root);
			it = m_effects.insert(name, effect);
			if (it == m_effects.end())
			{//insert error
				delete effect;
				effect = nullptr;
			}
		}
		return effect;
	}
}