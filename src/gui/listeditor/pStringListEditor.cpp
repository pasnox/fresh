#include "pStringListEditor.h"
#include "gui/pIconManager.h"

#include <QToolBar>
#include <QAction>
#include <QListView>
#include <QItemSelectionModel>
#include <QStringListModel>
#include <QVBoxLayout>

#include <QDebug>

class pStringListModel : public QStringListModel
{
public:
	pStringListModel( QObject* parent = 0 )
		: QStringListModel( parent )
	{
	}
	
	pStringListModel( const QStringList& strings, QObject* parent = 0 )
		: QStringListModel( strings, parent )
	{
	}

	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const
	{
		switch ( role ) {
			case Qt::ToolTipRole:
				return QStringListModel::data( index, Qt::DisplayRole ).toString();
		}
		
		return QStringListModel::data( index, role );
	}
};

/*!
	\details Create a new pStringListEditor instance
	\param parent The parent widget
*/
pStringListEditor::pStringListEditor( QWidget* parent )
	: QGroupBox( parent )
{
	init();
}

/*!
	\details Create a new pStringListEditor instance
	\param title The editor title
	\param parent The parent widget
*/
pStringListEditor::pStringListEditor( const QString& title, QWidget* parent )
	: QGroupBox( title, parent )
{
	init();
}

/*!
	\details internal initialisation
*/
void pStringListEditor::init()
{
	// create toolbar
	tbActions = new QToolBar( this );
	tbActions->layout()->setMargin( 0 );
	tbActions->layout()->setSpacing( 0 );
	tbActions->setIconSize( QSize( 16, 16 ) );
	
	// create actions
	aAdd = new QAction( pIconManager::icon( "add.png", ":/fresh/icons" ), tr( "Add Item" ), tbActions );
	aEdit = new QAction( pIconManager::icon( "edit.png", ":/fresh/icons" ), tr( "Edit Item" ), tbActions );
	aRemove = new QAction( pIconManager::icon( "remove.png", ":/fresh/icons" ), tr( "Remove Item" ), tbActions );
	aClear = new QAction( pIconManager::icon( "clear.png", ":/fresh/icons" ), tr( "Clear Items" ), tbActions );
	aUp = new QAction( pIconManager::icon( "up.png", ":/fresh/icons" ), tr( "Move Item Up" ), tbActions );
	aDown = new QAction( pIconManager::icon( "down.png", ":/fresh/icons" ), tr( "Move Item Down" ), tbActions );
	
	// add actions to toolbar
	tbActions->addAction( aAdd );
	tbActions->addAction( aEdit );
	tbActions->addAction( aRemove );
	tbActions->addAction( aClear );
	tbActions->addAction( aUp );
	tbActions->addAction( aDown );

	// create listview
	lvValues = new QListView( this );
	lvValues->setMinimumHeight( 40 );
	lvValues->setUniformItemSizes( true );
	lvValues->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
	
	// create model
	slmValues = new pStringListModel( lvValues );
	lvValues->setModel( slmValues );
	
	// create layout
	vLayout = new QVBoxLayout( this );
	vLayout->setMargin( 5 );
	vLayout->setSpacing( 3 );
	vLayout->addWidget( tbActions );
	vLayout->addWidget( lvValues );
	
	onSelectionChanged();
	
	// connections
	connect( aAdd, SIGNAL( triggered() ), this, SLOT( onAddItem() ) );
	connect( aEdit, SIGNAL( triggered() ), this, SLOT( onEditItem() ) );
	connect( aRemove, SIGNAL( triggered() ), this, SLOT( onRemoveItem() ) );
	connect( aClear, SIGNAL( triggered() ), this, SLOT( onClearItems() ) );
	connect( aUp, SIGNAL( triggered() ), this, SLOT( onMoveUpItem() ) );
	connect( aDown, SIGNAL( triggered() ), this, SLOT( onMoveDownItem() ) );
	connect( lvValues->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( onSelectionChanged() ) );
	connect( slmValues, SIGNAL( dataChanged( const QModelIndex&, const QModelIndex& ) ), this, SIGNAL( modified() ) );
	connect( slmValues, SIGNAL( rowsInserted( const QModelIndex&, int, int ) ), this, SIGNAL( modified() ) );
	connect( slmValues, SIGNAL( rowsMoved( const QModelIndex&, int, int, const QModelIndex&, int ) ), this, SIGNAL( modified() ) );
	connect( slmValues, SIGNAL( rowsRemoved( const QModelIndex&, int, int ) ), this, SIGNAL( modified() ) );
	connect( this, SIGNAL( modified() ), this, SLOT( onSelectionChanged() ) );
}

