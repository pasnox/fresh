#ifndef PACTIONSNODESHORTCUTEDITOR_H
#define PACTIONSNODESHORTCUTEDITOR_H

#include "core/FreshExport.h"
#include "pActionsNode.h"

#include <QDialog>

class Ui_pActionsNodeShortcutEditor;
class pActionsNodeModel;
class pRecursiveSortFilterProxyModel;

class QAbstractButton;

class FRESH_EXPORT pActionsNodeShortcutEditor : public QDialog
{
	Q_OBJECT

public:
	pActionsNodeShortcutEditor( pActionsNodeModel* model, QWidget* parent = 0 );
	~pActionsNodeShortcutEditor();

protected:
	Ui_pActionsNodeShortcutEditor* ui;
	pActionsNodeModel* mModel;
	pRecursiveSortFilterProxyModel* mProxy;
	QHash<pActionsNode, QKeySequence> mOriginalShortcuts;
	
	pActionsNode selectedActionNode() const;
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
