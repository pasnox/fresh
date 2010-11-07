#include "MainWindow.h"

#include <core/pSettings.h>
#include <gui/pDockToolBarManager.h>
#include <gui/pDockToolBar.h>
#include <gui/pActionsNodeModel.h>
#include <gui/pActionsNodeShortcutEditor.h>
#include <gui/pActionsNodeMenuBar.h>
#include <gui/pStringListEditor.h>
#include <gui/pFileListEditor.h>
#include <gui/pPathListEditor.h>

#include <modeltest.h>

#include <QDebug>

MainWindow::MainWindow( QWidget* parent )
	: pMainWindow( parent )
{
	QWidget* centralWidget = new QWidget( this );
	QVBoxLayout* hl = new QVBoxLayout( centralWidget );
	
	mActionsModel = new pActionsNodeModel( this );
	mMenuBar = new pActionsNodeMenuBar( this );
	mMenuBar->setModel( mActionsModel );
	
	tvActions = new QTreeView( this );
	tvActions->setModel( mActionsModel );
	
	pbAddAction = new QPushButton( tr( "Add new action" ), this );
	pbRemoveAction = new QPushButton( tr( "Remove selected action" ), this );
	pbEditTextNode = new QPushButton( tr( "Edit selected node text" ), this );
	pbEditShortcuts = new QPushButton( tr( "Edit the actions shortcuts" ), this );
	
	QHBoxLayout* buttonsLayout = new QHBoxLayout;
	buttonsLayout->addWidget( pbAddAction );
	buttonsLayout->addWidget( pbRemoveAction );
	buttonsLayout->addWidget( pbEditTextNode );
	buttonsLayout->addWidget( pbEditShortcuts );
	
	hl->addWidget( new QPlainTextEdit( this ) );
	hl->addLayout( buttonsLayout );
	hl->addWidget( tvActions );

	setMenuBar( mMenuBar );
	setCentralWidget( centralWidget );
	
	// connections
	connect( pbAddAction, SIGNAL( clicked() ), this, SLOT( pbAddAction_clicked() ) );
	connect( pbRemoveAction, SIGNAL( clicked() ), this, SLOT( pbRemoveAction_clicked() ) );
	connect( pbEditTextNode, SIGNAL( clicked() ), this, SLOT( pbEditTextNode_clicked() ) );
	connect( pbEditShortcuts, SIGNAL( clicked() ), this, SLOT( pbEditShortcuts_clicked() ) );
	
	// **********************
	
	new ModelTest( mActionsModel, this );
	
	// string list editor
	QDockWidget* dwStringListEditor = new QDockWidget( this );
	dwStringListEditor->setObjectName( "DockStringListEditor" );
	dwStringListEditor->toggleViewAction()->setObjectName( "DockStringListEditorViewAction" );
	dwStringListEditor->setWidget( new pStringListEditor( tr( "Edit strings" ), this ) );
	dockToolBar( Qt::LeftToolBarArea )->addDock( dwStringListEditor, tr( "String List Editor" ), QIcon( ":/fresh/country-flags/fr.png" ) );
	
	// file list editor
	QDockWidget* dwFileListEditor = new QDockWidget( this );
	dwFileListEditor->setObjectName( "DockFileListEditor" );
	dwFileListEditor->toggleViewAction()->setObjectName( "DockFileListEditorViewAction" );
	dwFileListEditor->setWidget( new pFileListEditor( tr( "Edit files" ), ".", "*.png", this ) );
	dockToolBar( Qt::LeftToolBarArea )->addDock( dwFileListEditor, tr( "File List Editor" ), QIcon( ":/fresh/country-flags/gb.png" ) );
	
	// path list editor
	QDockWidget* dwPathListEditor = new QDockWidget( this );
	dwPathListEditor->setObjectName( "DockPathListEditor" );
	dwPathListEditor->toggleViewAction()->setObjectName( "DockPathListEditorViewAction" );
	dwPathListEditor->setWidget( new pPathListEditor( tr( "Edit paths" ), ".", this ) );
	dockToolBar( Qt::LeftToolBarArea )->addDock( dwPathListEditor, tr( "Path List Editor" ), QIcon( ":/fresh/country-flags/us.png" ) );
	
	/*for ( int i = 0; i < count; i++ ) {
		QDockWidget* dw = new QDockWidget( this );
		dw->setObjectName( QString( "Dock%1" ).arg( i ) );
		dw->toggleViewAction()->setObjectName( QString( "DockViewAction%1" ).arg( i ) );
		dockToolBar( Qt::LeftToolBarArea )->addDock( dw, QString( "Dock %1" ).arg( i ), QIcon( ":/fresh/country-flags/ad.png" ) );
	}*/
	
	mMenuBar->addAction( "file/edit/undo", "undo" );
	mMenuBar->addAction( "file/edit/redo", "redo" );
	mMenuBar->addAction( "move/again", "again" );
	
	foreach ( pDockToolBar* dockToolBar, dockToolBarManager()->dockToolBars() ) {
		QAction* action;
		
		action = dockToolBar->toggleViewAction();
		mMenuBar->addAction( QString( "view/docksToolBars/%1" ).arg( action->objectName() ), action );
		
		action = dockToolBar->toggleExclusiveAction();
		mMenuBar->addAction( QString( "view/docksToolBars/%1" ).arg( action->objectName() ), action );
	}
	
	foreach ( QDockWidget* dockWidget, findChildren<QDockWidget*>() ) {
		QAction* action = dockWidget->toggleViewAction();
		mMenuBar->addAction( QString( "view/docks/%1" ).arg( action->objectName() ), action );
		pActionsNode node = mActionsModel->actionToNode( action );
		node.setDefaultShortcut( QKeySequence( QString( "Ctrl+%1" ).arg( QChar( qMax( 32, qrand() % 127 ) ) ) ) );
	}
}

