#ifndef _QTUSER_CORE_RESOURCESFINDER_1590131561766_H
#define _QTUSER_CORE_RESOURCESFINDER_1590131561766_H
#include "qtusercore/qtusercoreexport.h"

namespace qtuser_core
{
	class QTUSER_CORE_API ResourcesFinder: public QObject
	{
		ResourcesFinder(QObject* parent = nullptr);
	public:
		virtual ~ResourcesFinder();

		static ResourcesFinder& instance();

		void addSearchPath(const QString& path);
	protected:
		static ResourcesFinder m_resourcesFinder;
	};
}

#define APP_RESOURCES qtuser_core::ResourcesFinder::instance()
#endif // _QTUSER_CORE_RESOURCESFINDER_1590131561766_H
