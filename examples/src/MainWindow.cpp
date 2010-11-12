#include "MainWindow.h"

#include <Core/pSettings>
#include <Core/pVersion>
#include <Gui/pDockToolBarManager>
#include <Gui/pDockToolBar>
#include <Gui/pActionsNodeModel>
#include <Gui/pActionsNodeShortcutEditor>
#include <Gui/pActionsNodeMenuBar>
#include <Gui/pStringListEditor>
#include <Gui/pFileListEditor>
#include <Gui/pPathListEditor>
#include <Gui/pColorButton>

#if defined( QT_MODELTEST )
#include <modeltest.h>
#endif

#include <QtGui>

MainWindow::MainWindow( QWidget* parent )
	: pMainWindow( parent )
{
	initializeGui();
}

MainWindow::~MainWindow()
{
}

QPixmap MainWindow::scaledPixmap( const QString& filePath, const QSize& size ) const
{
	const QString key = QString( "%1-%2-%3" ).arg( filePath ).arg( size.width() ).arg( size.height() );
	QPixmap pixmap;
	
	if ( !QPixmapCache::find( key, pixmap ) ) {
		if ( pixmap.load( filePath ) ) {
			
			if ( size != QSize() ) {
				pixmap = pixmap.scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation );
			}
			
			if ( !QPixmapCache::insert( key, pixmap ) ) {
				qWarning() << Q_FUNC_INFO << "Can't cache pixmap" << filePath;
			}
		}
		else {
			qWarning() << Q_FUNC_INFO << "Pixmap not exists" << filePath;
		}
	}
	
	return pixmap;
}

void MainWindow::initializeGui()
{
	twPages = new QTabWidget( this );
	setCentralWidget( twPages );
	
	initializeMenuBar();
	pteLog = initializePlainTextEdit();
	tvActions = initializeActionsTreeView();
	
	menuBar()->addAction( "mFile/mSession/aRestore", tr( "restore" ) );
	
	
	
	return;
	
	// **********************

	
	// list editor dock
	/*QDockWidget* dwListEditor = new QDockWidget( this );
	QTabWidget* twListEditors = new QTabWidget( dwListEditor );
	dwListEditor->setObjectName( "DockListEditor" );
	dwListEditor->setWidget( twListEditors );
	dwListEditor->toggleViewAction()->setObjectName( "DockListEditorViewAction" );
	dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dwListEditor, tr( "List Editor" ), QIcon( scaledPixmap( ":/fresh/country-flags/fr.png", QSize( 96, 96 ) ) ) );
	
	twListEditors->addTab( new pStringListEditor( QString::null, this ), tr( "Edit strings" ) );
	twListEditors->addTab( new pPathListEditor( QString::null, ".", this ), tr( "Edit paths" ) );
	twListEditors->addTab( new pFileListEditor( QString::null, ".", "*.png", this ), tr( "Edit files" ) );
	
	// custom widgets
	QDockWidget* dwWidgets = new QDockWidget( this );
	dwWidgets->setObjectName( "dwWidgets" );
	dwWidgets->toggleViewAction()->setObjectName( "dwWidgetsViewAction" );
	dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dwWidgets, tr( "Testing widgets" ), QIcon( scaledPixmap( ":/fresh/country-flags/es.png", QSize( 96, 96 ) ) ) );
	
	QWidget* dwWidgetsContents = new QWidget( this );
	QGridLayout* dwWidgetsContentsLayout = new QGridLayout( dwWidgetsContents );
	dwWidgets->setWidget( dwWidgetsContents );
	
	pColorButton* colorButton = new pColorButton( dwWidgetsContents );
	dwWidgetsContentsLayout->addWidget( colorButton, 0, 0 );
	
	const pVersion v1( "1.5.4" );
	const pVersion v2( "1.5.4a" );
	QLabel* lVersion = new QLabel( dwWidgetsContents );
	lVersion->setText( QString(
		"%1 > %2: %3\n"
		"%4 < %5: %6\n"
		"%7 >= %8: %9\n"
		"%10 <= %11: %12\n"
		"%13 == %14: %15\n"
		"%16 != %17: %18"
		)
		.arg( v1.toString() ).arg( v2.toString() ).arg( v1 > v2 )
		.arg( v1.toString() ).arg( v2.toString() ).arg( v1 < v2 )
		.arg( v1.toString() ).arg( v2.toString() ).arg( v1 >= v2 )
		.arg( v1.toString() ).arg( v2.toString() ).arg( v1 <= v2 )
		.arg( v1.toString() ).arg( v2.toString() ).arg( v1 == v2 )
		.arg( v1.toString() ).arg( v2.toString() ).arg( v1 != v2 )
	);
	dwWidgetsContentsLayout->addWidget( lVersion, 0, 1 );
	
	QLabel* lVersion2 = new QLabel( dwWidgetsContents );
	lVersion2->setText( QString(
		"%1 > %2: %3\n"
		"%4 < %5: %6\n"
		"%7 >= %8: %9\n"
		"%10 <= %11: %12\n"
		"%13 == %14: %15\n"
		"%16 != %17: %18"
		)
		.arg( v1.toString() ).arg( v1.toString() ).arg( v1 > v1 )
		.arg( v1.toString() ).arg( v1.toString() ).arg( v1 < v1 )
		.arg( v1.toString() ).arg( v1.toString() ).arg( v1 >= v1 )
		.arg( v1.toString() ).arg( v1.toString() ).arg( v1 <= v1 )
		.arg( v1.toString() ).arg( v1.toString() ).arg( v1 == v1 )
		.arg( v1.toString() ).arg( v1.toString() ).arg( v1 != v1 )
	);
	dwWidgetsContentsLayout->addWidget( lVersion2, 0, 2 );*/
	
	/*for ( int i = 0; i < 10; i++ ) {
		QDockWidget* dw = new QDockWidget( this );
		dw->setObjectName( QString( "Dock%1" ).arg( i ) );
		dw->toggleViewAction()->setObjectName( QString( "DockViewAction%1" ).arg( i ) );
		dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dw, QString( "Dock %1" ).arg( i ), QIcon( scaledPixmap( ":/fresh/country-flags/ad.png", QSize( 96, 96 ) ) ) );
	}*/
	
	/*mMenuBar->addAction( "file/edit/undo", "undo" );
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
	}*/
}

