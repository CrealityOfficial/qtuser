#ifndef _NULLSPACE_KEYWORDLIST_1591950619889_H
#define _NULLSPACE_KEYWORDLIST_1591950619889_H
#include "qtusercore/qtusercoreexport.h"
#include <QtCore/QObject>
#include <QtCore/QHash>

class QTUSER_CORE_API KeywordList: public QObject
{
public:
	KeywordList(QObject* parent = nullptr);
	virtual ~KeywordList();

	void set(const QStringList& builtinKeys);
	void set(const QString& key, const QString& value);
	QString get(const QString& key);

	void setTemplate(const QString& name);
	QString use();

	QString directory();
	void setDirectory(const QString& directory);

	bool test(const QString& key);
	static bool builtin(const QString& key);
	void updateList(const QHash<QString, QString>& keyvalues);
protected:
	QHash<QString, QString> m_keywords;
	QString m_useTemplate;
	QString m_directory;
};
#endif // _NULLSPACE_KEYWORDLIST_1591950619889_H
