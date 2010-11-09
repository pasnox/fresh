#include "MainWindow.h"

#include <Core/pSettings>
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
	QWidget* centralWidget = new QWidget( this );
	QVBoxLayout* hl = new QVBoxLayout( centralWidget );
	
	mActionsModel = new pActionsNodeModel( this );
	mMenuBar = menuBar();
	mMenuBar->setModel( mActionsModel );
	
	QAction* aClassic = mMenuBar->addAction( "view/mode/aShowClassic", tr( "Classic Mode" ) );
	aClassic->setCheckable( true );
	
	QAction* aModern = mMenuBar->addAction( "view/mode/aShowModern", tr( "Modern Mode" ) );
	aModern->setCheckable( true );
	
	QActionGroup* agDockToolBarManagerMode = new QActionGroup( this );
	agDockToolBarManagerMode->addAction( aClassic );
	agDockToolBarManagerMode->addAction( aModern );
	
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
	connect( aClassic, SIGNAL( triggered() ), this, SLOT( dockToolBarManagerClassic() ) );
	connect( aModern, SIGNAL( triggered() ), this, SLOT( dockToolBarManagerModern() ) );
	connect( pbAddAction, SIGNAL( clicked() ), this, SLOT( pbAddAction_clicked() ) );
	connect( pbRemoveAction, SIGNAL( clicked() ), this, SLOT( pbRemoveAction_clicked() ) );
	connect( pbEditTextNode, SIGNAL( clicked() ), this, SLOT( pbEditTextNode_clicked() ) );
	connect( pbEditShortcuts, SIGNAL( clicked() ), this, SLOT( pbEditShortcuts_clicked() ) );
	
	// **********************
#if defined( QT_MODELTEST )
	new ModelTest( mActionsModel, this );
#endif
	
	// string list editor
	QDockWidget* dwStringListEditor = new QDockWidget( this );
	dwStringListEditor->setObjectName( "DockStringListEditor" );
	dwStringListEditor->toggleViewAction()->setObjectName( "DockStringListEditorViewAction" );
	dwStringListEditor->setWidget( new pStringListEditor( tr( "Edit strings" ), this ) );
	dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dwStringListEditor, tr( "String List Editor" ), QIcon( scaledPixmap( ":/fresh/country-flags/fr.png", QSize( 96, 96 ) ) ) );
	
	// file list editor
	QDockWidget* dwFileListEditor = new QDockWidget( this );
	dwFileListEditor->setObjectName( "DockFileListEditor" );
	dwFileListEditor->toggleViewAction()->setObjectName( "DockFileListEditorViewAction" );
	dwFileListEditor->setWidget( new pFileListEditor( tr( "Edit files" ), ".", "*.png", this ) );
	dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dwFileListEditor, tr( "File List Editor" ), QIcon( scaledPixmap( ":/fresh/country-flags/gb.png", QSize( 96, 96 ) ) ) );
	
	// path list editor
	QDockWidget* dwPathListEditor = new QDockWidget( this );
	dwPathListEditor->setObjectName( "DockPathListEditor" );
	dwPathListEditor->toggleViewAction()->setObjectName( "DockPathListEditorViewAction" );
	dwPathListEditor->setWidget( new pPathListEditor( tr( "Edit paths" ), ".", this ) );
	dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dwPathListEditor, tr( "Path List Editor" ), QIcon( scaledPixmap( ":/fresh/country-flags/us.png", QSize( 96, 96 ) ) ) );
	
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
	
	/*for ( int i = 0; i < 10; i++ ) {
		QDockWidget* dw = new QDockWidget( this );
		dw->setObjectName( QString( "Dock%1" ).arg( i ) );
		dw->toggleViewAction()->setObjectName( QString( "DockViewAction%1" ).arg( i ) );
		dockToolBar( Qt::LeftToolBarArea )->addDockWidget( dw, QString( "Dock %1" ).arg( i ), QIcon( scaledPixmap( ":/fresh/country-flags/ad.png", QSize( 96, 96 ) ) ) );
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

void MainWindow::dockToolBarManagerClassic()
{
	dockToolBarManager()->setMode( pDockToolBarManager::Classic );
}

void MainWindow::dockToolBarManagerModern()
{
	dockToolBarManager()->setMode( pDockToolBarManager::Modern );
}
