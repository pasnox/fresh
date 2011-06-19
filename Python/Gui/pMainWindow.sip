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
	/*!
		Create an instance of the main window having \a parent as parent and window flags \a windowFlags.
	*/
	pMainWindow( QWidget* parent = 0, Qt::WindowFlags windowFlags = 0 );
	/*!
		Reimplemented.
	*/
	virtual ~pMainWindow();
	/*!
		Reimplemented.
	*/
	virtual QMenu* createPopupMenu();
	/*!
		Return the settings object used by this main window.
	*/
	virtual pSettings* settings() const;
	/*!
		Return the menu bar object.
	*/
	virtual pActionsMenuBar* menuBar() const;
	/*!
		Add \a dockWidget in \a area having \a orientation.
	*/
	void addDockManagerWidget( Qt::ToolBarArea area, QDockWidget* dockWidget, Qt::Orientation orientation = Qt::Horizontal );
	/*!
		Seth the dock toolbar in \a area to be exclusive according to \a exclusive.
	*/
	void setDockManagerAreaExclusive( Qt::ToolBarArea area, bool exclusive );
	/*!
		Set the dock manager presentation mode to \a mode.
	*/
	void setDockManagerMode( pDockToolBarManager::Mode mode );
	/*!
		Append a queued \a message in the message toolbar with an auto close defined to \a milliSeconds.
		
		\sa pQueuedMessageToolBar::appendMessage( const QString&, int ).
	*/
	void appendMessage( const QString& message, int milliSeconds = -1 );
	/*!
		Append a queued \a message in the message toolbar.
		
		\sa pQueuedMessageToolBar::appendMessage( const pQueuedMessage& ).
	*/
	void appendMessage( const pQueuedMessage& message );

protected:
	/*!
		Reimplemented.
	*/
	virtual void showEvent( QShowEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void closeEvent( QCloseEvent* event );
	/*!
		Return the queued message toolbar object.
	*/
	virtual pQueuedMessageToolBar* queuedMessageToolBar() const;
	/*!
		Return the dock widget toolbar manager object.
	*/
	virtual pDockToolBarManager* dockToolBarManager() const;
	/*!
		Return the dock toolbar manager for \a area.
	*/
	pDockToolBar* dockToolBar( Qt::ToolBarArea area ) const;

public slots:
	/*!
		Save the state of the main window.
		
		\sa QMainWindow::saveState().
	*/
	virtual void saveState();
	/*!
		Restore the state of the main window.
		
		\sa QMainWindow::restoreState().
	*/
	virtual void restoreState();
};
