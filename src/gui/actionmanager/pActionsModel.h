/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsModel.h
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
#ifndef PACTIONSMODEL_H
#define PACTIONSMODEL_H

/*!
	\file pActionsModel.h
	\brief A model for presenting menus and actions.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QAbstractItemModel>
#include <QIcon>

class QAction;

/*!
	\ingroup FreshGui
	\class pActionsModel
	\brief A model for presenting menus and actions.
	
	That allow to have a menu bar that can be easily queried / handled by third party components like plugins.
*/
class pActionsModel : public QAbstractItemModel
{
	Q_OBJECT
	friend class pActionsNodeMenuBar;
	
public:
	/*! This enum defines the differents columns of the model. */
	enum Column {
		Action = 0, /*!< The action column. */
		Shortcut, /*!< The shortcut column. */
		DefaultShortcut /*!< The default shortcut. */
	};
	/*!
		Create a new pActionsModel having parent \a parent.
	*/
	pActionsModel( QObject* parent = 0 );
	/*!
		Destroys an instance of the model.
	*/
	virtual ~pActionsModel();
	/*!
		\reimp
	*/
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		\reimp
	*/
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	/*!
		\reimp
	*/
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	/*!
		\reimp
	*/
	virtual QModelIndex parent( const QModelIndex& index ) const;
	/*!
		\reimp
	*/
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		\reimp
	*/
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		\reimp
	*/
	virtual	QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	/*!
		Return the QModelIndex for the given \a action.
	*/
	QModelIndex index( QAction* action, int column = 0 ) const;
	/*!
		Return the action of the given \a index.
	*/
	QAction* action( const QModelIndex& index ) const;
	/*!
		Return the path of the given \a action.
	*/
	QString path( QAction* action ) const;
	/*!
		Return the action of the given \a path.
	*/
	QAction* action( const QString& path ) const;
	/*!
		Delete all actions and menus.
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
		Add a menu at the given \a path.
	*/
	QAction* addMenu( const QString& path, const QString& text, const QIcon& icon = QIcon() );
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
		Remove the menu at \a path and delete the hierarchy if it contains no more actions and if the \a removeEmptyPath is true.
		
		Return true on success else false.
	*/
	bool removeMenu( const QString& path, bool removeEmptyPath = false );
	/*!
		Remove the given menu \a action and delete the hierarchy if it contains no more actions and if the \a removeEmptyPath is true.
		
		Return true on success else false.
	*/
	bool removeMenu( QAction* action, bool removeEmptyPath = false );
	/*!
		Return the parent action of \a action.
	*/
	QAction* parent( QAction* action ) const;
	/*!
		Return true if \a action has children else false.
	*/
	bool hasChildren( QAction* action ) const;
	/*!
		Return a list of children actions for \a action.
	*/
	QList<QAction*> children( QAction* action ) const;
	/*!
		Return the default shortcut of \a action.
	*/
	QKeySequence defaultShortcut( QAction* action ) const;
	/*!
		Return the default shortcut of the action a the given \a path.
	*/
	QKeySequence defaultShortcut( const QString& path ) const;
	/*!
		Set the default shortcut of the given \a action to \a shortcut.
	*/
	void setDefaultShortcut( QAction* action, const QKeySequence& shortcut );
	/*!
		Set the default shortcut of the action at the given \a path to \a shortcut.
	*/
	void setDefaultShortcut( const QString& path, const QKeySequence& shortcut );
	
	/*!
		Set the shortcut of the given \a action to \a shortcut taking care that the shortcut is not yet used.
		If an error occurs it will be set in \a error.
	*/
	bool setShortcut( QAction* action, const QKeySequence& shortcut, QString* error = 0 );
	/*!
		Set the shortcut of the action at the given \a path to \a shortcut taking care the shortcut is not yet used.
		If an error occurs it will be set in \a error.
	*/
	bool setShortcut( const QString& path, const QKeySequence& shortcut, QString* error = 0 );

protected:
	static int mColumnCount;
	QHash<QAction*, QList<QAction*> > mChildren;
	QHash<QString, QAction*> mActions;
	
	void insertAction( const QString& path, QAction* action, QAction* parent, int row );
	void removeAction( QAction* action, QAction* parent, int row );
	bool isValid( const QModelIndex& index ) const;
	QAction* createCompletePathNode( const QString& path );
	void removeCompleteEmptyPathNode( QAction* action );
	
	static QString fixedPath( const QString& path );

protected slots:
	void actionChanged();
	void actionDestroyed( QObject* object );

signals:
	/*!
		This signal is emited when the given \a action is inserted.
	*/
	void actionInserted( QAction* node );
	/*!
		This signal is emited when the given \a action is modified.
	*/
	void actionChanged( QAction* action );
	/*!
		This signal is emited when the given \a action is removed.
	*/
	void actionRemoved( QAction* action );
	/*!
		This signal is emited when the model is cleared.
	*/
	void actionsCleared();
};

#endif // PACTIONSMODEL_H
