class pDockToolBarManagerModernWidget : QToolBar
{
%TypeHeaderCode
#include <../src/gui/dockmanager/pDockToolBarManagerModernWidget.h>
%End

public:
	pDockToolBarManagerModernWidget( QWidget* parent = 0 );
	virtual ~pDockToolBarManagerModernWidget();
	
	virtual QSize sizeHint() const;
	
	void setToolBars( const QList<pDockToolBar*>& toolBars );

protected slots:
	void _q_orientationChanged( Qt::Orientation orientation );
};
