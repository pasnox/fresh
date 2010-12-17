class pActionsNodeShortcutEditor : QDialog
{
%TypeHeaderCode
#include <../src/gui/actionmanager/pActionsNodeShortcutEditor.h>
%End


public:
	pActionsNodeShortcutEditor( pActionsNodeModel* model, QWidget* parent = 0 );
	~pActionsNodeShortcutEditor();

private slots:
	void on_leFilter_textChanged( const QString& text );
	void tvActions_selectionModel_selectionChanged();
	void on_kseShortcut_textChanged( const QString& text );
	void on_tbSet_clicked();
	void on_tbClear_clicked();
	void on_dbbButtons_clicked( QAbstractButton* button );
};

