#include "cxopenandsavefilemanager.h"
#include "cxhandlerbase.h"

#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>
#include <QtCore/QProcess>
#include <QtWidgets/QFileDialog>

namespace qtuser_core
{

	CXFileOpenAndSaveManager::CXFileOpenAndSaveManager(QObject* parent)
		: QObject(parent)
		, m_invokeObject(nullptr)
		, m_State(OpenSaveState::oss_none)
		, m_externalHandler(nullptr)
	{
	}

	CXFileOpenAndSaveManager::~CXFileOpenAndSaveManager()
	{
	}

	void CXFileOpenAndSaveManager::init(QObject* obj)
	{
		m_invokeObject = obj;
	}

	void CXFileOpenAndSaveManager::clear()
	{

	}

	QString CXFileOpenAndSaveManager::title()
	{
		QString _title("Error");
		switch (m_State)
		{
		case OpenSaveState::oss_open:
		case OpenSaveState::oss_external_open:
			_title = QString("Open");
			break;
		case OpenSaveState::oss_external_save:
		case OpenSaveState::oss_save:
			_title = QString("Save");
			break;
		default:
			break;
		}
		return _title;
	}

	QStringList CXFileOpenAndSaveManager::nameFilters()
	{
		QStringList filterList;
		switch (m_State)
		{
		case OpenSaveState::oss_open:
			filterList = generateFiltersFromHandlers(false);
			break;
		case OpenSaveState::oss_save:
			filterList = generateFiltersFromHandlers(true);
			break;
		case OpenSaveState::oss_external_open:
		case OpenSaveState::oss_external_save:
			filterList = generateFilters(m_externalFilterList);
			break;
		default:
			break;
		}

		return filterList;
	}

	QStringList CXFileOpenAndSaveManager::generateFilters(const QStringList& extensions)
	{
		QStringList filters;
		QString filtersall;
		for (const QString& ext : extensions)
		{
			filters << QString("*.%1 ").arg(ext);
			filtersall += QString("*.%1 ").arg(ext);
		}
		if (-1 == filters.indexOf(filtersall))
		{
			filters.push_front(filtersall);
		}
		return filters;
	}

