#include "undoproxy.h"

UndoProxy::UndoProxy(QObject* parent)
	:QObject(parent)
	, m_undoStack(nullptr)
{
}

UndoProxy::~UndoProxy()
{
}

void UndoProxy::setUndoStack(QUndoStack* undoStack)
{
	if (m_undoStack == undoStack)
		return;

	m_undoStack = undoStack;
	if (m_undoStack)
	{
		connect(m_undoStack, SIGNAL(canRedoChanged(bool)), this, SIGNAL(canRedoChanged()));
		connect(m_undoStack, SIGNAL(canUndoChanged(bool)), this, SIGNAL(canUndoChanged()));
		connect(m_undoStack, SIGNAL(redoTextChanged(QString)), this, SIGNAL(redoTextChanged()));
		connect(m_undoStack, SIGNAL(undoTextChanged(QString)), this, SIGNAL(undoTextChanged()));
	}

	emit canRedoChanged();
	emit canUndoChanged();
	emit redoTextChanged();
	emit undoTextChanged();
}

void UndoProxy::undo()
{
	if (m_undoStack)
		m_undoStack->undo();
}

void UndoProxy::redo()
{
	if (m_undoStack)
		m_undoStack->redo();
}

bool UndoProxy::canRedo() const
{
	if (m_undoStack)
		return m_undoStack->canRedo();
	return false;
}

bool UndoProxy::canUndo() const
{
	if (m_undoStack)
		return m_undoStack->canUndo();
	return false;
}

QString UndoProxy::redoText() const
{
	if (m_undoStack)
		return m_undoStack->redoText();
	return QString();
}

QString UndoProxy::undoText() const
{
	if (m_undoStack)
		return m_undoStack->undoText();
	return QString();
}
