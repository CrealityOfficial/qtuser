#ifndef _NULLSPACE_TOOLCOMMAND_1589423984319_H
#define _NULLSPACE_TOOLCOMMAND_1589423984319_H
#include "qtuserqml/qtuserqmlexport.h"
#include <QtCore/QObject>

class QTUSER_QML_API ToolCommand: public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
public:
	ToolCommand(QObject* parent = nullptr);
	virtual ~ToolCommand();

	bool enabled();
	Q_INVOKABLE void execute();
protected:
	virtual void onExecute();
	virtual bool enableImpl();
signals:
	void enabledChanged();
};
#endif // _NULLSPACE_TOOLCOMMAND_1589423984319_H
