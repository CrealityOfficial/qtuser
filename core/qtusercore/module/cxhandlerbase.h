#ifndef QTUSER_CORE_CXHANDLERBASE_H
#define QTUSER_CORE_CXHANDLERBASE_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QString>

namespace qtuser_core
{
	class QTUSER_CORE_API CXHandleBase : public QObject
	{
		Q_OBJECT
	public:
		CXHandleBase(QObject* parent = nullptr);
		virtual ~CXHandleBase();

	public:
		virtual QString filter();     //format "XXXX (*.sufix1 *.sufix2 *.sufix3)"
		virtual void cancelHandle();
		virtual void handle(const QString& fileName);
		virtual void handle(const QStringList& fileNames);

		virtual void openfail();
		QStringList suffixesFromFilter();
	private:
		QStringList m_fileNames;
		QString m_fileName;
	};
}

#endif // QTUSER_CORE_CXHANDLERBASE_H


