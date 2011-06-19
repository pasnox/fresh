/*!
	\file pDockToolBar.h
	\date 2008-01-14T00:27:40
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A toolbar that can manage dock visibility by adding buttons in it
*/

/*!
	\brief A toolbar that can manage dock visibility by adding buttons in it
	\details The dock toolbars are managed by a pDockToolBarManager for top, right, bottom, left
*/
class pDockToolBar : QToolBar
{
%TypeHeaderCode
#include <gui/dockmanager/pDockToolBar.h>
%End

public:
	/*! This enumeration represents the possible order to use. */ 
	enum DockWidgetsOrder {
		HashedOrder = 0, /*!< The list is ordered by the hash order. */ 
		InsertedOrder /*!< The list is ordered by the insertion order. */ 
	};
	/*!
		Create a pDockToolBar having \a manager as manager and orientation \a orientation.
	*/
	pDockToolBar( pDockToolBarManager* manager, Qt::Orientation orientation = Qt::Horizontal );
	/*!
		Reimplemented.
	*/
	virtual bool event( QEvent* event );
	/*!
		Reimplemented.
	*/
	virtual bool eventFilter( QObject* object, QEvent* event );
	/*!
		Add \a dockWidget being handled by this toolbar.
		
		If \a title and \a icon are non null, they will overwrite the dock widget window title and window icon.
	*/
	void addDockWidget( QDockWidget* dockWidget, const QString& title = QString(), const QIcon& icon = QIcon() );
	/*!
		Remove \a dockWidget from the list of handled dock widgets by this toolbar.
		
		The dock is not deleted nor hided.
	*/
	void removeDockWidget( QDockWidget* dockWidget );
	/*!
		Return true if this toolbar buttons are exclusive, else false.
	*/
	bool isExclusive() const;
	/*!
		Set the toolbar button to be exclusive according to \a exclusive.
	*/
	void setExclusive( bool exclusive );
	/*!
		Change the \a dockWidget visibility according to \a visible.
	*/
	void setDockVisible( QDockWidget* dockWidget, bool visible );
	/*!
		Return the list of managed dock widgets in this toolbar sorted by \a order method.
	*/
	QList<QDockWidget*> dockWidgets( pDockToolBar::DockWidgetsOrder order = pDockToolBar::HashedOrder ) const;
	/*!
		Return the dock widget associated with \a button.
	*/
	QDockWidget* dockWidget( pToolButton* button ) const;
	/*!
		Return true if \a dockWidget is managed by this toolbar.
	*/
	bool hasDockWidget( QDockWidget* dockWidget ) const;
	/*!
		Return all the buttons associated with dock widgets managed by this toolbar.
	*/
	QList<pToolButton*> buttons() const;
	/*!
		Return the button associated with \a dockWidget.
	*/
	pToolButton* button( QDockWidget* dockWidget ) const;
	/*!
		Return the number of managed dock widgets.
	*/
	int count() const;
	/*!
		Return the checkable action for changing the exclusive mode of this toolbar.
	*/
	QAction* toggleExclusiveAction() const;

private:
	/*!
		Update the button properties according to the current manager mode.
	*/
	void setButtonMode( pToolButton* button );
	/*!
		Add a button for \a dockWidget and return it.
	*/
	pToolButton* addButton( QDockWidget* dockWidget );

private slots:
	void internal_checkToolBarVisibility();
	void internal_checkButtonExclusivity( QDockWidget* dockWidget = 0 );
	void internal_updateButtonsState();
	void internal_orientationChanged( Qt::Orientation orientation );
	void internal_buttonClicked( bool checked );
};
