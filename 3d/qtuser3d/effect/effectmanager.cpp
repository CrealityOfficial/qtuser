#include "effectmanager.h"
#include "qtuser3d/utils/techniquecreator.h"
#include "qtuser3d/renderpass/renderpassmanager.h"
#include "qtuser3d/effect/ueffect.h"
#include "qtuser3d/refactor/xeffect.h"
#include "qtuser3d/refactor/xrenderpass.h"
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
		Qt3DRender::QEffect* effect = new UEffect(parent);
		Qt3DRender::QTechnique* technique = TechniqueCreator::createOpenGLBase(effect);
		effect->addTechnique(technique);
		QStringList renderPasses = name.split("_");
		for (QString& str : renderPasses)
		{
			Qt3DRender::QRenderPass* pass = parent ? RENDERPASSCREATE(str, technique) : RENDERPASS(str);
			if (pass) technique->addRenderPass(pass);
		}
		return effect;
	}

	qtuser_3d::XEffect* EffectManager::xeffect(const QString& name)
	{
		QMap<QString, qtuser_3d::XEffect*>::iterator it = m_xeffects.find(name);
		if (it != m_xeffects.end()) return (*it);

		qtuser_3d::XEffect* effect = xcreate(name, nullptr);
		if (effect)
		{
			effect->setParent(m_root);
			it = m_xeffects.insert(name, effect);
			if (it == m_xeffects.end())
			{//insert error
				delete effect;
				effect = nullptr;
			}
		}
		return effect;
	}

	qtuser_3d::XEffect* EffectManager::xcreate(const QString& name, Qt3DCore::QNode* parent)
	{
		qtuser_3d::XEffect* effect = new XEffect(parent);
		Qt3DRender::QTechnique* technique = effect->techniques()[0];
		QStringList renderPasses = name.split("_");
		for (QString& str : renderPasses)
		{
			qtuser_3d::XRenderPass* pass = parent ? XRENDERPASSCREATE(str, technique) : XRENDERPASS(str);
			if (pass) effect->addRenderPass(pass);
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