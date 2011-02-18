#ifndef PACTIONSNODESHORTCUTEDITOR_H
#define PACTIONSNODESHORTCUTEDITOR_H

/*!
	\file pActionsNodeShortcutEditor.h
	\brief This class allow to edit the pActionsNode shortcuts of a pActionsNodeModel.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pActionsNode.h"

#include <QDialog>

class Ui_pActionsNodeShortcutEditor;
class pActionsNodeModel;
class pRecursiveSortFilterProxyModel;

class QAbstractButton;

/*!
	\ingroup Gui
	\class pActionsNodeShortcutEditor
	\brief This class allow to edit the pActionsNode shortcuts of a pActionsNodeModel.
*/
class FRESH_EXPORT pActionsNodeShortcutEditor : public QDialog
{
	Q_OBJECT

public:
	/*!
		Create an editor for editing the model \a model and having parent \a parent.
	*/
	pActionsNodeShortcutEditor( pActionsNodeModel* model, QWidget* parent = 0 );
	/*!
		Destroys the editor.
	*/
	~pActionsNodeShortcutEditor();

protected:
	Ui_pActionsNodeShortcutEditor* ui;
	pActionsNodeModel* mModel;
	pRecursiveSortFilterProxyModel* mProxy;
	QHash<pActionsNode, QKeySequence> mOriginalShortcuts;
	/*!
		Return the selected node of type pActionsNode::Action.
	*/
	pActionsNode selectedActionNode() const;
	/*!
		Set the \a node \a shortcut.
		
		The \a error is shown if the shortcut can't be setted.
	*/
	void setNodeShortcut( pActionsNode& node, const QString& shortcut, const QString& error );

private slots:
	void on_leFilter_textChanged( const QString& text );
	void tvActions_selectionModel_selectionChanged();
	void on_kseShortcut_textChanged( const QString& text );
	void on_tbSet_clicked();
	void on_tbClear_clicked();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

#endif // PACTIONSNODESHORTCUTEDITOR_H
