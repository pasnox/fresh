/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsModel.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
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
#include "pActionsModel.h"

#include <QMenu>
#include <QAction>
#include <QDir>
#include <QDebug>

#define DEFAULT_SHORTCUT_PROPERTY "Default Shortcut"

int pActionsModel::mColumnCount = 3;

pActionsModel::pActionsModel( QObject* parent )
	: QAbstractItemModel( parent )
{
}

pActionsModel::~pActionsModel()
{
	clear();
}

int pActionsModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return mColumnCount;
}

QVariant pActionsModel::data( const QModelIndex& index, int role ) const
{
	QAction* action = this->action( index );
	
	if ( !action ) {
		return QVariant();
	}
	
	switch ( role )
	{
		case Qt::DecorationRole:
			switch ( index.column() ) {
				case 0:
					return action->icon();
				default:
					break;
			}
			
			break;
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
			switch ( index.column() ) {
				case pActionsModel::Action: {
					const QString sep = "\001";
					return action->text()
						.replace( "&&", sep )
						.remove( "&" )
						.replace( sep, "&&" );
				}
				case pActionsModel::Shortcut:
					return action->shortcut().toString( QKeySequence::NativeText );
				case pActionsModel::DefaultShortcut:
					return defaultShortcut( action ).toString( QKeySequence::NativeText );
			}
			
			break;
		
		case Qt::FontRole: {
			QFont font = action->font();
			
			if ( action->menu() ) {
				font.setBold( true );
			}
			
			return font;
		}
		
		/*case Qt::BackgroundRole:
			return action->menu() ? QBrush( QColor( 0, 0, 255, 20 ) ) : QVariant();*/
	}
	
	return QVariant();
}

QModelIndex pActionsModel::index( int row, int column, const QModelIndex& parent ) const
{
	QAction* action = this->action( parent );
	QList<QAction*> actions = children( action );
	
	if ( row < 0 || row >= actions.count()
		|| column < 0 || column >= mColumnCount
		|| ( parent.column() != 0 && parent.isValid() ) ) {
		return QModelIndex();
	}
	
	return createIndex( row, column, actions.at( row ) );
}

QModelIndex pActionsModel::parent( const QModelIndex& index ) const
{
	QAction* action = this->action( index );
	
	if ( !action ) {
		return QModelIndex();
	}
	
	QAction* parentAction = parent( action );
	QAction* parentParentAction = parent( parentAction );
	const int row = parentParentAction ? children( parentParentAction ).indexOf( parentAction ) : 0;
	return createIndex( row, 0, parentAction );
}

int pActionsModel::rowCount( const QModelIndex& parent ) const
{
	QAction* action = this->action( parent );
	
	return ( parent.isValid() && parent.column() == 0 ) || parent == QModelIndex()
		? children( action ).count()
		: 0;
}

bool pActionsModel::hasChildren( const QModelIndex& parent ) const
{
	QAction* action = this->action( parent );
	
	return ( parent.isValid() && parent.column() == 0 ) || parent == QModelIndex()
		? hasChildren( action )
		: false;
}

QVariant pActionsModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	switch ( orientation ) {
		case Qt::Horizontal: {
			if ( role == Qt::DisplayRole || role == Qt::ToolTipRole ) {
				switch ( section ) {
					case pActionsModel::Action:
						return tr( "Action" );
					case pActionsModel::Shortcut:
						return tr( "Shortcut" );
					case pActionsModel::DefaultShortcut:
						return tr( "Default Shortcut" );
				}
			}
			
			break;
		}
		
		default:
			break;
	}
	
	return QAbstractItemModel::headerData( section, orientation, role );
}

QModelIndex pActionsModel::index( QAction* action, int column ) const
{
	const QString path = this->path( action );
	
	if ( !mActions.contains( path ) ) {
		return QModelIndex();
	}
	
	QAction* parentAction = parent( action );
	const int row = parentAction ? children( parentAction ).indexOf( action ) : 0;
	
	return createIndex( row, column, mActions[ path ] );
}