/*!
	\details Return the selected index
*/
QModelIndex pStringListEditor::selectedIndex() const
{
	return lvValues->selectionModel()->selectedIndexes().value( 0 );
}

/*!
	\details Set the editor values
	\param values The string list to set as values
*/
void pStringListEditor::setValues( const QStringList& values )
{
	slmValues->setStringList( values );
	emit modified();
}

/*!
	\details Return the editor QStringList values
*/
QStringList pStringListEditor::values() const
{
	return slmValues->stringList();
}

/*!
	\details Append a value to the list and scroll to it
*/
void pStringListEditor::appendValue( const QString& value )
{
	const int row = slmValues->rowCount();
	
	if ( slmValues->insertRow( row ) ) {
		const QModelIndex index = slmValues->index( row, 0 );
		
		slmValues->setData( index, value, Qt::DisplayRole );
		lvValues->setCurrentIndex( index );
		lvValues->scrollTo( index );
		lvValues->edit( index );
	}
}

/*!
	\details Update the selected value
*/
void pStringListEditor::setSelectedIndexValue( const QString& value )
{
	const QModelIndex index = selectedIndex();
	slmValues->setData( index, value, Qt::DisplayRole );
}

void pStringListEditor::onSelectionChanged()
{
	const QModelIndex index = selectedIndex();
	const int count = slmValues->rowCount();
	
	aEdit->setEnabled( index.isValid() );
	aRemove->setEnabled( index.isValid() );
	aClear->setEnabled( count > 0 );
	aUp->setEnabled( index.isValid() && index.row() > 0 );
	aDown->setEnabled( index.isValid() && index.row() < count -1 );
}

void pStringListEditor::onAddItem()
{
	const int row = slmValues->rowCount();
	appendValue( tr( "New item %1" ).arg( row ) );
}

void pStringListEditor::onEditItem()
{
	lvValues->edit( selectedIndex() );
}

void pStringListEditor::onRemoveItem()
{
	slmValues->removeRow( selectedIndex().row() );
}

void pStringListEditor::onClearItems()
{
	slmValues->setStringList( QStringList() );
	onSelectionChanged();
}

void pStringListEditor::onMoveUpItem()
{
	const QModelIndex index = selectedIndex();
	const QModelIndex upIndex = index.sibling( index.row() -1, 0 );
	const QPair<QString, QString> values = qMakePair( index.data().toString(), upIndex.data().toString() );
	
	slmValues->setData( index, values.second, Qt::DisplayRole );
	slmValues->setData( upIndex, values.first, Qt::DisplayRole );
	
	lvValues->setCurrentIndex( upIndex );
}

void pStringListEditor::onMoveDownItem()
{
	const QModelIndex index = selectedIndex();
	const QModelIndex downIndex = index.sibling( index.row() +1, 0 );
	const QPair<QString, QString> values = qMakePair( index.data().toString(), downIndex.data().toString() );
	
	slmValues->setData( index, values.second, Qt::DisplayRole );
	slmValues->setData( downIndex, values.first, Qt::DisplayRole );
	
	lvValues->setCurrentIndex( downIndex );
}
