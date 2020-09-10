#ifndef _NULLSPACE_TEMPLATELIBRARY_1591949333365_H
#define _NULLSPACE_TEMPLATELIBRARY_1591949333365_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>
#include <QtCore/QHash>
#include "template.h"

class QTUSER_CORE_API TemplateLibrary: public QObject
{
public:
	TemplateLibrary(QObject* parent = nullptr);
	virtual ~TemplateLibrary();

	Template* get(const QString& name);
	void addSearchPath(const QStringList& pathes);
	void loadAll(const QString& directory);

	QStringList enumAll();
private:
	Template* load(const QString& name);
	Template* loadFile(const QString& fileName);
protected:
	QHash<QString, Template*> m_templates;
	QStringList m_pathes;
};
#endif // _NULLSPACE_TEMPLATELIBRARY_1591949333365_H