QAction* pActionsModel::action( const QModelIndex& index ) const
{
	return isValid( index ) ? static_cast<QAction*>( index.internalPointer() ) : 0;
}

QString pActionsModel::path( QAction* action ) const
{
	return mActions.key( action );
}

QAction* pActionsModel::action( const QString& path ) const
{
	return mActions.value( fixedPath( path ) );
}

void pActionsModel::clear()
{
	const int count = rowCount();
	
	if ( count == 0 ) {
		return;
	}
	
	beginRemoveRows( QModelIndex(), 0, count -1 );
	qDeleteAll( mChildren.value( 0 ) );
	mChildren.clear();
	mActions.clear();
	endRemoveRows();
	
	emit actionsCleared();
}

bool pActionsModel::addAction( const QString& _path, QAction* action )
{
	Q_ASSERT( action );
	
	if ( !action || !path( action ).isEmpty() ) {
		return false;
	}
	
	const QString path = fixedPath( _path );
	const QString subPath = path.section( '/', 0, -2 );
	QAction* parentAction = createCompletePathNode( subPath );
	
	if ( !parentAction ) {
		return false;
	}
	
	const int row = children( parentAction ).count();
	insertAction( path, action, parentAction, row );
	return true;
}

QAction* pActionsModel::addAction( const QString& path, const QString& text, const QIcon& icon )
{
	QAction* action = new QAction( icon, text, this );
	
	if ( !addAction( path, action ) ) {
		delete action;
		return 0;
	}
	
	return action;
}

QAction* pActionsModel::addMenu( const QString& path, const QString& text, const QIcon& icon )
{
	QAction* action = createCompletePathNode( path );
	
	if ( action ) {
		action->setIcon( icon );
		
		if ( !text.isEmpty() ) {
			action->setText( text );
		}
	}
	
	return action;
}

bool pActionsModel::removeAction( const QString& path, bool removeEmptyPath )
{
	return removeMenu( path, removeEmptyPath );
}

bool pActionsModel::removeAction( QAction* action, bool removeEmptyPath )
{
	return removeMenu( action, removeEmptyPath );
}

bool pActionsModel::removeMenu( const QString& path, bool removeEmptyPath )
{
	return removeMenu( action( path ), removeEmptyPath );
}

bool pActionsModel::removeMenu( QAction* action, bool removeEmptyPath )
{
	if ( !action ) {
		return false;
	}
	
	QAction* parentAction = parent( action );
	const int row = children( parentAction ).indexOf( action );
	
	removeAction( action, parentAction, row );
	
	if ( removeEmptyPath ) {
		removeCompleteEmptyPathNode( parentAction );
	}
	
	return true;
}

QAction* pActionsModel::parent( QAction* action ) const
{
	return mActions.value( path( action ).section( '/', 0, -2 ) );
}

bool pActionsModel::hasChildren( QAction* action ) const
{
	return !mChildren.value( action ).isEmpty();
}

QList<QAction*> pActionsModel::children( QAction* action ) const
{
	return mChildren.value( action );
}

QKeySequence pActionsModel::defaultShortcut( QAction* action ) const
{
	return action ? action->property( DEFAULT_SHORTCUT_PROPERTY ).value<QKeySequence>() : QKeySequence();
}

QKeySequence pActionsModel::defaultShortcut( const QString& path ) const
{
	return defaultShortcut( action( path ) );
}

void pActionsModel::setDefaultShortcut( QAction* action, const QKeySequence& shortcut )
{
	if ( action ) {
		action->setProperty( DEFAULT_SHORTCUT_PROPERTY, shortcut );
		setShortcut( action, shortcut );
	}
}

void pActionsModel::setDefaultShortcut( const QString& path, const QKeySequence& shortcut )
{
	setDefaultShortcut( action( path ), shortcut );
}

bool pActionsModel::setShortcut( QAction* action, const QKeySequence& shortcut, QString* error )
{
	action->setShortcut( shortcut );
	
	if ( error ) {
		*error = tr( "Can't set shortcut, it's already used by action '%1'." ).arg( "toto" );
	}
	
	return true;
}

