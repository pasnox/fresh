/*!
	\brief A QDockWidget that keep it's size
	\details when hidden then shown when docked
*/
class pDockWidget : QDockWidget
{
%TypeHeaderCode
#include <gui/dockwidget/pDockWidget.h>
%End

public:
	pDockWidget( const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0 );
	pDockWidget( QWidget* parent = 0, Qt::WindowFlags flags = 0 );

	pDockWidgetTitleBar* titleBar() const;

protected:
	void init();
	virtual void paintEvent( QPaintEvent* event );
};
