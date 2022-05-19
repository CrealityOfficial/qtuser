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

	QStringList CXHandleBase::supportFilters()
	{
		QStringList filters;
		return filters;
	}

	QStringList CXHandleBase::enableFilters()
	{
		return supportFilters();
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
}