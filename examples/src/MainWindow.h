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
	QPushButton* pbAddAction;
	QPushButton* pbRemoveAction;
	QPushButton* pbEditTextNode;
	QPushButton* pbEditShortcuts;
	pActionsNodeModel* mActionsModel;
	pActionsNodeMenuBar* mMenuBar;
	QTreeView* tvActions;
	
	QPixmap scaledPixmap( const QString& filePath, const QSize& size = QSize() ) const;

protected slots:
	void pbAddAction_clicked();
	void pbRemoveAction_clicked();
	void pbEditTextNode_clicked();
	void pbEditShortcuts_clicked();
	void dockToolBarManagerClassic();
	void dockToolBarManagerModern();
};

#endif // MAINWINDOW_H
