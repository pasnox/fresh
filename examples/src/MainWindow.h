#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Gui/pMainWindow>
#include <Gui/pDockToolBarManager>

#include <QtGui>

class pActionsNodeModel;
class pActionsNodeMenuBar;

class MainWindow : public pMainWindow
{
	Q_OBJECT
	
public:
	MainWindow( QWidget* parent = 0 );
	virtual ~MainWindow();

protected:
	QTabWidget* twPages;
	QPlainTextEdit* pteLog;
	QTreeView* tvActions;
	pActionsNodeModel* mActionsModel;
	
	void initializeGui();
	void initializeMenuBar();
	QPlainTextEdit* initializePlainTextEdit();
	QTreeView* initializeActionsTreeView();
	
	void versionsTests();
	void createListEditors();
	void createCustomWidgets();

protected slots:
	void aAddAction_triggered();
	void aRemoveAction_triggered();
	void aEditTextNode_triggered();
	void aEditShortcuts_triggered();
	void dockToolBarManagerModeChanged( pDockToolBarManager::Mode mode );
	void dockToolBarManagerClassic();
	void dockToolBarManagerModern();
};

#endif // MAINWINDOW_H
