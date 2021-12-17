#include "autocodepanel.h"
#include "ui_autocodepanel.h"

#include "keyworditem.h"

#include "qtusercore/auto/templatelibrary.h"
#include "qtusercore/auto/keywordlist.h"

#include <QtWidgets/QFileDialog>

namespace qtuser_core
{
	AutoCodePanel::AutoCodePanel(QWidget* parent)
		: QWidget(parent)
		, m_ui(new Ui::AutoCodePanel())
		, m_templateLibrary(nullptr)
	{
		m_ui->setupUi(this);
		m_templateLibrary = new TemplateLibrary(this);
	}

	AutoCodePanel::~AutoCodePanel()
	{
	}

	void AutoCodePanel::on_path_button_clicked()
	{
		QString templatePath = QFileDialog::getExistingDirectory(this, "Template Pathes");
		setSearchPath(templatePath);
	}

	void AutoCodePanel::on_gen_button_clicked()
	{
		Template* codeTemplate = m_templateLibrary->get(m_ui->templates->currentText());
		if (!codeTemplate)
			return;

		QString directory = m_ui->out_path->text();
		QList<KeywordItem*> keywordItems = m_tempKeywords;
		QString names;
		for (KeywordItem* item : keywordItems)
		{
			if (item->key() == "name")
			{
				names = item->value();
				break;
			}
		}
		
		QHash<QString, QString> keyvalues;
		for (KeywordItem* item : keywordItems)
		{
			keyvalues.insert(item->key(), item->value());
		}
		
		KeywordList keywordList;

		QStringList ns = names.split(" ");
		if (ns.size() > 0)
		{
			for (QString n : ns)
			{
				keyvalues["name"] = n;
				keywordList.updateList(keyvalues);
				keywordList.setDirectory(directory);
				codeTemplate->write(&keywordList);
			}
		}
		else
		{
			keywordList.updateList(keyvalues);
			keywordList.setDirectory(directory);
			codeTemplate->write(&keywordList);
		}
	}

	void AutoCodePanel::on_out_button_clicked()
	{
		QString outPath = QFileDialog::getExistingDirectory(this, "Output Pathes");
		if(!outPath.isEmpty())
			setOutPath(outPath);
	}

	void AutoCodePanel::on_templates_currentIndexChanged(const QString& templateName)
	{
		clearKeywordItems();
		buildKeywordItems();
	}

	void AutoCodePanel::setOutPath(const QString& path)
	{
		m_ui->out_path->setText(path);
	}

	void AutoCodePanel::setSearchPath(const QString& templatePath)
	{
		m_ui->path->setText(templatePath);
		m_templateLibrary->loadAll(templatePath);
		QStringList templateList = m_templateLibrary->enumAll();
		
		m_ui->templates->clear();
		m_ui->templates->addItems(templateList);
	}

	void AutoCodePanel::clearKeywordItems()
	{
		for (QWidget* widget : m_tempKeywords)
			m_ui->keywords->removeWidget(widget);
		
		qDeleteAll(m_tempKeywords);
		m_tempKeywords.clear();
	}

	void AutoCodePanel::buildKeywordItems()
	{
		Template* codeTemplate = m_templateLibrary->get(m_ui->templates->currentText());
		if (codeTemplate)
		{
			QStringList keys = codeTemplate->userKeys();
			keys.sort();
			for (QString key : keys)
			{
				KeywordItem* item = new KeywordItem(this);
				item->setKey(key);
				m_ui->keywords->addWidget(item);
				m_tempKeywords.push_back(item);
			}
		}
	}
}
