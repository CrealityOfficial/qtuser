#include "templatelibrary.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>

TemplateLibrary::TemplateLibrary(QObject* parent)
	:QObject(parent)
{
	m_pathes << QCoreApplication::applicationDirPath();
}

TemplateLibrary::~TemplateLibrary()
{
}

void TemplateLibrary::addSearchPath(const QStringList& pathes)
{
	m_pathes << pathes;
}

Template* TemplateLibrary::get(const QString& name)
{
	QHash<QString, Template*>::iterator it = m_templates.find(name);
	if (it != m_templates.end())
		return it.value();

	Template* t = load(name);
	if (t)
	{
		m_templates.insert(name, t);
	}
	return t;
}

Template* TemplateLibrary::load(const QString& name)
{
	Template* t = new Template(this);
	for (const QString& path : m_pathes)
	{
		QString templateFile = path + "/" + name + ".template";
		if (t->parse(templateFile))
		{
			break;
		}
	}
	if (!t->valid())
	{
		delete t;
		t = nullptr;
	}
	return t;
}

Template* TemplateLibrary::loadFile(const QString& fileName)
{
	Template* t = new Template(this);
	t->parse(fileName);

	if (!t->valid())
	{
		delete t;
		t = nullptr;
	}
	return t;
}

void TemplateLibrary::loadAll(const QString& directory)
{
	QStringList nameFilters;
	nameFilters << QString("*.template");

	QDir dir(directory);
	QList<QFileInfo> fileInfos = dir.entryInfoList(nameFilters, QDir::Files);
	for (const QFileInfo& fileInfo : fileInfos)
	{
		Template* t = loadFile(fileInfo.filePath());
		if (t)
		{
			m_templates.insert(fileInfo.fileName(), t);
		}
	}
}

QStringList TemplateLibrary::enumAll()
{
	QStringList libraries;
	for (QHash<QString, Template*>::iterator it = m_templates.begin(); it != m_templates.end(); ++it)
	{
		libraries << it.key();
	}
	return libraries;
}