void MainWindow::initializeMenuBar()
{
	// set menu bar model
	mActionsModel = new pActionsNodeModel( this );
	menuBar()->setModel( mActionsModel );
	
#if defined( QT_MODELTEST )
	new ModelTest( mActionsModel, this );
#endif
	
	// create menus and sub menus
	menuBar()->addMenu( "mFile" ).setText( tr( "&File" ) );
	menuBar()->addMenu( "mEdit" ).setText( tr( "&Edit" ) );
	menuBar()->addMenu( "mView" ).setText( tr( "&View" ) );
	menuBar()->addMenu( "mView/mMode" ).setText( tr( "&Mode" ) );
	
	// create actions
	QAction* aQuit = menuBar()->addAction( "mFile/aQuit", tr( "&Quit" ) );
	
	QAction* aClassic = menuBar()->addAction( "mView/mMode/aShowClassic", tr( "Classic" ) );
	aClassic->setCheckable( true );
	
	QAction* aModern = menuBar()->addAction( "mView/mMode/aShowModern", tr( "Modern" ) );
	aModern->setCheckable( true );
	
	// action group
	QActionGroup* agDockToolBarManagerMode = new QActionGroup( this );
	agDockToolBarManagerMode->addAction( aClassic );
	agDockToolBarManagerMode->addAction( aModern );
	
	// connections
	connect( aQuit, SIGNAL( triggered() ), this, SLOT( close() ) );
	connect( aClassic, SIGNAL( triggered() ), this, SLOT( dockToolBarManagerClassic() ) );
	connect( aModern, SIGNAL( triggered() ), this, SLOT( dockToolBarManagerModern() ) );
}

QPlainTextEdit* MainWindow::initializePlainTextEdit()
{
	QPlainTextEdit* pte = new QPlainTextEdit( this );
	pte->setReadOnly( true );
	pte->setTabStopWidth( 40 );
	twPages->addTab( pte, tr( "Log" ) );
	return pte;
}

QTreeView* MainWindow::initializeActionsTreeView()
{
	QTreeView* tv = new QTreeView( this );
	tv->setModel( menuBar()->model() );
	
	menuBar()->addMenu( "mEdit/mActions" ).setText( tr( "&Actions" ) );
	
	QAction* aAddAction = menuBar()->addAction( "mEdit/mActions/aAddAction", tr( "&Add new action" ) );
	QAction* aRemoveAction = menuBar()->addAction( "mEdit/mActions/aRemoveAction", tr( "&Remove selected action" ) );
	QAction* aEditTextNode = menuBar()->addAction( "mEdit/mActions/aEditTextNode", tr( "&Edit selected node text" ) );
	QAction* aEditShortcuts = menuBar()->addAction( "mEdit/mActions/aEditShortcuts", tr( "Edit the actions &shortcuts" ) );
	
	connect( aAddAction, SIGNAL( triggered() ), this, SLOT( aAddAction_triggered() ) );
	connect( aRemoveAction, SIGNAL( triggered() ), this, SLOT( aRemoveAction_triggered() ) );
	connect( aEditTextNode, SIGNAL( triggered() ), this, SLOT( aEditTextNode_triggered() ) );
	connect( aEditShortcuts, SIGNAL( triggered() ), this, SLOT( aEditShortcuts_triggered() ) );
	
	twPages->addTab( tv, tr( "Actions" ) );
	
	return tv;
}

void MainWindow::aAddAction_triggered()
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

void MainWindow::aRemoveAction_triggered()
{
	const QModelIndex index = tvActions->selectionModel()->selectedIndexes().value( 0 );
	
	if ( index.isValid() ) {
		const pActionsNode node = mActionsModel->indexToNode( index );
		
		if ( !mActionsModel->removeAction( node.path() ) ) {
			QMessageBox::information( this, QString::null, tr( "Can't remove action '%1'" ).arg( node.path() ) );
		}
	}
}

void MainWindow::aEditTextNode_triggered()
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

void MainWindow::aEditShortcuts_triggered()
{
	pActionsNodeShortcutEditor dlg( mActionsModel, this );
	dlg.exec();
}

void MainWindow::dockToolBarManagerClassic()
{
	dockToolBarManager()->setMode( pDockToolBarManager::Classic );
}

void MainWindow::dockToolBarManagerModern()
{
	dockToolBarManager()->setMode( pDockToolBarManager::Modern );
}
