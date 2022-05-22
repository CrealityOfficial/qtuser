#ifndef _QTUSER_CORE_UNDOPROXY_1588906890012_H
#define _QTUSER_CORE_UNDOPROXY_1588906890012_H
#include "qtusercore/qtusercoreexport.h"
#include <QtWidgets/QUndoStack>

class QTUSER_CORE_API UndoProxy : public QObject
{
	Q_OBJECT

	Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
	Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)

	// Text for the undo and redo actions
	Q_PROPERTY(QString redoText READ redoText NOTIFY redoTextChanged)
	Q_PROPERTY(QString undoText READ undoText NOTIFY undoTextChanged)
public:
	UndoProxy(QObject* parent = nullptr);
	virtual ~UndoProxy();

	bool canRedo() const;
	bool canUndo() const;
	QString redoText() const;
	QString undoText() const;

	Q_INVOKABLE void undo();
	Q_INVOKABLE void redo();

	void setUndoStack(QUndoStack* undoStack);
signals:
	void canRedoChanged();
	void canUndoChanged();
	void redoTextChanged();
	void undoTextChanged();
protected:
	QUndoStack* m_undoStack;
};

#endif // _QTUSER_CORE_UNDOPROXY_1588906890012_H
