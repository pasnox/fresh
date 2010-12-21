/*!
	\file pMainWindow.h
	\date 2008-01-14T00:27:46
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief An extended QMainWindow
*/

/*!
	\brief An extended QMainWindow.
	\details This extended mainwindow contains some usefull features :
	\details a pMenuBar as menu bar, a pDockToolBarManager for tabbed management of docked widgets
	\details and a pSettings object for storing/reading your settings.
	\details There is also a confortable dockToolBar() member.
*/
class pMainWindow : QMainWindow
{
%TypeHeaderCode
#include <gui/pMainWindow.h>
%End

public:
	pMainWindow( QWidget* parent = 0, Qt::WindowFlags windowFlags = 0 );
	virtual ~pMainWindow();
	
	virtual QMenu* createPopupMenu();
	
	virtual pSettings* settings() const;
	virtual pActionsNodeMenuBar* menuBar() const;
	virtual pQueuedMessageToolBar* queuedMessageToolBar() const;
	virtual pDockToolBarManager* dockToolBarManager() const;
	pDockToolBar* dockToolBar( Qt::ToolBarArea area ) const;

protected:
	virtual void showEvent( QShowEvent* event );
	virtual void closeEvent( QCloseEvent* event );

public slots:
	virtual void saveState();
	virtual void restoreState();
};
