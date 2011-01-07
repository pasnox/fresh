#ifndef PDOCKTOOLBARMANAGERMODERNWIDGET_H
#define PDOCKTOOLBARMANAGERMODERNWIDGET_H

/*!
	\file pDockToolBarManagerModernWidget.h
	\brief This is the modern widget used in the pDockToolBarManager when in Modern mode.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QToolBar>

class QScrollArea;
class QBoxLayout;

class pDockToolBar;

/*!
	\ingroup Gui
	\class pDockToolBarManagerModernWidget
	\brief This is the modern widget used in the pDockToolBarManager when in Modern mode.
*/
class FRESH_EXPORT pDockToolBarManagerModernWidget : public QToolBar
{
	Q_OBJECT
	friend class pDockToolBarManager;
	
public:
	/*!
		Create a pDockToolBarManagerModernWidget having parent \a parent.
	*/
	pDockToolBarManagerModernWidget( QWidget* parent = 0 );
	/*!
		Destroyes the widget.
	*/
	virtual ~pDockToolBarManagerModernWidget();
	/*!
		Reimplemented.
	*/
	virtual QSize sizeHint() const;
	/*!
		Set the internal toolbars handled by the modern widget..
	*/
	void setToolBars( const QList<pDockToolBar*>& toolBars );

protected:
	QScrollArea* mArea;
	QWidget* mContent;
	QBoxLayout* mLayout;

protected slots:
	void _q_orientationChanged( Qt::Orientation orientation );
};

#endif // PDOCKTOOLBARMANAGERMODERNWIDGET_H
