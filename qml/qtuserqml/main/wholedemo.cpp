#include "qtuserqml/main/wholedemo.h"
#include "qtuserqml/main/demoqmlregister.h"
#include "qtuserqml/main/rendercenter.h"
#include "qtuserqml/gl/glquickitem.h"
#include "qtuser3d/utils/shaderprogrammanager.h"
#include "qtuser3d/utils/primitiveshapecache.h"
#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/renderpass/renderpassmanager.h"

#include "qtuserqml/main/renderinterface.h"
#include "qtuserqml/main/demobase.h"
#include "qtusercore/module/metacreator.h"

WholeDemo* qDemo = nullptr;

CXSW_REG(GLQuickItem)
CXSW_REG(WholeDemo)
WholeDemo::WholeDemo(QObject* parent)
	:QObject(parent)
	, m_currentBase(nullptr)
{
	qDemo = this;
	m_root = new Qt3DCore::QEntity();

	m_commandCenter.addFunc("state", FUNC(WholeDemo::changeState));
}

WholeDemo::~WholeDemo()
{
	delete m_root;
}

GLQuickItem* WholeDemo::glQuickItem()
{
	return nullptr;
}

void WholeDemo::setGLQuickItem(GLQuickItem* quickItem)
{
	getRenderCenter()->setGLQuickItem(quickItem);
}

QObject* WholeDemo::demo()
{
	return m_currentBase;
}

void WholeDemo::setDemo(QObject* demo)
{
	m_currentBase = qobject_cast<DemoBase*>(demo);
}

void WholeDemo::execute(const QString& text)
{
	if (!m_commandCenter.execute(text))
	{
		if (m_currentBase)
			m_currentBase->execute(text);
	}
}

void WholeDemo::initialize()
{
	PRIMITIVEROOT->setParent(m_root);
	SHADERROOT->setParent(m_root);
	RENDERPASSROOT->setParent(m_root);
	EFFECTROOT->setParent(m_root);

	EFFECT("color");
	EFFECT("pure");

	registerResidentNode(m_root);

	setContinousRender();
}

void WholeDemo::uninitialize()
{
	unRegisterResidentNode(m_root);

	EFFECTROOT->setParent((Qt3DCore::QNode*)nullptr);
	RENDERPASSROOT->setParent((Qt3DCore::QNode*)nullptr);
	PRIMITIVEROOT->setParent((Qt3DCore::QNode*)nullptr);
	SHADERROOT->setParent((Qt3DCore::QNode*)nullptr);
}

void WholeDemo::changeState(const QStringList& argv)
{
	if (argv.size() > 0)
	{
		DemoBase* base = qobject_cast<DemoBase*>(qtuser_core::getMetaCreator()->create(argv[0]));

		if (base)
		{
			if (m_currentBase)
				m_currentBase->uninitialize();

			m_currentBase = base;

			if (m_currentBase)
				m_currentBase->initialize();
		}
	}
}