/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pStringListEditor.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pStringListEditor.h"
#include "pIconManager.h"

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

pStringListEditor::pStringListEditor( QWidget* parent )
    : QGroupBox( parent )
{
    init();
}

pStringListEditor::pStringListEditor( const QString& title, QWidget* parent )
    : QGroupBox( title, parent )
{
    init();
}

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

QModelIndex pStringListEditor::selectedIndex() const
{
    return lvValues->selectionModel()->selectedIndexes().value( 0 );
}

QModelIndex pStringListEditor::indexForValue( const QString& value ) const
{
    const Qt::MatchFlags flags = Qt::MatchFixedString | Qt::MatchCaseSensitive | Qt::MatchRecursive;
    return slmValues->match( indexForRow( 0 ), Qt::DisplayRole, value, 1, flags ).value( 0 );
}

QModelIndex pStringListEditor::indexForRow( int row ) const
{
    return slmValues->index( row, 0 );
}

int pStringListEditor::rowForValue( const QString& value ) const
{
    return indexForValue( value ).row();
}

void pStringListEditor::setValues( const QStringList& values )
{
    slmValues->setStringList( values );
    emit modified();
}

QStringList pStringListEditor::values() const
{
    return slmValues->stringList();
}

void pStringListEditor::insert( int index, const QString& value )
{
    const int row = slmValues->rowCount();
    
    if ( index < 0 ) {
        index = 0;
    }
    
    if ( index > row ) {
        index = row;
    }
    
    if ( slmValues->insertRow( row ) ) {
        const QModelIndex index = slmValues->index( row, 0 );
        
        slmValues->setData( index, value, Qt::DisplayRole );
        lvValues->setCurrentIndex( index );
        lvValues->scrollTo( index );
        lvValues->edit( index );
    }
}

void pStringListEditor::append( const QString& value )
{
    insert( slmValues->rowCount(), value );
}

void pStringListEditor::selectRow( int row )
{
    lvValues->setCurrentIndex( indexForRow( row ) );
}

int pStringListEditor::selectedRow() const
{
    return selectedIndex().row();
}

void pStringListEditor::selectValue( const QString& value )
{
    lvValues->setCurrentIndex( indexForValue( value ) );
}

QString pStringListEditor::selectedValue() const
{
    return selectedIndex().data( Qt::DisplayRole ).toString();
}

void pStringListEditor::setValue( const QString& value )
{
    const QModelIndex index = selectedIndex();
    slmValues->setData( index, value, Qt::DisplayRole );
}

void pStringListEditor::editValue( const QString& value )
{
    lvValues->edit( indexForValue( value ) );
}

void pStringListEditor::editValue( int row )
{
    lvValues->edit( indexForRow( row ) );
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
    append( tr( "New item %1" ).arg( row ) );
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
