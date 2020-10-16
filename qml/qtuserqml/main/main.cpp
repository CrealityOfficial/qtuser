#include "qtuserqml/main/main.h"
#include <QtGui/QGuiApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include "qtuserqml/interface/qmlapplicationinterface.h"

namespace qtuser_qml
{
	int qmlAppMain(int argc, char* argv[], const QString& dll)
	{
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;

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

		return app.exec();
	}
}