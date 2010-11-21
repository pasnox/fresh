#ifndef PDOCKWIDGETTITLEBAR_H
#define PDOCKWIDGETTITLEBAR_H

#include "core/FreshExport.h"
#include "pDockWidget.h"

#include <QToolBar>

class QToolButton;
class pToolButton;
class QStyleOptionToolButton;

/*!
	\brief A custom title bar for pDockWidget
	\details that herits QToolBar so custom actions can be added in the title bar.
*/
class FRESH_EXPORT pDockWidgetTitleBar : public QToolBar
{
	Q_OBJECT

public:
	pDockWidgetTitleBar( pDockWidget* parent = 0 );
	
	virtual bool event( QEvent* event );
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	
	QWidget* addAction( QAction* action, int index = -1 );
	void addSeparator( int index = -1 );

protected:
	pDockWidget* mDock;
	QAction* aOrientation;
	QAction* aFloat;
	QAction* aClose;
	pToolButton* tbOrientation;
	pToolButton* tbFloat;
	pToolButton* tbClose;
	
	virtual bool eventFilter( QObject* object, QEvent* event );
	virtual void paintEvent( QPaintEvent* event );
	QSize windowIconSize() const;
	void updateStyleChange();

protected slots:
	void aOrientation_triggered();
	void aFloat_triggered();
	void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );
};

#endif // PDOCKWIDGETTITLEBAR_H
