#include "generator.h"
#include "keywordcache.h"
#include "templatelibrary.h"

Generator::Generator(QObject* parent)
	:QObject(parent)
{
}
Generator::~Generator()
{
}

void Generator::generate(KeywordCache* cache)
{
	if (!cache) return;

	TemplateLibrary library;
	library.addSearchPath(cache->additionalPathes());

	for (KeywordList* keyword : cache->keywords())
	{
		QString templateName = keyword->use();
		Template* t = library.get(templateName);
		if (t)
		{
			t->write(keyword);
		}
	}
}
