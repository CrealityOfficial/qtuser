#include "qtuserqml/main/main.h"
#include <QtGui/QGuiApplication>
#include <QtWidgets/QApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QtGui/QOpenGLContext>
#include "qtuserqml/interface/qmlapplicationinterface.h"
#include  <QThread>
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
    }

	int qmlAppMain(int argc, char* argv[], const QString& dll)
	{
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QGuiApplication app(argc, argv);

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

    int qmlAppMainNoPointer(int argc, char* argv[], const QString& dll)
    {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QGuiApplication app(argc, argv);

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
		QGuiApplication app(argc, argv);
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
