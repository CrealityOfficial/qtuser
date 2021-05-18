#include "qtuserqml/main/main.h"
#include <QtGui/QGuiApplication>
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
        QSurfaceFormat format;
#if __APPLE__
        format.setVersion(3, 2);
        format.setProfile(QSurfaceFormat::CoreProfile);
#endif
        format.setDepthBufferSize(24);
        format.setStencilBufferSize(8);
        format.setSamples(4);
        QSurfaceFormat::setDefaultFormat(format);
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
			QGuiApplication app(argc, argv);
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
}
