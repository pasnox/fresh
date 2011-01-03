#ifndef PDOCKWIDGET_H
#define PDOCKWIDGET_H

#include "core/FreshExport.h"

#include <QDockWidget>

class pDockWidgetTitleBar;

/*!
	\brief A QDockWidget that keep it's size
		
	\details when hidden then shown when docked
	If pDockWidget has focus proxy, this proxy will get focus,
	when QDockWidget::toggleViewAction was activated.	
*/
class FRESH_EXPORT pDockWidget : public QDockWidget
{
	Q_OBJECT
	friend class pDockWidgetTitleBar;
	
public:
	pDockWidget( const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	pDockWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

	pDockWidgetTitleBar* titleBar() const;

protected:
	pDockWidgetTitleBar* mTitleBar;

	void init();
	virtual void paintEvent( QPaintEvent* event );

protected slots:
	void toggleViewAction_toggled( bool toggled );
	void handleWindowActivation();
	void handleFocusProxy();
};

#endif // PDOCKWIDGET_H
