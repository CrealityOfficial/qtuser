#include "qtuserqml/main/demobase.h"

#include "qtuser3d/framegraph/surface.h"
#include "qtuser3d/camera/cameracontroller.h"
#include "qtuser3d/camera/screencamera.h"
#include "qtusercore/string/urlutils.h"

#include "qtuserqml/main/renderinterface.h"
#include "qtuserqml/main/eventinterface.h"
#include "qtuser3d/effect/effectmanager.h"

#include <QtCore/QDebug>

#define NULLCHECK Qt3DCore::QEntity* entity = this->findWithName(name); \
				 if (entity == nullptr) { qDebug() << "can;t find entity ---> " << name; return; }
#define BASICNULLCHECK BasicEntity* entity = bEntity(name);; \
				 if (entity == nullptr) { qDebug() << "can;t find entity ---> " << name; return; }

using namespace qtuser_3d;
CXSW_REG(DemoBase)
DemoBase::DemoBase(Qt3DCore::QNode* parent)
	:RenderGraph(parent)
	, m_uiRoot(nullptr)
{
	m_surface = new qtuser_3d::Surface();
	m_surface->setClearColor(QColor(255, 255, 255));
	m_root = new Qt3DCore::QEntity();
	m_offRoot = new Qt3DCore::QEntity();

	m_camera = new qtuser_3d::ScreenCamera(this);
	m_cameraController = new CameraController(this);
	m_cameraController->setScreenCamera(m_camera);
	m_surface->setCamera(m_camera->camera());

	addFunc("delete", FUNC(DemoBase::_deleteEntity));
	addFunc("show", FUNC(DemoBase::_showEntity));
	addFunc("hide", FUNC(DemoBase::_hideEntity));
}

DemoBase::~DemoBase()
{
	delete m_camera;
	m_surface->setCamera(nullptr);
	delete m_surface;
	delete m_cameraController;
	delete m_offRoot;
}

void DemoBase::initialize(QObject* uiRoot)
{
	m_uiRoot = uiRoot;
	registerRenderGraph(this);
	renderRenderGraph(this);	

	addRightMouseEventHandler(m_cameraController);
	addMidMouseEventHandler(m_cameraController);
	addResizeEventHandler(m_cameraController);
	addWheelEventHandler(m_cameraController);
	addKeyEventHandler(this);

	connect(m_cameraController, SIGNAL(signalViewChanged(bool)), this, SLOT(viewChanged(bool)));
}

void DemoBase::uninitialize()
{
	renderRenderGraph(nullptr);

	removeRightMouseEventHandler(m_cameraController);
	removeMidMouseEventHandler(m_cameraController);
	removeResizeEventHandler(m_cameraController);
	removeWheelEventHandler(m_cameraController);
	removeKeyEventHandler(this);

	disconnect(m_cameraController, SIGNAL(signalViewChanged(bool)), this, SLOT(viewChanged(bool)));
}

Qt3DCore::QEntity* DemoBase::sceneGraph()
{
	return m_root;
}

Qt3DRender::QFrameGraphNode* DemoBase::frameGraph()
{
	return m_surface;
}

void DemoBase::begineRender()
{

}

void DemoBase::endRender()
{
}

void DemoBase::updateRenderSize(QSize size)
{
	m_surface->updateSurfaceSize(size);
}

void DemoBase::onRegistered()
{

}

void DemoBase::onUnRegistered()
{

}

QString DemoBase::translateUrl(QUrl& url)
{
	return qtuser_core::QMLUrl2FileString(url);
}

void DemoBase::home(qtuser_3d::Box3D box)
{
	m_cameraController->home(box);
}

void DemoBase::viewChanged(bool requestUpdate)
{
	signalUpdate();
}

void DemoBase::execute(const QString& text)
{
	m_commandCenter.execute(text);
}

void DemoBase::onKeyPress(QKeyEvent* event)
{

}

void DemoBase::onKeyRelease(QKeyEvent* event)
{

}

void DemoBase::addFunc(const QString& cmd, cmdFunc func)
{
	m_commandCenter.addFunc(cmd, func);
}

void DemoBase::_deleteEntity(const QStringList& args)
{
	ARGCHECK;
	deleteEntity(args.at(0));
}

void DemoBase::_showEntity(const QStringList& args)
{
	ARGCHECK;
	showEntity(args.at(0));
}

void DemoBase::_hideEntity(const QStringList& args)
{
	ARGCHECK;
	hideEntity(args.at(0));
}

Qt3DCore::QEntity* DemoBase::findWithName(const QString& name)
{
	QHash<QString, Qt3DCore::QEntity*>::iterator it = m_namedEntities.find(name);
	if (it != m_namedEntities.end())
		return it.value();
	return nullptr;
}

qtuser_3d::BasicEntity* DemoBase::bEntity(const QString& name)
{
	Qt3DCore::QEntity* e = findWithName(name);
	return dynamic_cast<qtuser_3d::BasicEntity*>(e);
}

void DemoBase::setNamedGeometry(const QString& name, Qt3DRender::QGeometry* geometry, Qt3DRender::QGeometryRenderer::PrimitiveType type)
{
	BASICNULLCHECK;
	entity->setGeometry(geometry, type);
}

void DemoBase::deleteEntity(Qt3DCore::QEntity* entity)
{
	if (!entity)
	{
		qDebug() << "entity is empty.";
		return;
	}

	NamedEntitiesIter iter = m_namedEntities.end();
	for (iter = m_namedEntities.begin(); iter != m_namedEntities.end(); ++iter)
	{
		if (iter.value() == entity)
		{
			break;
		}
	}

	if (iter != m_namedEntities.end())
	{
		iter.value()->setParent((Qt3DCore::QNode*)nullptr);
		delete iter.value();
		m_namedEntities.erase(iter);
	}
}

void DemoBase::deleteEntity(const QString& name)
{
	NULLCHECK;
	deleteEntity(entity);
}

bool DemoBase::addEntity(const QString& name, Qt3DCore::QEntity* entity)
{
	if (!entity)
	{
		qDebug() << "entity is empty.";
		return false;
	}

	if (findWithName(name))
	{
		qDebug() << "named entity " << name << " exist.";
		return false;
	}

	m_namedEntities.insert(name, entity);
	showEntity(name);
	return true;
}

qtuser_3d::BasicEntity* DemoBase::newEntity(const QString& name, const QString& shaderName)
{
	qtuser_3d::BasicEntity* e = new qtuser_3d::BasicEntity();
	if (addEntity(name, e))
	{
		e->setEffect(EFFECTCREATE("finephong", nullptr));
		return e;
	}

	delete e;
	return nullptr;
}

void DemoBase::showEntity(const QString& name)
{
	NULLCHECK;
	if (entity->parent() == m_root)
	{
		qDebug() << "named entity " << name << " already visible.";
		return;
	}

	entity->setParent(m_root);
}

void DemoBase::hideEntity(const QString& name)
{
	NULLCHECK;
	if (entity->parent() == nullptr)
	{
		qDebug() << "named entity " << name << " already invisible.";
		return;
	}

	entity->setParent((Qt3DCore::QNode*)nullptr);
}