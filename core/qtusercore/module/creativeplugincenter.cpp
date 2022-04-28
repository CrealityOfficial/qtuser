#include "creativeplugincenter.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>

CreativePluginCenter::CreativePluginCenter(QObject* parent)
	:QObject(parent)
{
}

CreativePluginCenter::~CreativePluginCenter()
{
}

void CreativePluginCenter::load()
{
	if (m_searchPathes.empty())
		m_searchPathes << QCoreApplication::applicationDirPath();

	QStringList nameFilters;
#ifdef __APPLE__
    QString plugin_root_path = QCoreApplication::applicationDirPath()+"/../Frameworks/";
    qDebug() << "plugin_root_path = " << plugin_root_path;
    m_searchPathes << QCoreApplication::applicationDirPath()+"/../Frameworks/";
    int pos_debug = plugin_root_path.indexOf("Debug");
    int pos_release = plugin_root_path.indexOf("Release");
    if(pos_debug >= 0 || pos_release)
    {
        m_searchPathes << QCoreApplication::applicationDirPath() + "/../../../";
    }
    qDebug() << "m_searchPathes = " << m_searchPathes;
    nameFilters << QString("libplugin*.dylib");
    qDebug() << "nameFilters = " << nameFilters;
#else
	nameFilters << QString("plugin*.dll");
#endif
	for (const QString& path : m_searchPathes)
	{
		QDir dir(path);
		QList<QFileInfo> fileInfos = dir.entryInfoList(nameFilters, QDir::Files);
		for (const QFileInfo& fileInfo : fileInfos)
		{
            qDebug() << "plugin load " << fileInfo.filePath();
			QPluginLoader loader(fileInfo.filePath());
			if (loader.load())
			{
				QObject* object = loader.instance();
				{
					CreativeInterface* interf = qobject_cast<CreativeInterface*>(object);
					if (interf)
					{
                        m_interfaces.insert(fileInfo.baseName(),interf);
					}else
						qDebug() << "Can't Find Interface in Plugin." << fileInfo.filePath();
				}
			}
			else
			{
				qDebug() << fileInfo.filePath() << " QPluginLoader Error. " << loader.errorString();
			}
		}
	}
}

void CreativePluginCenter::initialize()
{
    for (CreativeInterface* interf : m_interfaces.values())
	{
		interf->initialize();
	}
}

void CreativePluginCenter::uninitialize()
{	
    for (CreativeInterface* interf : m_interfaces.values())
	{
		interf->uninitialize();
	}

	qDeleteAll(m_interfaces);
}

CreativeInterface* CreativePluginCenter::getPluginByName(QString name)
{
    if(m_interfaces.contains(name))
    {
        return m_interfaces.value(name);
    }
    return nullptr;
}

