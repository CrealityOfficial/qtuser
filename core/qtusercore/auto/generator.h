#ifndef _NULLSPACE_GENERATOR_1591949333397_H
#define _NULLSPACE_GENERATOR_1591949333397_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>

namespace qtuser_core
{
	class KeywordCache;
	class QTUSER_CORE_API Generator : public QObject
	{
	public:
		Generator(QObject* parent = nullptr);
		virtual ~Generator();

		void generate(KeywordCache* cache);
	};
}
#endif // _NULLSPACE_GENERATOR_1591949333397_H
