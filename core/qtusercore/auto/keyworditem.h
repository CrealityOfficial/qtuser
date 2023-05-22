#ifndef _NULLSPACE_KEYWORDITEM_1592008927012_H
#define _NULLSPACE_KEYWORDITEM_1592008927012_H
#include "qtusercore/qtusercoreexport.h"
#include <QtWidgets/QWidget>

namespace Ui
{
	class keyword;
}

namespace qtuser_core
{
	class TemplateLibrary;
	class KeywordItem : public QWidget
	{
		Q_OBJECT
	public:
		KeywordItem(QWidget* parent = nullptr);
		virtual ~KeywordItem();

		void setKey(const QString& key);
		QString key();

		QString value();
	protected:
		Ui::keyword* m_ui;
	};
}

#endif // _NULLSPACE_KEYWORDITEM_1592008927012_H
