#ifndef _NULLSPACE_FILEGENERATOR_1591949333397_H
#define _NULLSPACE_FILEGENERATOR_1591949333397_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>
#include <QtCore/QSet>

namespace qtuser_core
{
	class KeywordList;
	class QTUSER_CORE_API FileGenerator : public QObject
	{
	public:
		FileGenerator(QObject* parent = nullptr);
		virtual ~FileGenerator();

		void write(KeywordList* keywordList);
		void parse(const QString& name, const QString& source);
		void parseName(const QString& name);
		void parseSource(const QString& source);
		bool valid();

		QStringList keywords();
	protected:
		void parseString(const QString& str, QSet<QString>& realKeys);
		void replace(KeywordList* keywordList, QString& str, QSet<QString>& realKeys);
	protected:
		QString m_name;
		QString m_source;

		QSet<QString> m_keywords;
		QSet<QString> m_nameKeys;
		QSet<QString> m_sourceKeys;
	};
}
#endif // _NULLSPACE_FILEGENERATOR_1591949333397_H
