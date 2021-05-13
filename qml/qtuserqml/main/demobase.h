#ifndef _DEMOBASE_1594285008655_H
#define _DEMOBASE_1594285008655_H
#include "qtuserqml/qtuserqmlexport.h"
#include "qtuserqml/macro.h"
#include "qtuser3d/framegraph/rendergraph.h"
#include "qtuser3d/event/eventhandlers.h"
#include "qtusercore/module/commandcenter.h"
#include "qtusercore/module/metacreator.h"
#include "qtuser3d/math/box3d.h"
#include "qtuser3d/entity/basicentity.h"
#include <QtCore/QUrl>
#include <QtCore/QHash>

#include "qtuser3d/effect/effectmanager.h"
#include "qtuser3d/entity/triangleentity.h"
#include "qtuser3d/entity/lineentity.h"
#include "qtuser3d/entity/pointcloudentity.h"

namespace qtuser_3d
{
	class Surface;
	class ScreenCamera;
}

#define ARGCHECK if(args.size() == 0){ qDebug() << "args size is zero."; return; } 

class CameraController;

typedef QHash<QString, Qt3DCore::QEntity*> NamedEntities;
typedef NamedEntities::Iterator NamedEntitiesIter;

#define CXSW_SCOPE "com.cxsw.demo"
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define COMMON_REASON "Created by Cpp"

#define CXSW_REG(x) QML_AUTO_TYPE(x, CXSW_SCOPE, VERSION_MAJOR, VERSION_MINOR)
#define CXSW_REG2(x) QML_INTERFACE(x, CXSW_SCOPE, VERSION_MAJOR, VERSION_MINOR)

class QTUSER_QML_API DemoBase : public qtuser_3d::RenderGraph
	,public qtuser_3d::KeyEventHandler
{
	Q_OBJECT
public:
	DemoBase(Qt3DCore::QNode* parent = nullptr);
	virtual ~DemoBase();

	Q_INVOKABLE void initialize(QObject* uiRoot = nullptr);
	Q_INVOKABLE void uninitialize();

	Qt3DCore::QEntity* sceneGraph();
	Qt3DRender::QFrameGraphNode* frameGraph();

	void begineRender() override;
	void endRender() override;
	void updateRenderSize(QSize size) override;
	virtual void execute(const QString& text);

	void onRegistered() override;
	void onUnRegistered() override;

	QString translateUrl(QUrl& url);

	void home(qtuser_3d::Box3D box);
protected:
	void onKeyPress(QKeyEvent* event) override;
	void onKeyRelease(QKeyEvent* event) override;
	void addFunc(const QString& cmd, cmdFunc func);

	void _deleteEntity(const QStringList& list);
	void _showEntity(const QStringList& list);
	void _hideEntity(const QStringList& list);
protected:
	Qt3DCore::QEntity* findWithName(const QString& name);
	qtuser_3d::BasicEntity* bEntity(const QString& name);
	void setNamedGeometry(const QString& name, Qt3DRender::QGeometry* geometry, 
		Qt3DRender::QGeometryRenderer::PrimitiveType type = Qt3DRender::QGeometryRenderer::Triangles);

	void deleteEntity(Qt3DCore::QEntity* entity);
	void deleteEntity(const QString& name);
	bool addEntity(const QString& name, Qt3DCore::QEntity* entity);
	qtuser_3d::BasicEntity* newEntity(const QString& name, const QString& shaderName);

	void showEntity(const QString& name);
	void hideEntity(const QString& name);

	qtuser_3d::TriangleEntity* addTriangleEntity(const QString& name);
	qtuser_3d::LineEntity* addLineEntity(const QString& name);
	qtuser_3d::PointCloudEntity* createPoint(const QString& name, const QVector4D& color);

	void setBackColor(const QColor& color);
public slots:
	void viewChanged(bool requestUpdate);
protected:
	qtuser_3d::Surface* m_surface;

	Qt3DCore::QEntity* m_root;
	Qt3DCore::QEntity* m_offRoot;

	qtuser_3d::ScreenCamera* m_camera;
	CameraController* m_cameraController;

	CommandCenter m_commandCenter;

	NamedEntities m_namedEntities;

protected:
	QObject* m_uiRoot;
};

#endif // _DEMOBASE_1594285008655_H