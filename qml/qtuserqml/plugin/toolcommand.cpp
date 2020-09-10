#include "qtuserqml/plugin/toolcommand.h"

ToolCommand::ToolCommand(QObject* parent)
	:QObject(parent)
    , m_enabled(true),
      orderindex(100)
{
}

ToolCommand::~ToolCommand()
{
}

bool ToolCommand::enabled()
{
	return m_enabled;
}

QString ToolCommand::enabledIcon() const
{
	return m_enabledIcon;
}

QString ToolCommand::disableIcon() const
{
	return m_disableIcon;
}

QString ToolCommand::pressedIcon() const
{
	return m_pressedIcon;
}

QString ToolCommand::source() const
{
	return m_source;
}

QString ToolCommand::name() const
{
	return m_name;
}

void ToolCommand::setEnabledIcon(const QString& icon)
{
	m_enabledIcon = icon;
}

void ToolCommand::setPressedIcon(const QString& icon)
{
	m_pressedIcon = icon;
}

void ToolCommand::setName(const QString& name)
{
	m_name = name;
}

void ToolCommand::setSource(const QString& source)
{
	m_source = source;
}

void ToolCommand::setEnabled(bool enabled)
{
	if (m_enabled != enabled)
	{
		m_enabled = enabled;
		emit enabledChanged();
	}
}

void ToolCommand::execute()
{
	onExecute();
	emit executed();
}

void ToolCommand::onExecute()
{

}
bool ToolCommand::checkSelect()
{
    return true;
}

