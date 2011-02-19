/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNodeModel.cpp
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
#include "pActionsNodeModel.h"

#include <QDir>

int pActionsNodeModel::mColumnCount = 3;

pActionsNodeModel::pActionsNodeModel( QObject* parent )
	: QAbstractItemModel( parent )
{
	mNodes[ QString::null ] = &mRootNode;
}

int pActionsNodeModel::columnCount( const QModelIndex& parent ) const
{
	Q_UNUSED( parent );
	return mColumnCount;
}

QVariant pActionsNodeModel::data( const QModelIndex& index, int role ) const
{
	const pActionsNode* node = nodePointer( index );
	
	if ( !node || node == &mRootNode ) {
		return QVariant();
	}
	
	switch ( role )
	{
		case Qt::DecorationRole:
			switch ( index.column() ) {
				case 0:
					return node->icon();
				default:
					break;
			}
			
			break;
		case Qt::DisplayRole:
		case Qt::ToolTipRole:
			switch ( index.column() ) {
				case pActionsNodeModel::Action: {
					const QString sep = "**MkS-Separator**";
					return node->text()
						.replace( "&&", sep )
						.remove( "&" )
						.replace( sep, "&&" );
				}
				case pActionsNodeModel::Shortcut:
					return node->shortcut().toString( QKeySequence::NativeText );
				case pActionsNodeModel::DefaultShortcut:
					return node->defaultShortcut().toString( QKeySequence::NativeText );
			}
			
			break;
		
		case Qt::FontRole: {
			QFont font;
			
			if ( node->type() == pActionsNode::Path ) {
				font.setBold( true );
			}
			
			return font;
		}
		
		/*case Qt::BackgroundRole:
			return node->type() == pActionsNode::Path ? QBrush( QColor( 0, 0, 255, 20 ) ) : QVariant();*/
	}
	
	return QVariant();
}

QModelIndex pActionsNodeModel::index( int row, int column, const QModelIndex& parent ) const
{
	const pActionsNode* node = nodePointer( parent );
	
	if ( !node
		|| row < 0 || row >= node->d->children.count()
		|| column < 0 || column >= mColumnCount
		|| ( parent.column() != 0 && parent.isValid() ) ) {
		return QModelIndex();
	}
	
	return createIndex( row, column, &node->d->children[ row ] );
}

QModelIndex pActionsNodeModel::parent( const QModelIndex& index ) const
{
	const pActionsNode* node = nodePointer( index );
	
	if ( !node || node == &mRootNode || node->d->parent == &mRootNode ) {
		return QModelIndex();
	}
	
	pActionsNode* parentNode = node->d->parent;
	const pActionsNode* parentParentNode = parentNode ? parentNode->d->parent : 0;
	const int row = parentParentNode ? parentParentNode->d->children.indexOf( *parentNode ) : 0;
	
	return createIndex( row, 0, parentNode );
}

int pActionsNodeModel::rowCount( const QModelIndex& parent ) const
{
	const pActionsNode* node = nodePointer( parent );
	
	return node && ( parent.column() == 0 || parent == QModelIndex() )
		? node->d->children.count()
		: 0;
}

bool pActionsNodeModel::hasChildren( const QModelIndex& parent ) const
{
	const pActionsNode* node = nodePointer( parent );
	
	return node && ( parent.column() == 0 || parent == QModelIndex() )
		? !node->d->children.isEmpty()
		: false;
}

