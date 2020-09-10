#include "keywordlist.h"
#include <QtCore/QSet>
#include <QtCore/QDateTime>

QSet<QString> builtins = {"time"};
KeywordList::KeywordList(QObject* parent)
	:QObject(parent)
{
}

KeywordList::~KeywordList()
{
}

void KeywordList::set(const QStringList& builtinKeys)
{

}

void KeywordList::set(const QString& key, const QString& value)
{
	auto it = m_keywords.find(key);
	if (it != m_keywords.end())
		it.value() = value;
	else
	{
		m_keywords.insert(key, value);
	}
}

QString KeywordList::get(const QString& key)
{
	auto it = m_keywords.find(key);
	if (it != m_keywords.end())
		return it.value();
	return QString();
}

bool KeywordList::test(const QString& key)
{
	return m_keywords.find(key) != m_keywords.end();
}

QString KeywordList::use()
{
	return m_useTemplate;
}

void KeywordList::setTemplate(const QString& name)
{
	m_useTemplate = name;
}

QString KeywordList::directory()
{
	return m_directory;
}

void KeywordList::setDirectory(const QString& directory)
{
	m_directory = directory;
}

bool KeywordList::builtin(const QString& key)
{
	return builtins.contains(key);
}

void KeywordList::updateList(const QHash<QString, QString>& keyvalues)
{
	//update builtin 
	QString time = QString("%1").arg(QDateTime::currentDateTime().toMSecsSinceEpoch());
	set("time", time);
	set("Time", time);
	set("TIME", time);

	//update user
	for (QHash<QString, QString>::const_iterator it = keyvalues.begin(); it != keyvalues.end(); ++it)
	{
		QString key = it.key();
		QString value = it.value();

		set(key.toLower(), value.toLower());
		set(key.toUpper(), value.toUpper());

		key[0] = key[0].toUpper();
		set(key, value);
	}
}