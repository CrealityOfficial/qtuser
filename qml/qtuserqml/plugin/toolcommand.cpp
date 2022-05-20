#include "qtuserqml/plugin/toolcommand.h"

ToolCommand::ToolCommand(QObject* parent)
	:QObject(parent)
{
}

ToolCommand::~ToolCommand()
{
}

bool ToolCommand::enabled()
{
	return enableImpl();
}

bool ToolCommand::enableImpl()
{
	return false;
}

void ToolCommand::execute()
{
	onExecute();
}

void ToolCommand::onExecute()
{
}
