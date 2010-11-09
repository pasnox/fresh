#ifndef PDOCKTOOLBARMANAGERMODERNWIDGET_H
#define PDOCKTOOLBARMANAGERMODERNWIDGET_H

#include "core/FreshExport.h"

#include <QToolBar>

class QScrollArea;
class QBoxLayout;

class pDockToolBar;

class FRESH_EXPORT pDockToolBarManagerModernWidget : public QToolBar
{
	Q_OBJECT
	
public:
	pDockToolBarManagerModernWidget( QWidget* parent = 0 );
	virtual ~pDockToolBarManagerModernWidget();
	
	virtual QSize sizeHint() const;
	
	void setToolBars( const QList<pDockToolBar*>& toolBars );

protected:
	QScrollArea* mArea;
	QWidget* mContent;
	QBoxLayout* mLayout;

protected slots:
	void _q_orientationChanged( Qt::Orientation orientation );
};

#endif // PDOCKTOOLBARMANAGERMODERNWIDGET_H
