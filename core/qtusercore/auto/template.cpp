#include "template.h"
#include <QtCore/QFile>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QDebug>
#include "filegenerator.h"
#include "keywordlist.h"

Template::Template(QObject* parent)
	:QObject(parent)
{
}
Template::~Template()
{
}

void Template::write(KeywordList* keywordList)
{
	if (!keywordList) return;

	for (FileGenerator* generator : m_fileGenerators)
	{
		const QStringList& keys = generator->keywords();
		QStringList mismatch;
		for (const QString& key : keys)
		{
			if (!keywordList->test(key))
				mismatch << key;
		}
		if (mismatch.size() == 0)
		{
			generator->write(keywordList);
		}
		else
		{
			qDebug() << "Mismatch Keywords " << mismatch;
		}
	}
}

bool Template::parse(const QString& templateFile)
{
	clearGenerators();

	QFile file(templateFile);
	if (file.open(QIODevice::ReadOnly))
	{
		QXmlStreamReader xmlReader(&file);

		FileGenerator* fileGenerator = nullptr;
		unsigned flag = 0;
		while (!xmlReader.atEnd())
		{
			if (xmlReader.isStartElement())
			{
				QStringRef tagname = xmlReader.name();
				if (tagname == "file")
				{
					fileGenerator = new FileGenerator(this);
				}
				if (tagname == "name")
				{
					flag = 1;
				}
				if (tagname == "source")
				{
					flag = 2;
				}
			}

			if (xmlReader.isCDATA())
			{
				QString data = xmlReader.text().toString().trimmed();
				if (flag == 1)
				{
					fileGenerator->parseName(data);
				}
				else if (flag == 2)
				{
					fileGenerator->parseSource(data);
				}
			}

			if (xmlReader.isEndElement())
			{
				QStringRef tagname = xmlReader.name();
				if (tagname == "file")
				{
					if (fileGenerator)
					{
						m_fileGenerators.push_back(fileGenerator);
						fileGenerator = nullptr;
					}
				}
				if (tagname == "name")
				{

				}
				if (tagname == "source")
				{

				}
			}

			xmlReader.readNext();
		}

		if (xmlReader.hasError()) 
		{
			clearGenerators();
		}

		parseKeys();
		return valid();
	}

	return false;
}

void Template::clearGenerators()
{
	qDeleteAll(m_fileGenerators);
	m_fileGenerators.clear();

	m_builtinKeys.clear();
	m_userKeys.clear();
}

bool Template::valid()
{
	return m_fileGenerators.size() > 0;
}

QStringList Template::builtinKeys()
{
	return m_builtinKeys;
}

QStringList Template::userKeys()
{
	return m_userKeys;
}

void Template::parseKeys()
{
	QSet<QString> unionKeys;
	for (FileGenerator* generator : m_fileGenerators)
	{
		QStringList keys = generator->keywords();
		for (QString key : keys)
		{
			QString lkey = key.toLower();
			if (!unionKeys.contains(lkey))
			{
				if (KeywordList::builtin(lkey))
				{
					m_builtinKeys << lkey;
				}
				else
				{
					m_userKeys << lkey;
				}
				unionKeys.insert(lkey);
			}
		}
	}
}
