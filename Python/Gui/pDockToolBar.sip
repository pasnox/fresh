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
#include <../src/gui/dockmanager/pDockToolBar.h>
%End



public:
	enum DockWidgetsOrder {
		HashedOrder = 0,
		InsertedOrder
	};
	
	pDockToolBar( pDockToolBarManager* manager, Qt::Orientation orientation = Qt::Horizontal );

	virtual bool event( QEvent* event );
	virtual bool eventFilter( QObject* object, QEvent* event );

	void addDockWidget( QDockWidget* dockWidget, const QString& title = QString(), const QIcon& icon = QIcon() );
	void removeDockWidget( QDockWidget* dockWidget );

	bool exclusive() const;
	void setExclusive( bool exclusive );
	
	void setDockVisible( QDockWidget* dockWidget, bool visible );
	
	QList<QDockWidget*> dockWidgets( pDockToolBar::DockWidgetsOrder order = pDockToolBar::HashedOrder ) const;
	QDockWidget* dockWidget( pToolButton* button ) const;
	bool hasDockWidget( QDockWidget* dockWidget ) const;
	
	QList<pToolButton*> buttons() const;
	pToolButton* button( QDockWidget* dockWidget ) const;

	int count() const;
	
	QAction* toggleExclusiveAction() const;

private slots:
	void internal_checkToolBarVisibility();
	void internal_checkButtonExclusivity( QDockWidget* dockWidget = 0 );
	void internal_updateButtonsState();
	void internal_orientationChanged( Qt::Orientation orientation );
	void internal_buttonClicked( bool checked );
};
