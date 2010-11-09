#ifndef PDOCKTOOLBAR_H
#define PDOCKTOOLBAR_H

/*!
	\file pDockToolBar.h
	\date 2008-01-14T00:27:40
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A toolbar that can manage dock visibility by adding buttons in it
*/

#include "core/FreshExport.h"

#include <QToolBar>
#include <QIcon>
#include <QHash>
#include <QBoxLayout>
#include <QMetaType>

class QDockWidget;

class pToolButton;
class pDockToolBarManager;
class pMainWindow;

/*!
	\brief A toolbar that can manage dock visibility by adding buttons in it
	\details The dock toolbars are managed by a pDockToolBarManager for top, right, bottom, left
*/
class FRESH_EXPORT pDockToolBar : public QToolBar
{
	Q_OBJECT
	friend class pDockToolBarManager;
	friend class pDockToolBarManagerModernWidget;

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

private:
	pDockToolBarManager* mManager;
	QWidget* mWidget;
	QBoxLayout* mLayout;
	QAction* aWidget;
	QAction* aToggleExclusive;
	QHash<QDockWidget*, pToolButton*> mDockWidgets;
	
	void setButtonMode( pToolButton* button );
	pToolButton* addButton( QDockWidget* dockWidget, QBoxLayout::Direction direction );

private slots:
	void internal_checkToolBarVisibility();
	void internal_checkButtonExclusivity( QDockWidget* dockWidget = 0 );
	void internal_updateButtonsState();
	void internal_orientationChanged( Qt::Orientation orientation );
	void internal_buttonClicked( bool checked );
};

Q_DECLARE_METATYPE( QDockWidget* )

#endif // PDOCKTOOLBAR_H
