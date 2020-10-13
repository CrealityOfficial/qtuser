#ifndef _NULLSPACE_QMLAPPLICATIONINTERFACE_1589944733818_H
#define _NULLSPACE_QMLAPPLICATIONINTERFACE_1589944733818_H
#include <QtQml/QQmlApplicationEngine>

class QMLApplicationInterface
{
public:
	virtual ~QMLApplicationInterface(){}

	virtual void startEngine(QQmlApplicationEngine& engine) = 0;
};

Q_DECLARE_INTERFACE(QMLApplicationInterface, "creative.QMLApplicationInterface")
#endif // _NULLSPACE_QMLAPPLICATIONINTERFACE_1589944733818_H
