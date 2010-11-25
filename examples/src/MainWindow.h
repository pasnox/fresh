#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Gui/pMainWindow>
#include <Gui/pDockToolBarManager>

#include <QtGui>

class pActionsNodeModel;
class pActionsNodeMenuBar;
class pQueuedMessage;
class pStylesActionGroup;
class pConsole;
class pEnvironmentVariablesEditor;

class MainWindow : public pMainWindow
{
	Q_OBJECT
	friend class ConsoleCommands;
	
public:
	MainWindow( QWidget* parent = 0 );
	virtual ~MainWindow();

public slots:
	virtual void saveState();
	virtual void restoreState();

protected:
	QTabWidget* twPages;
	QPlainTextEdit* pteLog;
	QTreeView* tvActions;
	pActionsNodeModel* mActionsModel;
	pConsole* cShell;
	pStylesActionGroup* agStyles;
	pEnvironmentVariablesEditor* eveVariables;
	
	void createGui();
	void createMenuBar();
	void createPlainTextEdit();
	void createActionsTreeView();
	void createConsole();
	void createEnvironmentVariablesEditor();
	void createVersionsTests();
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
	void addQueuedMessage();
	void queuedMessageToolBarButtonClicked( QDialogButtonBox::StandardButton button, const pQueuedMessage& message );
	void setCurrentStyle( const QString& style );
	void openFileDialog();
	void openDirectoryDialog();
};

#endif // MAINWINDOW_H
