#ifndef _NULLSPACE_KEYWORDCACHE_1591949333397_H
#define _NULLSPACE_KEYWORDCACHE_1591949333397_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>
#include "keywordlist.h"

namespace qtuser_core
{
	class QTUSER_CORE_API KeywordCache : public QObject
	{
	public:
		KeywordCache(QObject* parent = nullptr);
		virtual ~KeywordCache();

		void parse(int argc, char** argv);

		QStringList additionalPathes();
		QList<KeywordList*>& keywords();
	private:
		void clearCache();
	protected:
		QList<KeywordList*> m_keywordLists;
		QStringList m_additionalPathes;
	};
}
#endif // _NULLSPACE_KEYWORDCACHE_1591949333397_H
