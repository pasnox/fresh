#ifndef PDOCKWIDGET_H
#define PDOCKWIDGET_H

/*!
	\file pDockWidget.h
	\brief An extended QDockWidget class that handle a QToolBar as title bar widget.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QDockWidget>
#include <QAction>

class pDockWidgetTitleBar;

/*!
	\ingroup Gui
	\class pDockWidget
	\brief An extended QDockWidget class.

	This dock widget has a QToolBar as title bar widget and handle its proxy widget on activation.
	The dock widget has a small frame when it's floating (except for Oxygen style).
*/
class FRESH_EXPORT pDockWidget : public QDockWidget
{
	Q_OBJECT
	friend class pDockWidgetTitleBar;

public:
	/*!
		Create a pDockWidget having \a title, \a parent and \a flags.
	*/
	pDockWidget( const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	/*!
		Create a pDockWidget having \a parent and \a flags.
	*/
	pDockWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	/*!
		Return the title bar widget.
	*/
	pDockWidgetTitleBar* titleBar() const;
	
	/*!
		Show widget
		Unlike toggleViewAction, this action only shows widget, but never hides
	 */
	QAction* showAction();

protected:
	pDockWidgetTitleBar* mTitleBar;
	QAction* mShowAction;

	void init();
	/*!
		Reimplemented.
	*/
	virtual void paintEvent( QPaintEvent* event );

protected slots:
	void toggleViewAction_triggered( bool toggled );
	void handleWindowActivation();
	void handleFocusProxy();
};

#endif // PDOCKWIDGET_H
