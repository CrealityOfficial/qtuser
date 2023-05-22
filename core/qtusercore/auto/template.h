#ifndef _NULLSPACE_TEMPLATE_1591949333381_H
#define _NULLSPACE_TEMPLATE_1591949333381_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>

namespace qtuser_core
{
	class KeywordList;
	class FileGenerator;
	class QTUSER_CORE_API Template : public QObject
	{
	public:
		Template(QObject* parent = nullptr);
		virtual ~Template();

		void write(KeywordList* keywordList);
		bool parse(const QString& templateFile);
		bool valid();

		QStringList builtinKeys();
		QStringList userKeys();
	private:
		void clearGenerators();
		void parseKeys();
	protected:
		QList<FileGenerator*> m_fileGenerators;

		QStringList m_userKeys;
		QStringList m_builtinKeys;
	};
}
#endif // _NULLSPACE_TEMPLATE_1591949333381_H
