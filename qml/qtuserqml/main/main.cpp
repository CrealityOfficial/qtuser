#include "qtuserqml/main/main.h"
#include <QtGui/QGuiApplication>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>

#include <QtQml/QQmlApplicationEngine>
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
}