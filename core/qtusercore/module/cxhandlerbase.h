#ifndef QTUSER_CORE_CXHANDLERBASE_H
#define QTUSER_CORE_CXHANDLERBASE_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QString>

namespace qtuser_core
{
	class QTUSER_CORE_API CXHandleBase
	{
	public:
		CXHandleBase();
		virtual ~CXHandleBase();

	public:
		virtual QStringList supportFilters();
		virtual QStringList enableFilters();
    virtual void cancelHandle();
	virtual void handle(const QString& fileName);
		virtual void handle(const QStringList& fileNames);

	virtual void openfail();
	private:
		QStringList m_fileNames;
		QString m_fileName;
	};
}

#endif // QTUSER_CORE_CXHANDLERBASE_H


