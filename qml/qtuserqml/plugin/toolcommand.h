#ifndef _NULLSPACE_TOOLCOMMAND_1589423984319_H
#define _NULLSPACE_TOOLCOMMAND_1589423984319_H
#include "qtuserqml/qtuserqmlexport.h"
#include <QtCore/QObject>

class QTUSER_QML_API ToolCommand: public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)
    Q_PROPERTY(QString source READ source)
public:
	ToolCommand(QObject* parent = nullptr);
	virtual ~ToolCommand();
    int orderindex;
	bool enabled();
	QString enabledIcon() const;
	QString disableIcon() const;
	QString pressedIcon() const;
	QString source() const;
	QString name() const;
	void setEnabledIcon(const QString& icon);
	void setPressedIcon(const QString& icon);
	void setDisabledIcon(const QString& icon);
	void setName(const QString& name);
    void setSource(const QString& source);
    Q_INVOKABLE virtual bool checkSelect();
	Q_INVOKABLE virtual bool outofPlatform();
	void setEnabled(bool enabled);

	Q_INVOKABLE void execute();
	virtual void onExecute();

    Q_INVOKABLE virtual bool isSelectModel();
signals:
	void enabledChanged();
	void executed();
private:
	bool m_enabled;

protected:
	QString m_enabledIcon;
	QString m_disableIcon;
	QString m_pressedIcon;
	QString m_source;
	QString m_name;
};
#endif // _NULLSPACE_TOOLCOMMAND_1589423984319_H
