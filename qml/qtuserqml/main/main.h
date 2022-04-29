#ifndef QTUSER_QML_MAIN_1601388211209_H
#define QTUSER_QML_MAIN_1601388211209_H
#include "qtuserqml/qtuserqmlexport.h"
#include <functional>

class QQmlApplicationEngine;
class QApplication;
typedef std::function<void(QQmlApplicationEngine&)> appFunc;

namespace qtuser_qml
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

	QTUSER_QML_API int hookMain(int argc, char* argv[]);
	QTUSER_QML_API int qmlMain(int argc, char* argv[], const QString& qml);
	QTUSER_QML_API int qmlAppMain(int argc, char* argv[], const QString& dll);
	QTUSER_QML_API int qmlAppMain(int argc, char* argv[], appFunc func);
	QTUSER_QML_API int qmlAppMain(int argc, char* argv[], QmlAppModule& app);

    QTUSER_QML_API int qmlAppMainNoPointer(int argc, char* argv[], const QString& dll);

	QTUSER_QML_API int qmlSimulation(int argc, char* argv[]);
}

#endif // QTUSER_QML_MAIN_1601388211209_H
