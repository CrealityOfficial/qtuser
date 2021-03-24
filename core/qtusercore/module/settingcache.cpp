#include "settingcache.h"
#include <QtCore/QSettings>
#include <QtCore/QDebug>

namespace qtuser_core
{
	void cacheString(const QString& groupName, const QString& key, const QString& value)
	{
		if (groupName.isEmpty())
		{
			qDebug() << "Try Cache In Empty Group.";
			return;
		}

		if (key.isEmpty())
		{
			qDebug() << "Try Cache Empty Key.";
			return;
		}

		QSettings setting;
		setting.beginGroup(groupName);
		setting.setValue(key, value);
		setting.endGroup();
	}

	QString traitString(const QString& groupName, const QString& key, const QString& defaultStr)
	{
		QString str;

		QSettings setting;
		setting.beginGroup(groupName);
		str = setting.value(key, defaultStr).toString();
		setting.endGroup();

		return str;
	}
}