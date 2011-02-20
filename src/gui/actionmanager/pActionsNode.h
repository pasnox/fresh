/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pActionsNode.h
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
#ifndef PACTIONSNODE_H
#define PACTIONSNODE_H

/*!
    \file pActionsNode.h
    \brief It's the class used for storing indexes data in pActionsNodeModel.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QWeakPointer>
#include <QSharedData>
#include <QExplicitlySharedDataPointer>
#include <QAction>

class pActionsNodeModel;

/*!
    \ingroup Gui
    \class pActionsNode
    \brief It's the class used for storing indexes data in pActionsNodeModel.
	
	It handle basic data of a node like the icon, text and shortcut.
	
	The internal data is explicitly shared so it's easy to use the api.
*/

class FRESH_EXPORT pActionsNode
{
	friend class pActionsNodeModel;
	
public:
	/*! This enum defines the differents types of node. */
	enum Type {
		Invalid, /*!< An invalid type */
		Path, /*!< This type represents a path node (ie: a possible hierarchy where to integrate the actions nodes) */
		Action /*!< This type represente an action node (ie: it represente a QAction) */
	};
	
	enum Role {
		ShortcutRole = Qt::UserRole,
		DefaultShortcutRole
	};
	/*!
		Create an invalid node.
	*/
	pActionsNode();
	/*!
		Create a node that is a copy of \a other node.
	*/
	pActionsNode( const pActionsNode& other );
	/*!
		Create a node having type \a type and path \a path
	*/
	pActionsNode( pActionsNode::Type type, const QString& path );
	/*!
		Return true if this node is equal to \a other else false.
	*/
	bool operator==( const pActionsNode& other ) const;
	/*!
		Return true if this node is different than \a other else false.
	*/
	bool operator!=( const pActionsNode& other ) const;
	/*!
		Return true if this node is valid else false.
		
		A valid node has a type different than pActionsNode::Invalid.
	*/
	bool isValid() const;
	/*!
		Return true if this node has children else false.
	*/
	bool hasChildren() const;
	/*!
		Return the pActionsNode::Type of this node.
	*/
	pActionsNode::Type type() const;
	/*!
		Return the path of this node.
	*/
	QString path() const;
	/*!
		Return the QAction associated with this node.
	*/
	QAction* action() const;
	/*!
		Return the icon of this node.
	*/
	QIcon icon() const;
	/*!
		Return the text of this node.
	*/
	QString text() const;
	/*!
		Return the parent node of this node.
	*/
	pActionsNode parent() const;
	/*!
		Return a list of all children of this node.
	*/
	QList<pActionsNode> children() const;
	/*!
		Set the node icon, updating the QAction's icon if needed.
	*/
	void setIcon( const QIcon& icon );
	/*!
		Set the node text, updating the QAction's text if needed.
	*/
	void setText( const QString& text );
	/*!
		Return the shortcut of this node.
	*/
	QKeySequence shortcut() const;
	/*!
		Set the node shortcut, updating the QAction's shortcut if needed.
		
		The model take care that the shortcut is available and not already defined in another node.
	*/
	bool setShortcut( const QKeySequence& shortcut );
	/*!
		Return the node default shortcut.
	*/
	QKeySequence defaultShortcut() const;
	/*!
		Set the node default shortcut, updating the node shortcut if possible.
	*/
	void setDefaultShortcut( const QKeySequence& shortcut );
	/*!
		A convenience function to create a path node.
	*/
	static pActionsNode pathNode( const QString& path, const QString& text = QString::null, const QIcon& icon = QIcon() );
	/*!
		A convenience function to create an action node.
	*/
	static pActionsNode actionNode( const QString& path, QAction* action );

protected:
	class Data : public QSharedData
	{
	public:
		Data();
		virtual ~Data();
		
		bool operator==( const pActionsNode::Data& other ) const;
		bool operator!=( const pActionsNode::Data& other ) const;
		
		pActionsNode::Type type;
		QString path;
		QWeakPointer<QAction> action;
		QIcon icon;
		QString text;
		
		pActionsNodeModel* model;
		pActionsNode* parent;
		QList<pActionsNode> children;
	};
	
	QExplicitlySharedDataPointer<pActionsNode::Data> d;
};

uint qHash( const pActionsNode& node );

#endif // PACTIONSNODE_H
