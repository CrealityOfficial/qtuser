#include "keywordcache.h"
KeywordCache::KeywordCache(QObject* parent)
	:QObject(parent)
{
}
KeywordCache::~KeywordCache()
{
}

void KeywordCache::parse(int argc, char** argv)
{
	clearCache();
}

QList<KeywordList*>& KeywordCache::keywords()
{
	return m_keywordLists;
}

QStringList KeywordCache::additionalPathes()
{
	return m_additionalPathes;
}

void KeywordCache::clearCache()
{
	qDeleteAll(m_keywordLists);
	m_keywordLists.clear();
}
