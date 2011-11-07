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

protected slots:
	void model_actionInserted( QAction* action );
	void model_actionsCleared();
};

