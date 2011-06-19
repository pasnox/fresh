class pActionsShortcutEditor : QDialog
{
%TypeHeaderCode
#include <gui/actionmanager/pActionsShortcutEditor.h>
%End

public:
	/*!
		Create an editor for editing the model \a model and having parent \a parent.
	*/
	pActionsShortcutEditor( pActionsModel* model, QWidget* parent = 0 );
	/*!
		Destroys the editor.
	*/
	virtual ~pActionsShortcutEditor();

protected:
	/*!
		Return the selected node of type pActionsNode::Action.
	*/
	QAction* selectedAction() const;
	/*!
		Set the \a node \a shortcut.
		
		The \a error is shown if the shortcut can't be setted.
	*/
	void setShortcut( QAction* action, const QString& shortcut );

private slots:
	void on_leFilter_textChanged( const QString& text );
	void tvActions_selectionModel_selectionChanged();
	void on_kseShortcut_textChanged( const QString& text );
	void on_tbSet_clicked();
	void on_tbClear_clicked();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

