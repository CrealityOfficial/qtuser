#ifndef _NULLSPACE_MAINWINDOW_1591970265844_H
#define _NULLSPACE_MAINWINDOW_1591970265844_H
#include "qtusercore/qtusercoreexport.h"
#include <QtWidgets/QWidget>

namespace Ui
{
	class AutoCodePanel;
}

class TemplateLibrary;
namespace qtuser_core
{
	class KeywordItem;
	class QTUSER_CORE_API AutoCodePanel : public QWidget
	{
		Q_OBJECT
	public:
		AutoCodePanel(QWidget* parent = nullptr);
		virtual ~AutoCodePanel();

		void setSearchPath(const QString& path);
		void setOutPath(const QString& path);
	public slots:
		void on_path_button_clicked();
		void on_gen_button_clicked();
		void on_out_button_clicked();
		void on_templates_currentIndexChanged(const QString& templateName);
	protected:
		void clearKeywordItems();
		void buildKeywordItems();
	protected:
		Ui::AutoCodePanel* m_ui;
		TemplateLibrary* m_templateLibrary;

		QList<KeywordItem*> m_tempKeywords;
	};
}
#endif // _NULLSPACE_MAINWINDOW_1591970265844_H
