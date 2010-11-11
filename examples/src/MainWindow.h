#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Gui/pMainWindow>

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
	
	QPixmap scaledPixmap( const QString& filePath, const QSize& size = QSize() ) const;
	
	void initializeGui();
	void initializeMenuBar();
	QPlainTextEdit* initializePlainTextEdit();
	QTreeView* initializeActionsTreeView();

protected slots:
	void aAddAction_triggered();
	void aRemoveAction_triggered();
	void aEditTextNode_triggered();
	void aEditShortcuts_triggered();
	void dockToolBarManagerClassic();
	void dockToolBarManagerModern();
};

#endif // MAINWINDOW_H
