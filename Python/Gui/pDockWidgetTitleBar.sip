/*!
	\brief A custom title bar for pDockWidget
	\details that herits QToolBar so custom actions can be added in the title bar.
*/
class pDockWidgetTitleBar : QToolBar
{
%TypeHeaderCode
#include <gui/dockwidget/pDockWidgetTitleBar.h>
%End

public:
	pDockWidgetTitleBar( pDockWidget* parent = 0 );
	
	virtual bool event( QEvent* event );
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	
	QWidget* addAction( QAction* action, int index = -1 );
	void addSeparator( int index = -1 );

protected:
	virtual bool eventFilter( QObject* object, QEvent* event );
	virtual void paintEvent( QPaintEvent* event );
	QIcon icon() const;
	QSize windowIconSize() const;
	void updateStyleChange();

protected slots:
	void aOrientation_triggered();
	void aFloat_triggered();
	void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );
};
