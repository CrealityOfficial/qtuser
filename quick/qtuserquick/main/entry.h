#ifndef QTUSER_QUICK_MAIN_1601388211209_H
#define QTUSER_QUICK_MAIN_1601388211209_H
#include "qtuserquick/interface.h"
#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>

#include <functional>

class QApplication;
namespace qtuser_quick
{
	class QmlAppModule
	{
	public:
		~QmlAppModule() {}
		virtual void beforeAppConstruct() = 0;
		virtual void afterAppConstruct() = 0;

		virtual void startLoadQmlEngine(QApplication& app, QQmlApplicationEngine& engine) = 0;
		virtual void onAppEngineShutDown() = 0;
	};

	QTUSER_QUICK_API int qmlAppMain(int argc, char* argv[], QmlAppModule& app);


	class AppModule
	{
	public:
		~AppModule() {}

		virtual bool loadQmlEngine(QApplication& app, QQmlApplicationEngine& engine) = 0;
		virtual void unloadQmlEngine() = 0;
		virtual void shutDown() = 0;
	};
	QTUSER_QUICK_API int appMain(int argc, char* argv[], AppModule& appModule);
}

#endif // QTUSER_QUICK_MAIN_1601388211209_H
