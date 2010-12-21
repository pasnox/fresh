/*!
	\file pActionsNodeMenuBar.h
	\brief The pActionsNodeMenuBar class is a QMenuBar where the hierarchy and actions are exposed by a pActionsNodeModel.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

/*!
	\ingroup Gui
	\class pActionsNodeMenuBar
	\brief The pActionsNodeMenuBar class is a QMenuBar where the hierarchy and actions are exposed by a pActionsNodeModel.
*/

class pActionsNodeMenuBar : QMenuBar
{
%TypeHeaderCode
#include <gui/actionmanager/pActionsNodeMenuBar.h>
%End
	
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
	
};

