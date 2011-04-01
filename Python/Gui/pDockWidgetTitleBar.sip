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
	/*!
		Create a 
	*/
	pDockWidgetTitleBar( QDockWidget* parent = 0 );
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
	/*!
		Reimplemented
	*/
	virtual bool event( QEvent* event );
	/*!
		Reimplemented.
	*/
	virtual bool eventFilter( QObject* object, QEvent* event );
	/*!
		Reimplemented
	*/
	virtual void paintEvent( QPaintEvent* event );
	
	QIcon icon() const;
	QSize windowIconSize() const;
	void updateStyleChange();

protected slots:
	void aOrientation_triggered();
	void aFloat_triggered();
	void dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features );};
