#include "filegenerator.h"
#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include "keywordlist.h"
#include <QtCore/QFile>
#include <QtCore/QTextStream>

namespace qtuser_core
{
	FileGenerator::FileGenerator(QObject* parent)
		:QObject(parent)
	{
	}

	FileGenerator::~FileGenerator()
	{
	}

	void FileGenerator::write(KeywordList* keywordList)
	{
		QString name = m_name;
		QString source = m_source;

		replace(keywordList, name, m_nameKeys);
		replace(keywordList, source, m_sourceKeys);

		QString dir = keywordList->directory();
		QString file = name;
		if (dir.length() > 0) file = dir + "/" + file;

		QFile F(file);
		if (F.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream stream(&F);
			stream << source;
		}
		F.close();
	}

	void FileGenerator::parse(const QString& name, const QString& source)
	{
		m_name = name;
		m_source = source;
	}

	void FileGenerator::parseName(const QString& name)
	{
		m_name = name;
		parseString(m_name, m_nameKeys);
	}

	void FileGenerator::parseSource(const QString& source)
	{
		m_source = source;
		parseString(m_source, m_sourceKeys);
	}

	void FileGenerator::parseString(const QString& str, QSet<QString>& realKeys)
	{
		QRegExp reg("@[a-zA-Z]*@");
		int pos = 0;
		while ((pos = reg.indexIn(str, pos)) != -1)
		{
			pos += reg.matchedLength();

			QString key = reg.cap(0);
			key = key.mid(1, key.size() - 2);
			realKeys.insert(key);
			m_keywords.insert(key.toLower());
		}
	}

	void FileGenerator::replace(KeywordList* keywordList, QString& str, QSet<QString>& realKeys)
	{
		for (QString key : realKeys)
		{
			QRegExp reg("@" + key + "@");
			QString value = keywordList->get(key);

			str.replace(reg, value);
		}
	}

	bool FileGenerator::valid()
	{
		return m_name.size() > 0 && m_source.size() > 0;
	}

	QStringList FileGenerator::keywords()
	{
		return m_keywords.toList();
	}
}
