#include "keyworditem.h"
#include "ui_keyword.h"

namespace qtuser_core
{
	KeywordItem::KeywordItem(QWidget* parent)
		:QWidget(parent)
		, m_ui(new Ui::keyword())
	{
		m_ui->setupUi(this);
	}

	KeywordItem::~KeywordItem()
	{
		delete m_ui;
	}

	void KeywordItem::setKey(const QString& key)
	{
		m_ui->key_text->setText(key);
	}

	QString KeywordItem::key()
	{
		return m_ui->key_text->text();
	}

	QString KeywordItem::value()
	{
		return m_ui->value_text->text();
	}
}