	QStringList CXFileOpenAndSaveManager::generateFiltersFromHandlers(bool saveState)
	{
		QMap<QString, CXHandleBase*>& handlers = saveState ? m_saveHandlers : m_openHandlers;
		QStringList filters;
		QString allSuffix;
		for (QMap<QString, CXHandleBase*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
		{
			QStringList enableFilters = it.value()->enableFilters();
			for (const QString& ext : enableFilters)
			{
				QString suffix = QString("*.%1 ").arg(ext);
				if (!filters.contains(suffix))
				{
					filters << suffix;
					allSuffix += suffix;
				}
			}
		}
		filters.push_front(allSuffix);
		return filters;
	}

	QString CXFileOpenAndSaveManager::generateFilterFromHandlers(bool saveState)
	{
		QMap<QString, CXHandleBase*>& handlers = saveState ? m_saveHandlers : m_openHandlers;
		QString filter;
		QStringList extensions;
		filter += "FILES (";
		for (QMap<QString, CXHandleBase*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
		{
			QStringList enableFilters = it.value()->enableFilters();
			for (const QString& ext : enableFilters)
			{
				if (!extensions.contains(ext))
				{
					QString suffix = QString("*.%1 ").arg(ext);
					filter += suffix;
					extensions << ext;
				}
			}
		}
		filter += ")";

		return filter;
	}

	void CXFileOpenAndSaveManager::fileOpen(const QString& url)
	{
		openWithUrl(QUrl(url));
	};

	void CXFileOpenAndSaveManager::filesOpen(const QList<QUrl>& urls)
	{
		QStringList fileNames;
		for (const QUrl& url : urls)
			fileNames << url.toLocalFile();

		openWithNames(fileNames);
	};

	void CXFileOpenAndSaveManager::fileSave(const QString& url)
	{
		saveWithUrl(QUrl(url));
	}

	void CXFileOpenAndSaveManager::open(CXHandleBase* receiver, const QStringList& filters)
	{
		m_State = OpenSaveState::oss_open;
		if (receiver)
		{
			m_externalHandler = receiver;
			m_externalFilterList = filters;
			m_State = OpenSaveState::oss_external_open;
		}

		if (m_invokeObject)
		{
			QMetaObject::invokeMethod(m_invokeObject,
				"requestOpenFileDialog", Q_ARG(QVariant, QVariant::fromValue(this)));
		}
		else
		{
			auto f = [this](const QStringList& files) {
				openWithNames(files);
			};

			QString filter = generateFilterFromHandlers(false);
			dialogOpenFiles(filter, f);
		}
	}

	void CXFileOpenAndSaveManager::save(CXHandleBase* receiver, const QStringList& filters)
	{
		m_State = OpenSaveState::oss_save;
		if (receiver)
		{
			m_externalHandler = receiver;
			m_externalFilterList = filters;
			m_State = OpenSaveState::oss_external_save;
		}

		if (m_invokeObject)
		{
			QMetaObject::invokeMethod(m_invokeObject,
				"requestSaveFileDialog", Q_ARG(QVariant, QVariant::fromValue(this)));
		}
		else
		{

		}
	}

	void CXFileOpenAndSaveManager::openWithParams(const QStringList& params)
	{
		if (params.size() > 1)
		{
			qDebug() << "params length > 1. maybe need open file";
			QString needOpenName;

			for (const QString& fileName : params)
			{
				QFile file(fileName);
				QFileInfo fileInfo(fileName);
				QString suffix = fileInfo.suffix();
				if (file.exists() && m_openFilterList.contains(suffix))
				{
					needOpenName = fileName;
					break;
				}
			}

			openWithName(needOpenName);
		}
	}

	bool CXFileOpenAndSaveManager::openWithName(const QString& fileName)
	{
		if (m_externalHandler)
		{
			m_externalHandler->handle(fileName);
			m_externalHandler = nullptr;
			m_State = OpenSaveState::oss_none;
			for (CXFileOpenSaveCallback* callback : m_callbacks)
				callback->onFileOpened(fileName);
			return true;
		}

		QFileInfo info(fileName);
		QString suffix = info.suffix();
		suffix = suffix.toLower();
		return openWithNameSuffix(fileName, suffix);
	}

	void CXFileOpenAndSaveManager::openWithNames(const QStringList& fileNames)
	{
		if (m_externalHandler)
		{
			m_externalHandler->handle(fileNames);
			m_externalHandler = nullptr;
			m_State = OpenSaveState::oss_none;
			return;
		}

		if (fileNames.size() > 0)
		{
			QFileInfo info(fileNames.at(0));
			QString suffix = info.suffix();
			suffix = suffix.toLower();
			CXHandleBase* handler = findHandler(suffix, m_openHandlers);
			if (!handler)
			{
				qDebug() << "not register handler for this file type!";
				return;
			}

			handler->handle(fileNames);
			openWithNamesSuffix(fileNames);
		}
	}

	bool CXFileOpenAndSaveManager::openWithUrl(const QUrl& url)
	{
		return openWithName(url.toLocalFile());
	}

	bool CXFileOpenAndSaveManager::openWithUrl(const QList<QUrl>& urls)
	{
		if (urls.size() == 1)
		{
			return openWithName(urls[0].toLocalFile());
		}
		else if (urls.size() > 1)
		{
			QStringList fileNames;
			for (QUrl url : urls)
			{
				fileNames.push_back(url.toLocalFile());
			}
			openWithNames(fileNames);
			return true;
		}
		return false;
	}

	bool CXFileOpenAndSaveManager::saveWithName(const QString& fileName)
	{
		m_lastSaveFile = fileName;
		if (m_externalHandler)
		{
			m_externalHandler->handle(fileName);
			m_externalHandler = nullptr;
			m_State = OpenSaveState::oss_none;
			return true;
		}

		QFileInfo info(fileName);
		QString suffix = info.suffix();
		suffix = suffix.toLower();
		return saveWithNameSuffix(fileName, suffix);
	}

	bool CXFileOpenAndSaveManager::saveWithUrl(const QUrl& url)
	{
		return saveWithName(url.toLocalFile());
	}

	bool CXFileOpenAndSaveManager::openWithNameSuffix(const QString& fileName, const QString suffix)
	{
		CXHandleBase* handler = findHandler(suffix, m_openHandlers);
		if (!handler)
		{
			qDebug() << "not register handler for this file type!";
			return false;
		}

		handler->handle(fileName);
		for (CXFileOpenSaveCallback* callback : m_callbacks)
			callback->onFileOpened(fileName);
		return true;
	}

	bool CXFileOpenAndSaveManager::openWithNamesSuffix(const QStringList& fileNames)
	{
		for (CXFileOpenSaveCallback* callback : m_callbacks)
		{
			for (QString fileName : fileNames)
			{
				callback->onFileOpened(fileName);
			}
		}
		return true;
	}

	bool CXFileOpenAndSaveManager::saveWithNameSuffix(const QString& fileName, const QString suffix)
	{
		CXHandleBase* handler = findHandler(suffix, m_saveHandlers);
		if (!handler)
		{
			qDebug() << "not register handler for this file type!";
			return false;
		}

		handler->handle(fileName);
		return true;
	}

	CXHandleBase* CXFileOpenAndSaveManager::findHandler(const QString& suffix, QMap<QString, CXHandleBase*>& handlers)
	{
		QMap<QString, CXHandleBase*>::iterator it = handlers.find(suffix);
		if (it != handlers.end())
			return it.value();
		return nullptr;
	}

	void CXFileOpenAndSaveManager::registerOpenHandler(CXHandleBase* handler)
	{
		if (handler)
			registerOpenHandler(handler->supportFilters(), handler);
	}

	void CXFileOpenAndSaveManager::registerOpenHandler(const QStringList& suffixes, CXHandleBase* handler)
	{
		for (const QString& suffix : suffixes)
			registerOpenHandler(suffix, handler);
	}

	void CXFileOpenAndSaveManager::unRegisterOpenHandler(const QStringList& suffixes)
	{
		for (const QString& suffix : suffixes)
			unRegisterOpenHandler(suffix);
	}

	void CXFileOpenAndSaveManager::registerOpenHandler(const QString& suffix, CXHandleBase* handler)
	{
		if (registerHandler(suffix, handler, m_openHandlers))
			m_openFilterList << suffix;
	}

	void CXFileOpenAndSaveManager::unRegisterOpenHandler(const QString& suffix)
	{
		unRegisterHandler(suffix, m_openHandlers);
		m_openFilterList.removeOne(suffix);
	}

	void CXFileOpenAndSaveManager::registerSaveHandler(CXHandleBase* handler)
	{
		if (handler)
			registerSaveHandler(handler->supportFilters(), handler);
	}

	void CXFileOpenAndSaveManager::registerSaveHandler(const QStringList& suffixes, CXHandleBase* handler)
	{
		for (const QString& suffix : suffixes)
			registerSaveHandler(suffix, handler);
	}

	void CXFileOpenAndSaveManager::unRegisterSaveHandler(const QStringList& suffixes)
	{
		for (const QString& suffix : suffixes)
			unRegisterSaveHandler(suffix);
	}

	void CXFileOpenAndSaveManager::registerSaveHandler(const QString& suffix, CXHandleBase* handler)
	{
		if (registerHandler(suffix, handler, m_saveHandlers))
			m_saveFilterList << suffix;
	}

	void CXFileOpenAndSaveManager::unRegisterSaveHandler(const QString& suffix)
	{
		unRegisterHandler(suffix, m_saveHandlers);
		m_saveFilterList.removeOne(suffix);
	}

	bool CXFileOpenAndSaveManager::registerHandler(const QString& suffix, CXHandleBase* handler, QMap<QString, CXHandleBase*>& handlers)
	{
		if (!handler || handlers.contains(suffix))
		{
			qDebug() << "handler exist for " << suffix;
			return false;
		}

		handlers.insert(suffix, handler);
		return true;
	}

	void CXFileOpenAndSaveManager::unRegisterHandler(const QString& suffix, QMap<QString, CXHandleBase*>& handlers)
	{
		handlers.remove(suffix);
	}

	QString CXFileOpenAndSaveManager::lastOpenFileName()
	{
		return m_lastOpenFile;
	}

	QString CXFileOpenAndSaveManager::lastSaveFileName()
	{
		return m_lastSaveFile;
	}

	void CXFileOpenAndSaveManager::setLastSaveFileName(QString filePath)
	{
		m_lastSaveFile = filePath;
	}

	void CXFileOpenAndSaveManager::openDesktopFolder()
	{
		QFileInfo info(m_lastSaveFile);
		QDesktopServices::openUrl(QUrl::fromLocalFile(info.path()));
	}

	void CXFileOpenAndSaveManager::openLastSaveFolder()
	{
		QFileInfo fileInfo(m_lastSaveFile);
		openFolder(fileInfo.absolutePath());
	}

	void CXFileOpenAndSaveManager::openFolder(const QString& folder)
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(folder));
	}

	void CXFileOpenAndSaveManager::addCXFileOpenSaveCallback(CXFileOpenSaveCallback* callback)
	{
		if (callback && !m_callbacks.contains(callback))
			m_callbacks.append(callback);
	}

	void CXFileOpenAndSaveManager::removeCXFileOpenSaveCallback(CXFileOpenSaveCallback* callback)
	{
		if (callback)
			m_callbacks.removeOne(callback);
	}

	void dialogOpenFiles(const QString& filter, loadFunc func)
	{
		if (!func)
			return;

		QStringList fileName = QFileDialog::getOpenFileNames(
			nullptr, QObject::tr("OpenFile"),
			QString(), filter);

		if (fileName.isEmpty())
			return;

		func(fileName);
	}
}