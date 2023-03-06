#include "qtuserqml/main/main.h"
//#include <QtGui/QGuiApplication>
#include <QApplication>
#include <QtWidgets/QApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickWindow> 
#include <QtGui/QOpenGLContext>
#include "qtusercore/module/systemutil.h"
#include "qtuserqml/interface/qmlapplicationinterface.h"
#include <QThread>
#include <QDir>
namespace qtuser_qml
{
    void specifyOpenGL()
    {
#if __APPLE__
        QSurfaceFormat format;
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setSamples(4);
        QSurfaceFormat::setDefaultFormat(format);
#endif

#ifdef Q_OS_WIN32
		QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#endif
    }

	int qmlAppMain(int argc, char* argv[], const QString& dll)
	{
#ifndef __APPLE__
        //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
        QApplication app(argc, argv);

        QQmlApplicationEngine *engine=new QQmlApplicationEngine();
        
        specifyOpenGL();
        QMLApplicationInterface* appInterface = nullptr;
        QPluginLoader loader(dll);
        if (loader.load())
            appInterface = qobject_cast<QMLApplicationInterface*>(loader.instance());

        int e = -1;
        if (appInterface)
        {
            appInterface->startEngine(*engine);

            e = app.exec();
            delete appInterface;
        }
        else
        {
            qDebug() << dll << " is invalid QMLApplicationInterface " << loader.errorString();
        }

        return e;
	}

	void preSetDynamicLoadPath()
	{
		//dynamic plugin
	    QStringList dynamicPathList = QCoreApplication::libraryPaths();

#ifdef Q_OS_OSX
		qDebug() << "OS OSX pre setDynamicLoadPath";
#elif defined Q_OS_WIN32
		qDebug() << "OS WIN32 pre setDynamicLoadPath";
#elif defined Q_OS_LINUX
		qDebug() << "OS LINUX pre setDynamicLoadPath";

        if (qEnvironmentVariableIsSet("APPDIR"))
        {
            QString appdir = qEnvironmentVariable("APPDIR");
            qDebug() << "Linux get the APPDIR : " << appdir;
            dynamicPathList << appdir + "/plugins";
        }
#endif

		qDebug() << "Pre Dynamic import paths:";
		qDebug() << dynamicPathList;
		QCoreApplication::setLibraryPaths(dynamicPathList);
	}
	
	void setDynamicLoadPath(QQmlApplicationEngine& engine)
	{
		//dynamic plugin
	    QStringList dynamicPathList = QCoreApplication::libraryPaths();
		//qml plugin
		QStringList qmlPathList = engine.importPathList();

		QString applicationDir = QCoreApplication::applicationDirPath();
		qDebug() << "applicationDir " << applicationDir;

		dynamicPathList << applicationDir;
#ifdef Q_OS_OSX
		qDebug() << "OS OSX setDynamicLoadPath";
        dynamicPathList << QCoreApplication::applicationDirPath() + "/../Frameworks";
    	qmlPathList << QCoreApplication::applicationDirPath() + "/../Resources/qml";
#elif defined Q_OS_WIN32
		qDebug() << "OS WIN32 setDynamicLoadPath";
#elif defined Q_OS_LINUX
		qDebug() << "OS LINUX setDynamicLoadPath";
		dynamicPathList << applicationDir + "/lib/";
		qmlPathList << applicationDir + "/lib/";
		qmlPathList << applicationDir + "/qml/";
#endif

    	qDebug() << "Qml import paths:";
        qDebug() << qmlPathList;
		qDebug() << "Dynamic import paths:";
		qDebug() << dynamicPathList;

		engine.setImportPathList(qmlPathList);
		QCoreApplication::setLibraryPaths(dynamicPathList);
	}

    int qmlAppMain(int argc, char* argv[], appFunc func)
    {
        qtuser_core::initializeLog(argc, argv);
        int ret = 0;
        {
#ifndef __APPLE__
            //QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
            preSetDynamicLoadPath();

            specifyOpenGL();
            QApplication app(argc, argv);
            QQmlApplicationEngine* engine = new QQmlApplicationEngine();

            setDynamicLoadPath(*engine);

#ifdef Q_OS_OSX
            QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
#endif

            initSystemUtil();
            showDetailSystemInfo();

            func(*engine);
            ret = app.exec();

#ifndef __APPLE__
            delete engine;
#endif
        }
        qtuser_core::uninitializeLog();
        return ret;
    }

    int qmlAppMainNoPointer(int argc, char* argv[], const QString& dll)
    {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication app(argc, argv);

        QQmlApplicationEngine engine;
        specifyOpenGL();

        QMLApplicationInterface* appInterface = nullptr;
        QPluginLoader loader(dll);
        if (loader.load())
            appInterface = qobject_cast<QMLApplicationInterface*>(loader.instance());

        int e = -1;
        if (appInterface)
        {
            appInterface->startEngine(engine);

            e = app.exec();
            delete appInterface;
        }
        else
        {
            qDebug() << dll << " is invalid QMLApplicationInterface " << loader.errorString();
        }

        return e;
    }

	int hookMain(int argc, char* argv[])
	{
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QApplication app(argc, argv);
		app.setOrganizationName("DEMO");
		app.setOrganizationDomain("DEMO");
		app.setApplicationName("DEMO");

		QQmlApplicationEngine engine;

		QString entryQml = "demo.qml";
		QString prefix = "qrc:/qtuserqml/res/";
		QString qmlFile = prefix + entryQml;
		engine.load(QUrl(qmlFile));

		return app.exec();
	}

	int qmlMain(int argc, char* argv[], const QString& qml)
	{
		int result = 0;
		{
			QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
			QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
			QApplication app(argc, argv);
			app.setOrganizationName("DEMO");
			app.setOrganizationDomain("DEMO");
			app.setApplicationName("DEMO");

			QQmlApplicationEngine engine;

			engine.rootContext()->setContextProperty("invoke", qml);
			QString entryQml = "demo1.qml";
			QString prefix = "qrc:/qtuserqml/res/";
			QString qmlFile = prefix + entryQml;
			engine.load(QUrl(qmlFile));

			result = app.exec();
		}

		return result;
	}

    int qmlSimulation(int argc, char* argv[])
    {
        int result = 0;
        {
            QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
			QApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
            QApplication app(argc, argv);
            app.setOrganizationName("Simulation");
            app.setOrganizationDomain("Simulation");
            app.setApplicationName("Simulation");

            QQmlApplicationEngine engine;
            QString entryQml = "qrc:/qtuserqml/res/simulation.qml";
            engine.load(QUrl(entryQml));

            result = app.exec();
        }

        return result;
    }
}
