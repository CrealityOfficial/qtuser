#ifndef _WHOLEDEMO_1594283718543_H
#define _WHOLEDEMO_1594283718543_H
#include "qtuserqml/qtuserqmlexport.h"
#include "qtusercore/module/commandcenter.h"
#include <QtCore/QObject>
#include <Qt3DCore/QEntity>

class DemoBase;
class GLQuickItem;
class QTUSER_QML_API WholeDemo : public QObject
{
	Q_OBJECT
	Q_PROPERTY(GLQuickItem* glQuickItem READ glQuickItem WRITE setGLQuickItem)
	Q_PROPERTY(QObject* demo READ demo WRITE setDemo)
public:
	WholeDemo(QObject* parent = nullptr);
	virtual ~WholeDemo();

	Q_INVOKABLE void initialize();
	Q_INVOKABLE void uninitialize();
	Q_INVOKABLE void execute(const QString& text);
	void changeState(const QStringList& argv);
public:
	GLQuickItem* glQuickItem();
	void setGLQuickItem(GLQuickItem* quickItem);
	QObject* demo();
	void setDemo(QObject* demo);
protected:
	Qt3DCore::QEntity* m_root;

	DemoBase* m_currentBase;
	CommandCenter m_commandCenter;
};

extern WholeDemo* qDemo;
#endif // _WHOLEDEMO_1594283718543_H