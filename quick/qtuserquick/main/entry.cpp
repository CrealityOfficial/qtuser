#include "qtuserquick/main/entry.h"
#include <QtWidgets/QApplication>

#include <QtCore/QDebug>
#include <QtQml/QQmlApplicationEngine>
#include <QtQml/QQmlContext>
#include <QFont>
#include "qtusercore/module/systemutil.h"

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
			setDefaultQmlBeforeApp();

			QApplication app(argc, argv);
            QFont default_font = QFont();
            default_font.setPointSize(9);
            app.setFont(default_font);
			QQmlApplicationEngine engine;

			qtuser_core::setDefaultAfterApp();
			setDefaultQmlAfterApp(engine);

			appModule.afterAppConstruct();

			appModule.startLoadQmlEngine(app, engine);
			ret = app.exec();

			appModule.onAppEngineShutDown();
		}

		qtuser_core::uninitializeLog();
		return ret;
	}

	int wQmlAppMain(int argc, wchar_t* argv[], QmlAppModule& app)
	{
		std::vector<char*> wargv(argc);
		return qmlAppMain(argc, &wargv[0], app);
	}
}