bool pActionsModel::setShortcut( const QString& path, const QKeySequence& shortcut, QString* error )
{
	return setShortcut( action( path ), shortcut, error );
}

bool pActionsModel::isValid( const QModelIndex& index ) const
{
	if ( !index.isValid() || index.row() < 0 || index.column() < 0 || index.column() >= mColumnCount ) {
		return false;
	}
	
	QAction* action = static_cast<QAction*>( index.internalPointer() );
	QAction* parentAction = parent( action );
	
	if ( !action ) {
		return false;
	}
	
	if ( index.row() >= children( parentAction ).count() ) {
		return false;
	}
	
	return true;
}

void pActionsModel::insertAction( const QString& path, QAction* action, QAction* parent, int row )
{
	QObject* p = parent;
	
	if ( !p ) {
		p = this;
	}
	
	beginInsertRows( index( parent ), row, row );
	action->setObjectName( QString( path ).replace( "/", "_" ) );
	action->setParent( p );
	if ( parent ) {
		parent->menu()->addAction( action );
	}
	mChildren[ parent ] << action;
	mActions[ path ] = action;
	connect( action, SIGNAL( changed() ), this, SLOT( actionChanged() ) );
	connect( action, SIGNAL( destroyed( QObject* ) ), this, SLOT( actionDestroyed( QObject* ) ) );
	endInsertRows();
	
	emit actionInserted( action );
}

void pActionsModel::removeAction( QAction* action, QAction* parent, int row )
{
	beginRemoveRows( index( parent ), row, row );
	mChildren[ parent ].removeAt( row );
	mChildren.remove( action );
	mActions.remove( path( action ) );
	if ( parent ) {
		parent->menu()->removeAction( action );
	}
	endRemoveRows();
	
	emit actionRemoved( action );
	
	if ( action->menu() ) {
		action->menu()->deleteLater();
	}
	else {
		action->deleteLater();
	}
}

QAction* pActionsModel::createCompletePathNode( const QString& path )
{
	QAction* action = mActions.value( path );
	
	if ( action ) {
		return action->menu() ? action : 0;
	}
	
	const int separatorCount = path.count( "/" ) +1;
	QAction* parentAction = 0;
	QString subPath;
	
	for ( int i = 0; i < separatorCount; i++ ) {
		subPath = path.section( '/', 0, i );
		action = mActions.value( subPath );
		
		if ( action ) {
			if ( path != subPath ) {
				continue;
			}
			
			return action->menu() ? action : 0;
		}
		
		parentAction = mActions.value( i == 0 ? QString::null : path.section( '/', 0, i -1 ) );
		const int row = children( parentAction ).count();
		action = (new QMenu)->menuAction();
		action->setText( path.section( '/', i, i ) );
		insertAction( subPath, action, parentAction, row );
	}
	
	return action;
}

void pActionsModel::removeCompleteEmptyPathNode( QAction* action )
{
	if ( !action || !mActions.contains( path( action ) ) ) {
		return;
	}
	
	if ( !hasChildren( action ) ) {
		QAction* parentAction = parent( action );
		const int row = children( parentAction ).indexOf( action );
		
		removeAction( action, parentAction, row );
		removeCompleteEmptyPathNode( parentAction );
	}
}

QString pActionsModel::fixedPath( const QString& path )
{
	QString data = QDir::cleanPath( path )
		.replace( '\\', '/' )
		.remove( ' ' );
	
	while ( data.startsWith( '/' ) ) {
		data.remove( 0, 1 );
	}
	
	while ( data.endsWith( '/' ) ) {
		data.chop( 1 );
	}
	
	return data;
}

void pActionsModel::actionChanged()
{
	QAction* action = qobject_cast<QAction*>( sender() );
	
	if ( action ) {
		emit dataChanged( index( action, 0 ), index( action, mColumnCount -1 ) );
		emit actionChanged( action );
	}
}

void pActionsModel::actionDestroyed( QObject* object )
{
	removeAction( path( (QAction*)object ) );
}
