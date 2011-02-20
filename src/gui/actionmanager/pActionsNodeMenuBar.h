/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNodeMenuBar.h
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
#ifndef PACTIONSNODEMENUBAR_H
#define PACTIONSNODEMENUBAR_H

/*!
	\file pActionsNodeMenuBar.h
	\brief An extended QMenuBar having menu/actions exposed by a pActionsNodeModel.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pActionsNode.h"

#include <QMenuBar>

class pActionsNodeModel;

/*!
	\ingroup FreshGui
	\class pActionsNodeMenuBar
	\brief An extended QMenuBar having menu/actions exposed by a pActionsNodeModel.
*/

class FRESH_EXPORT pActionsNodeMenuBar : public QMenuBar
{
	Q_OBJECT
	
public:
	/*!
		Create a pActionsNodeMenuBar having parent \a parent.
	*/
	pActionsNodeMenuBar( QWidget* parent = 0 );
	/*!
		Destroys the menu bar.
	*/
	~pActionsNodeMenuBar();
	/*!
		Set the model associated with this menu bar.
	*/
	void setModel( pActionsNodeModel* model );
	/*!
		Return the model associated with this menu bar.
	*/
	pActionsNodeModel* model() const;
	/*!
		Add the action \a action at the given \a path.
		
		Return true on success else false.
	*/
	bool addAction( const QString& path, QAction* action );
	/*!
		Add a new action at the given \a path having text \a text and icon \a icon.
		
		Return the new action on success else 0.
	*/
	QAction* addAction( const QString& path, const QString& text, const QIcon& icon = QIcon() );
	/*!
		Add a new menu at the given path \a path and return the pActionsNode associated with this menu.
	*/
	pActionsNode addMenu( const QString& path );
	/*!
		Remove the action at the given path \a path.
	*/
	bool removeAction( const QString& path );
	/*!
		Remove the given action \a action.
	*/
	bool removeAction( QAction* action );
	/*!
		Remove the menu at the given path \a path.
		
		Any child menus and actions are destroyed.
	*/
	bool removeMenu( const QString& path );
	/*!
		Return the menu at the given path \a path.
	*/
	QMenu* menu( const QString& path ) const;
	/*!
		Return the path of the menu \a menu.
	*/
	QString menuPath( QMenu* menu ) const;
	/*!
		Return the action at the given path \a path.
	*/
	QAction* action( const QString& path ) const;
	/*!
		Return the action path at the given path \a path.
	*/
	QString actionPath( QAction* action ) const;
	
protected:
	pActionsNodeModel* mModel;
	QHash<QString, QMenu*> mMenus;
	/*!
		This member update the QMenuBar to reflete to the last changes in the pActionsNodeModel starting from node \a node.
		
		The scan is recursively done.
	*/
	void recursiveSync( const pActionsNode& node );
	/*!
		Completly synchronize the menu bar with the pActionsNodeModel.
	*/
	void sync();

protected slots:
	void model_nodeInserted( const pActionsNode& node );
	void model_nodeChanged( const pActionsNode& node );
	void model_nodeRemoved( const pActionsNode& node );
	void model_nodesCleared();
};

#endif // PACTIONSNODEMENUBAR_H
