#include "detaildescription.h"

namespace qtuser_core
{
	DetailDescription::DetailDescription(QObject* parent)
		:QObject(parent)
	{
	}

	DetailDescription::~DetailDescription()
	{
	}

	QString DetailDescription::get(const QString& key)
	{
		QHash<QString, QString>::iterator it = m_details.find(key);
		if (it != m_details.end())
			return it.value();
		return QString();
	}

	void DetailDescription::set(const QString& key, const QString& text)
	{
		QHash<QString, QString>::iterator it = m_details.find(key);
		if (it != m_details.end())
			it.value() = text;
		
		m_details.insert(key, text);
	}

	QStringList DetailDescription::enumKeys()
	{
		QStringList keys;
		for (QHash<QString, QString>::iterator it = m_details.begin(); it != m_details.end(); ++it)
			keys << it.key();
		return keys;
	}

	void DetailDescription::clear()
	{
		m_details.clear();
	}
}