QVariant pActionsNodeModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
	switch ( orientation ) {
		case Qt::Horizontal: {
			if ( role == Qt::DisplayRole ) {
				switch ( section ) {
					case pActionsNodeModel::Action:
						return tr( "Action" );
					case pActionsNodeModel::Shortcut:
						return tr( "Shortcut" );
					case pActionsNodeModel::DefaultShortcut:
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

bool pActionsNodeModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
	pActionsNode* node = nodePointer( index );
	
	if ( !node || node == &mRootNode ) {
		return false;
	}
	
	switch ( role ) {
		case Qt::DecorationRole: {
			switch ( node->d->type ) {
				case pActionsNode::Action:
					if ( node->d->action ) {
						node->d->action.data()->setIcon( value.value<QIcon>() );
					}
					
					break;
				case pActionsNode::Path:
					node->d->icon = value.value<QIcon>();
					break;
				default:
					break;
			}
			
			break;
		}
		case Qt::DisplayRole: {
			switch ( node->d->type ) {
				case pActionsNode::Action:
					if ( node->d->action ) {
						node->d->action.data()->setText( value.toString() );
					}
					
					break;
				case pActionsNode::Path:
					node->d->text = value.toString();
					break;
				default:
					break;
			}
			
			break;
		}
		case pActionsNode::ShortcutRole: {
			if ( node->d->type == pActionsNode::Action ) {
				if ( node->d->action ) {
					const QKeySequence shortcut = value.value<QKeySequence>();
					
					// check all actions for duplicate shortcuts
					foreach ( pActionsNode* childNode, mNodes ) {
						if ( childNode->action() && childNode->action() != node->action() ) {
							if ( childNode->action()->shortcut() == shortcut && !shortcut.isEmpty() ) {
								return false;
							}
						}
					}
					
					// set shortcut
					node->d->action.data()->setShortcut( shortcut );
				}
			}
			
			break;
		}
		case pActionsNode::DefaultShortcutRole: {
			if ( node->d->type == pActionsNode::Action ) {
				if ( node->d->action ) {
					node->d->action.data()->setData( value );
				}
			}
			
			break;
		}
		default:
			return QAbstractItemModel::setData( index, value, role );
	}
	
	emit dataChanged( index.sibling( index.row(), 0 ), index.sibling( index.row(), mColumnCount -1 ) );
	emit nodeChanged( *node );
	
	return true;
}

pActionsNode pActionsNodeModel::rootNode() const
{
	return mRootNode;
}

pActionsNode pActionsNodeModel::indexToNode( const QModelIndex& index ) const
{
	const pActionsNode* node = nodePointer( index );
	return node ? *node : pActionsNode();
}

pActionsNode pActionsNodeModel::pathToNode( const QString& _path ) const
{
	const QString path = fixedPath( _path );
	const pActionsNode* node = mNodes.value( path );
	return node ? *node : pActionsNode();
}

pActionsNode pActionsNodeModel::actionToNode( QAction* action ) const
{
	return pathToNode( action ? action->objectName() : QString::null );
}

QModelIndex pActionsNodeModel::nodeToIndex( const pActionsNode& node ) const
{
	if ( !mNodes.contains( node.d->path ) || node == mRootNode ) {
		return QModelIndex();
	}
	
	const pActionsNode* parentNode = node.d->parent;
	const int row = parentNode ? parentNode->d->children.indexOf( node ) : 0;
	
	return createIndex( row, 0, mNodes[ node.path() ] );
}

QModelIndex pActionsNodeModel::pathToIndex( const QString& _path ) const
{
	const QString path = fixedPath( _path );
	const pActionsNode* node = mNodes.value( path );
	return nodeToIndex( node ? *node : pActionsNode() );
}

QModelIndex pActionsNodeModel::actionToIndex( QAction* action ) const
{
	return pathToIndex( action ? action->objectName() : QString::null );
}

void pActionsNodeModel::clear()
{
	const int count = mRootNode.d->children.count();
	
	if ( count == 0 ) {
		return;
	}
	
	beginRemoveRows( QModelIndex(), 0, count -1 );
	mRootNode.d->children.clear();
	endRemoveRows();
	
	emit nodesCleared();
}

bool pActionsNodeModel::addAction( const QString& _path, QAction* action )
{
	Q_ASSERT( action );
	
	if ( !action ) {
		return false;
	}
	
	const QString path = fixedPath( _path );
	const QString subPath = path.section( '/', 0, -2 );
	pActionsNode* parentNode = createCompletePathNode( subPath );
	
	if ( !parentNode ) {
		return false;
	}
	
	const int count = parentNode->d->children.count();
	pActionsNode* node = 0;
	
	connect( action, SIGNAL( destroyed( QObject* ) ), this, SLOT( actionDestroyed( QObject* ) ) );
	
	beginInsertRows( nodeToIndex( *parentNode ), count, count );
	parentNode->d->children << pActionsNode::actionNode( path, action );
	node = &parentNode->d->children.last();
	node->d->model = this;
	node->d->parent = parentNode;
	mNodes[ path ] = node;
	action->setParent( this );
	endInsertRows();
	
	emit nodeInserted( *node );
	
	return true;
}

QAction* pActionsNodeModel::addAction( const QString& path, const QString& text, const QIcon& icon )
{
	QAction* action = new QAction( icon, text, this );
	
	if ( !addAction( path, action ) ) {
		action->deleteLater();
		return 0;
	}
	
	return action;
}

pActionsNode pActionsNodeModel::addPath( const QString& path )
{
	pActionsNode* node = createCompletePathNode( path );
	return node ? *node : pActionsNode();
}

bool pActionsNodeModel::removeAction( const QString& _path, bool removeEmptyPath )
{
	return removePath( _path, removeEmptyPath );
}

bool pActionsNodeModel::removeAction( QAction* action, bool removeEmptyPath )
{
	return removeAction( action->objectName(), removeEmptyPath );
}

bool pActionsNodeModel::removePath( const QString& _path, bool removeEmptyPath )
{
	const QString path = fixedPath( _path );
	const pActionsNode* node = mNodes.value( path );
	
	if ( !node ) {
		return false;
	}
	
	pActionsNode* parentNode = node->d->parent;
	const int row = parentNode->d->children.indexOf( *node );
	const pActionsNode tmpNode = *node;
	
	beginRemoveRows( nodeToIndex( *parentNode ), row, row );
	parentNode->d->children.removeAt( row );
	endRemoveRows();
	
	emit nodeRemoved( tmpNode );
	
	if ( removeEmptyPath ) {
		removeCompleteEmptyPathNode( parentNode );
	}
	
	return true;
}

bool pActionsNodeModel::isValid( const QModelIndex& index ) const
{
	if ( !index.isValid() || index.row() < 0 || index.column() < 0 || index.column() >= mColumnCount ) {
		return false;
	}
	
	const pActionsNode* node = static_cast<pActionsNode*>( index.internalPointer() );
	const pActionsNode* parentNode = node ? node->d->parent : 0;
	
	if ( node == &mRootNode ) {
		return index.row() == 0;
	}
	
	if ( !node || !parentNode ) {
		return false;
	}
	
	if ( index.row() >= parentNode->d->children.count() ) {
		return false;
	}
	
	return true;
}

pActionsNode* pActionsNodeModel::nodePointer( const QModelIndex& index ) const
{
	if ( index == QModelIndex() ) {
		return const_cast<pActionsNode*>( &mRootNode );
	}
	
	if ( !isValid( index ) ) {
		return 0;
	}
	
	return static_cast<pActionsNode*>( index.internalPointer() );
}

pActionsNode* pActionsNodeModel::createCompletePathNode( const QString& path )
{
	pActionsNode* node = mNodes.value( path );
	
	if ( node ) {
		return node->d->type == pActionsNode::Path ? node : 0;
	}
	
	const int separatorCount = path.count( "/" ) +1;
	pActionsNode* parentNode = 0;
	QString subPath;
	
	for ( int i = 0; i < separatorCount; i++ ) {
		subPath = path.section( '/', 0, i );
		node = mNodes.value( subPath );
		
		if ( node ) {
			if ( path != subPath ) {
				continue;
			}
			
			return node->d->type == pActionsNode::Path ? node : 0;
		}
		
		parentNode = mNodes.value( i == 0 ? QString::null : path.section( '/', 0, i -1 ) );
		const int count = parentNode->d->children.count();
	
		beginInsertRows( nodeToIndex( *parentNode ), count, count );
		parentNode->d->children << pActionsNode::pathNode( subPath );
		node = &parentNode->d->children.last();
		node->d->model = this;
		node->d->parent = parentNode;
		mNodes[ subPath ] = node;
		endInsertRows();
		
		emit nodeInserted( *node );
	}
	
	return node;
}

void pActionsNodeModel::removeCompleteEmptyPathNode( pActionsNode* node )
{
	if ( node == &mRootNode ) {
		return;
	}
	
	if ( node->d->children.isEmpty() ) {
		pActionsNode* parentNode = node->d->parent;
		const int row = parentNode->d->children.indexOf( *node );
		const pActionsNode tmpNode = *node;
		
		beginRemoveRows( nodeToIndex( *parentNode ), row, row );
		parentNode->d->children.removeAt( row );
		endRemoveRows();
		
		emit nodeRemoved( tmpNode );
		
		removeCompleteEmptyPathNode( parentNode );
	}
}

QString pActionsNodeModel::fixedPath( const QString& path )
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

void pActionsNodeModel::actionDestroyed( QObject* object )
{
	const QString path = object->objectName();
	removeAction( path );
}
