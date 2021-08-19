#include "settingcache.h"
#include <QtCore/QSettings>
#include <QtCore/QDebug>

namespace qtuser_core
{
	bool checkKey(const QString& groupName, const QString& key)
	{
		if (groupName.isEmpty())
		{
			qDebug() << "Try Cache In Empty Group.";
			return false;
		}

		if (key.isEmpty())
		{
			qDebug() << "Try Cache Empty Key.";
			return false;
		}

		return true;
	}

	void cacheString(const QString& groupName, const QString& key, const QString& value)
	{
		if (!checkKey(groupName, key))
			return;

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

	void cacheStrings(const QString& key, const QStringList& values)
	{
		if (key.isEmpty())
		{
			qDebug() << "Try Cache Empty Key.";
			return;
		}

		QSettings settings;
		settings.remove(key);
		qDebug() << "QSettings -------------cacheStrings/key"<< key;
		settings.beginWriteArray(key);
		for (int i = 0; i < values.size(); ++i)
		{
			settings.setArrayIndex(i);
			settings.setValue("name", values[i]);
			qDebug() << i << "name" << values[i];
		}
		settings.endArray();
	}

	QStringList traitString(const QString& key)
	{
		QStringList values;

		QSettings settings;
		int size = settings.beginReadArray(key);
		for (int i = 0; i < size; ++i)
		{
			settings.setArrayIndex(i);
			QString machine = settings.value("name").toString();
			if (!machine.isEmpty())
				values.append(machine);
		}
		settings.endArray();

		return values;
	}

	void cacheInt(const QString& groupName, const QString& key, int value)
	{
		if (!checkKey(groupName, key))
			return;

		QSettings setting;
		setting.beginGroup(groupName);
		setting.setValue(key, value);
		setting.endGroup();
	}

	int traitInt(const QString& groupName, const QString& key, int defaultValue)
	{
		int value;

		QSettings setting;
		setting.beginGroup(groupName);
		value = setting.value(key, defaultValue).toInt();
		setting.endGroup();
		
		return value;
	}
}