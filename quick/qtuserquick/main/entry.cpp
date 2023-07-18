#include "qtuserquick/main/entry.h"
#include <QtWidgets/QApplication>

#include <QtCore/QDebug>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickWindow> 
#include <QFont>
#include "qtusercore/module/systemutil.h"
#include "qtuser3d/module/glcompatibility.h"
#include <QIcon>
namespace qtuser_quick
{
    void setDefaultQmlBeforeApp()
    {

    }

	void setDefaultQmlAfterApp(QQmlApplicationEngine& engine)
	{
		//qml plugin
		QStringList qmlPathList = engine.importPathList();

		QString applicationDir = QCoreApplication::applicationDirPath();
		qDebug() << "applicationDir " << applicationDir;

#ifdef Q_OS_OSX
		qDebug() << "OS OSX setQmlLoadPath";
		qmlPathList << applicationDir + "/../Resources/qml";
#elif defined Q_OS_WIN32
		qDebug() << "OS WIN32 setQmlLoadPath";
#elif defined Q_OS_LINUX
		qDebug() << "OS LINUX setQmlLoadPath";
		qmlPathList << applicationDir + "/lib/";
		qmlPathList << applicationDir + "/qml/";
#endif

		qDebug() << "Qml import paths:";
		qDebug() << qmlPathList;

		engine.setImportPathList(qmlPathList);
	}

	int qmlAppMain(int argc, char* argv[], QmlAppModule& appModule)
	{
		qtuser_core::initializeLog(argc, argv);
		appModule.beforeAppConstruct();

		int ret = 0;
		{
			qtuser_core::setDefaultBeforApp();
			qtuser_3d::setBeforeApplication();
			setDefaultQmlBeforeApp();

			QApplication app(argc, argv);
            QFont default_font = QFont();
            default_font.setPointSize(9);
            app.setFont(default_font);
			QQmlApplicationEngine* engine = new QQmlApplicationEngine();

			qtuser_core::setDefaultAfterApp();
			setDefaultQmlAfterApp(*engine);

			appModule.afterAppConstruct();

			appModule.startLoadQmlEngine(app, *engine);
			ret = app.exec();

			appModule.onAppEngineShutDown();

#ifndef __APPLE__
			delete engine;
#endif
		}

		qtuser_core::uninitializeLog();
		return ret;
	}

	int appMain(int argc, char* argv[], AppModuleCreateFunc func)
	{
		qtuser_core::initializeLog(argc, argv);

		qtuser_core::setDefaultBeforApp();
		qtuser_3d::setBeforeApplication();
		setDefaultQmlBeforeApp();

		// Will make Qt3D and QOpenGLWidget share a common context
		QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

		QApplication app(argc, argv);
		app.setWindowIcon(QIcon(":/scence3d/res/logo.png"));

#ifdef Q_OS_OSX
		QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
#endif

		AppModule* appModule = func ? func() : nullptr;
		int ret = 0;
		if (appModule)
		{
			{
				QFont default_font = QFont();
				default_font.setPointSize(9);
				app.setFont(default_font);
				QQmlApplicationEngine engine;

				qtuser_core::setDefaultAfterApp();
				setDefaultQmlAfterApp(engine);

				initSystemUtil();
				showDetailSystemInfo();

				if (appModule->loadQmlEngine(app, engine))
					ret = app.exec();

				appModule->unloadQmlEngine();
			}
			appModule->shutDown();
			delete appModule;
		}

		qtuser_core::uninitializeLog();
		return ret;
	}
}