MainWindow::~MainWindow()
{
}

void MainWindow::pbAddAction_clicked()
{
	const QModelIndex index = tvActions->selectionModel()->selectedIndexes().value( 0 );
	QString path;
	
	if ( index.isValid() ) {
		const pActionsNode node = mActionsModel->indexToNode( index );
		
		if ( node.type() == pActionsNode::Action ) {
			path = node.path().section( '/', 0, -2 );
		}
		else {
			path = node.path();
		}
	}
	
	if ( !path.isEmpty() ) {
		path.append( '/' );
	}
	
	path = QInputDialog::getText( this, QString::null, tr( "Enter the full path where to add the action (/some/path/to/add/the/actionName):" ), QLineEdit::Normal, path );
	
	if ( path.count( "/" ) == 0 || QString( path ).replace( "/", QString::null ).trimmed().isEmpty() ) {
		return;
	}
	
	QAction* action = new QAction( this );
	action->setText( path.section( '/', -1, -1 ) );
	
	if ( !mActionsModel->addAction( path, action ) ) {
		delete action;
		QMessageBox::information( this, QString::null, tr( "Can't add action to '%1'" ).arg( path ) );
	}
}

void MainWindow::pbRemoveAction_clicked()
{
	const QModelIndex index = tvActions->selectionModel()->selectedIndexes().value( 0 );
	
	if ( index.isValid() ) {
		const pActionsNode node = mActionsModel->indexToNode( index );
		
		if ( !mActionsModel->removeAction( node.path() ) ) {
			QMessageBox::information( this, QString::null, tr( "Can't remove action '%1'" ).arg( node.path() ) );
		}
	}
}

void MainWindow::pbEditTextNode_clicked()
{
	const QModelIndex index = tvActions->selectionModel()->selectedIndexes().value( 0 );
	
	if ( index.isValid() ) {
		pActionsNode node = mActionsModel->indexToNode( index );
		const QString text = QInputDialog::getText( this, QString::null, tr( "Enter the new node text:" ), QLineEdit::Normal, node.text() );
		
		if ( !text.isEmpty() ) {
			node.setText( text );
		}
	}
}

void MainWindow::pbEditShortcuts_clicked()
{
	pActionsNodeShortcutEditor dlg( mActionsModel, this );
	dlg.exec();
}
