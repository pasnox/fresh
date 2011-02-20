/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNodeModel.h
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
#ifndef PACTIONSNODEMODEL_H
#define PACTIONSNODEMODEL_H

/*!
	\file pActionsNodeModel.h
	\brief A model for presenting menus and actions.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QAbstractItemModel>

#include "pActionsNode.h"

class QAction;

/*!
	\ingroup FreshGui
	\class pActionsNodeModel
	\brief A model for presenting menus and actions.
	
	That allow to have a menu bar that can be easily queried / handled by third party components like plugins.
*/

class pActionsNodeModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class pActionsNode::Data;
	friend class pActionsNodeMenuBar;
	
public:
	/*! This enum defines teh differents columns of the model. */
	enum Column {
		Action = 0, /*!< The action column. */
		Shortcut, /*!< The shortcut column. */
		DefaultShortcut /*!< The default shortcut. */
	};
	/*!
		Create a new pActionsNodeModel having parent \a parent.
	*/
	pActionsNodeModel( QObject* parent = 0 );
	/*!
		Reimplemented from QAbstractItemModel::columnCount().
	*/
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented from QAbstractItemModel::data().
	*/
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	/*!
		Reimplemented from QAbstractItemModel::index().
	*/
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented from QAbstractItemModel::parent().
	*/
	virtual QModelIndex parent( const QModelIndex& index ) const;
	/*!
		Reimplemented from QAbstractItemModel::rowCount().
	*/
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented from QAbstractItemModel::hasChildren().
	*/
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented from QAbstractItemModel::headerData().
	*/
	virtual	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	/*!
		Reimplemented from QAbstractItemModel::setData().
	*/
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	/*!
		return the root node of the model.
	*/
	pActionsNode rootNode() const;
	/*!
		Return the pActionsNode for the given \a index.
	*/
	pActionsNode indexToNode( const QModelIndex& index ) const;
	/*!
		Return the pActionsNode for the given \a path.
	*/
	pActionsNode pathToNode( const QString& path ) const;
	/*!
		Return the pActionsNode for the given \a action.
	*/
	pActionsNode actionToNode( QAction* action ) const;
	/*!
		Return the QModelIndex for the given \a node.
	*/
	QModelIndex nodeToIndex( const pActionsNode& node ) const;
	/*!
		Return the QModelIndex for the given \a path.
	*/
	QModelIndex pathToIndex( const QString& path ) const;
	/*!
		Return the QModelIndex for the given \a action.
	*/
	QModelIndex actionToIndex( QAction* action ) const;
	/*!
		Clear all nodes of the root node.
		
		The node's QAction are deleted.
	*/
	void clear();
	/*!
		Add the \a action to the model at the given \a path.
		
		Return true on success else false.
	*/
	bool addAction( const QString& path, QAction* action );
	/*!
		Add a new action in the model at the given \a path having text \a text and icon \a icon.
		
		Return the created QAction on success else 0.
	*/
	QAction* addAction( const QString& path, const QString& text, const QIcon& icon = QIcon() );
	/*!
		Add a path node at the given \a path.
	*/
	pActionsNode addPath( const QString& path );
	/*!
		Remove the action at the given \a path and delete the path hierarchy if it 
		contains no more actions and the \a removeEmptyPath parameter is true.
		
		Return true on success else false.
	*/
	bool removeAction( const QString& path, bool removeEmptyPath = false );
	/*!
		Remove the action \a action and delete the hierarchy if no more actions exists and the \a removeEmptyPath parameter is true.
		
		Return true on success else false.
	*/
	bool removeAction( QAction* action, bool removeEmptyPath = false );
	/*!
		Remove the path \a path and delete the hierarchy if it contains no more actions and if the \a removeEmptyPath is true.
		
		Return true on success else false.
	*/
	bool removePath( const QString& path, bool removeEmptyPath = false );

protected:
	static int mColumnCount;
	pActionsNode mRootNode;
	QHash<QString, pActionsNode*> mNodes;
	
	/*!
		Return true if the given \a index is valid else false.
	*/
	bool isValid( const QModelIndex& index ) const;
	/*!
		Return the pActionsNode pointer for the given \a index.
	*/
	pActionsNode* nodePointer( const QModelIndex& index ) const;
	/*!
		Create the complete hierarchy to \a path.
		
		Return the pActionsNode pointer of \a path on success else 0.
	*/
	pActionsNode* createCompletePathNode( const QString& path );
	/*!
		Remove a complete hierarchy path \a path.
	*/
	void removeCompleteEmptyPathNode( pActionsNode* node );
	/*!
		Return a fixed path of \a path.
	*/
	static QString fixedPath( const QString& path );

protected slots:
	/*!
		Delete the node of deleted QActions.
	*/
	void actionDestroyed( QObject* object );

signals:
	/*!
		This signal is emited when the node \a node is inserted.
	*/
	void nodeInserted( const pActionsNode& node );
	/*!
		This signal is emited when the node \a node is modified.
	*/
	void nodeChanged( const pActionsNode& node );
	/*!
		This signal is emited when the node \a node is removed.
	*/
	void nodeRemoved( const pActionsNode& node );
	/*!
		This signal is emited when the model is cleared.
	*/
	void nodesCleared();
};

#endif // PACTIONSNODEMODEL_H
