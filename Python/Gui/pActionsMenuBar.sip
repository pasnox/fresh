class pActionsMenuBar : QMenuBar
{
%TypeHeaderCode
#include <gui/actionmanager/pActionsMenuBar.h>
%End
	
public:
	/*!
		Create a pActionsMenuBar having parent \a parent.
	*/
	pActionsMenuBar( QWidget* parent = 0 );
	/*!
		Destroys the menu bar.
	*/
	~pActionsMenuBar();
	/*!
		Set the model associated with this menu bar.
	*/
	void setModel( pActionsModel* model );
	/*!
		Return the model associated with this menu bar.
	*/
	pActionsModel* model() const;
	
protected:

	/*!
		This member update the QMenuBar to reflete to the last changes in the pActionsModel starting from node \a node.
		
		The scan is recursively done.
	*/
	void recursiveSync( QAction* action );
	/*!
		Completly synchronize the menu bar with the pActionsModel.
	*/
	void sync();

protected slots:
	void model_actionInserted( QAction* action );
	void model_actionChanged( QAction* action );
	void model_actionRemoved( QAction* action );
	void model_actionsCleared();
};

