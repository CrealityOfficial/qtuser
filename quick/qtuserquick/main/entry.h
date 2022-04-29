#ifndef QTUSER_QUICK_MAIN_1601388211209_H
#define QTUSER_QUICK_MAIN_1601388211209_H
#include "qtuserquick/interface.h"
#include <functional>

class QApplication;
class QQmlApplicationEngine;
typedef std::function<void(QQmlApplicationEngine&)> appFunc;

namespace qtuser_quick
{
	QTUSER_QUICK_API int qmlAppMain(int argc, char* argv[], appFunc func);

	class QTUSER_QUICK_API QmlEntry
	{
	public:
		virtual ~QmlEntry() {}
		virtual void beforeApp() {}
		virtual void afterApp(QApplication& app, QQmlApplicationEngine& engine) {}
		virtual void exitApp() {}   //call before application ctr~
	};
	QTUSER_QUICK_API int qmlAppMain(int argc, char* argv[], QmlEntry& entry);
}

#endif // QTUSER_QUICK_MAIN_1601388211209_H
