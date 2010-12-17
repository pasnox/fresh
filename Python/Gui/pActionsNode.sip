/*!
    \file pActionsNode.h
    \brief It's the class used for storing indexes data in pActionsNodeModel.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

/*!
    \ingroup Gui
    \class pActionsNode
    \brief It's the class used for storing indexes data in pActionsNodeModel.
	
	It handle basic data of a node like the icon, text and shortcut.
	
	The internal data is explicitly shared so it's easy to use the api.
*/

class pActionsNode
{
%TypeHeaderCode
#include <../src/gui/actionmanager/pActionsNode.h>
%End

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

};
