#ifndef PDOCKWIDGETTITLEBAR_H
#define PDOCKWIDGETTITLEBAR_H

/*!
	\file pDockWidgetTitleBar.h
	\brief A custom title bar for pDockWidget that herits QToolBar.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QToolBar>
#include <QDockWidget>

class QToolButton;
class pToolButton;
class QStyleOptionToolButton;

/*!
	\ingroup Gui
	\class pDockWidgetTitleBar
	\brief A custom title bar for pDockWidget that herits QToolBar.

	Actions can be inserted in the title bar like in any toolbar !
*/
class FRESH_EXPORT pDockWidgetTitleBar : public QToolBar
{
	Q_OBJECT

public:
	/*!
		Create a 
	*/
	pDockWidgetTitleBar( QDockWidget* parent = 0 );
	/*!
		Reimplemented.
	*/
	virtual bool event( QEvent* event );
	/*!
		Reimplemented.
	*/
	virtual QSize minimumSizeHint() const;
	/*!
		Reimplemented.
	*/
	virtual QSize sizeHint() const;
	/*!
		Insert \a action at \a index position.
		If \a index is -1 then it's append at end.
	*/
	QWidget* addAction( QAction* action, int index = -1 );
	/*!
		Insert a separator at \a index.
		If \a index is -1 then it's append at end.
	*/
	void addSeparator( int index = -1 );

protected:
	QDockWidget* mDock;
	QAction* aOrientation;
	QAction* aFloat;
	QAction* aClose;
	pToolButton* tbOrientation;
	pToolButton* tbFloat;
	pToolButton* tbClose;
	/*!
		Reimplemented.
	*/
	virtual bool eventFilter( QObject* object, QEvent* event );
	/*!
		Reimplemented
	*/
	virtual void paintEvent( QPaintEvent* event );
	/*!
		Return the required QSize for drawinf the window icon if available.
		\note The returned size will be null if no window icon is setted.
	*/
	QSize windowIconSize() const;
	void updateStyleChange();

protected slots:
	void aOrientation_triggered();
	void aFloat_triggered();
	void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );
};

#endif // PDOCKWIDGETTITLEBAR_H
