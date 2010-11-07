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
#include <QMetaType>

class QDockWidget;
class QFrame;
class QBoxLayout;

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

public:
	enum DockWidgetsOrder {
		HashedOrder = 0,
		InsertedOrder
	};
	
	pDockToolBar( pDockToolBarManager* manager, Qt::Orientation orientation = Qt::Horizontal );

	virtual bool event( QEvent* event );
	virtual bool eventFilter( QObject* object, QEvent* event );

	QAction* addAction( QAction* action = 0, bool insert = true );
	void addActions( QList<QAction*> actions, bool insert = true );

	void addDock( QDockWidget* dock, const QString& title = QString(), const QIcon& icon = QIcon() );
	void removeDock( QDockWidget* dock );

	bool exclusive() const;
	void setExclusive( bool exclusive );
	
	bool textAlwaysVisible() const;
	void setTextAlwaysVisible( bool visible );
	
	void setDockVisible( QDockWidget* dock, bool visible );
	
	QList<QDockWidget*> docks( pDockToolBar::DockWidgetsOrder order = pDockToolBar::HashedOrder ) const;
	QDockWidget* dock( pToolButton* button ) const;
	bool hasDockWidget( QDockWidget* dockWidget ) const;
	
	QList<pToolButton*> buttons() const;
	pToolButton* button( QDockWidget* dock ) const;

	int count() const;
	
	QAction* toggleExclusiveAction() const;

private:
	pDockToolBarManager* mManager;
	bool mTextAlwaysVisible;
	QFrame* mFrame;
	QBoxLayout* mLayout;
	QAction* aDockFrame;
	QAction* aToggleExclusive;
	QHash<QDockWidget*, pToolButton*> mDockWidgets;

private slots:
	void internal_checkToolBarVisibility();
	void internal_checkButtonExclusivity( QDockWidget* dockWidget = 0 );
	void internal_checkButtonText( pToolButton* button );
	void internal_orientationChanged( Qt::Orientation orientation );
	void internal_buttonClicked( bool checked );
};

Q_DECLARE_METATYPE( QDockWidget* )

#endif // PDOCKTOOLBAR_H
