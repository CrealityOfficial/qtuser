#include "cxhandlerbase.h"

namespace qtuser_core
{
	CXHandleBase::CXHandleBase(QObject* parent)
		:QObject(parent)
	{
	}

	CXHandleBase::~CXHandleBase()
	{

	}

	QString CXHandleBase::filter()
	{
		return QString("CXHandleBase ()");
	}

	void CXHandleBase::cancelHandle()
	{
	
	}

	void CXHandleBase::handle(const QString& fileName)
	{
		m_fileName = fileName;
	}

	void CXHandleBase::handle(const QStringList& fileNames)
	{
		for (QString fileName : fileNames)
		{
			m_fileNames << fileName;
		}
	}

	void CXHandleBase::openfail()
	{
		return ;
	}

	QStringList CXHandleBase::suffixesFromFilter()
	{
		QStringList suffixes;
		QString line = filter();
		int i1 = line.indexOf("(");
		int i2 = line.lastIndexOf(")");
		if (i1 >= 0 && i2 >= 0)
		{
			QString line1 = line.mid(i1 + 1, i2 - i1 - 1);
			QStringList lines = line1.split(" ");
			for (const QString& l : lines)
			{
				int i = l.lastIndexOf(".");
				if (i >= 0)
				{
					QString suf = l.mid(i + 1);
					suffixes.append(suf);
				}
			}
		}
		return suffixes;
	}
}