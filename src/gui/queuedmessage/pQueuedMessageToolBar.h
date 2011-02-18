#ifndef PQUEUEDMESSAGETOOLBAR_H
#define PQUEUEDMESSAGETOOLBAR_H

/*!
	\file pQueuedMessageToolBar.h
	\brief A toolbar that handle a pQueuedMessageWidget mimicing the chromium yellow toolbar.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pQueuedMessageWidget.h"

#include <QToolBar>

/*!
	\ingroup Gui
	\class pQueuedMessageToolBar
	\brief A toolbar that handle a pQueuedMessageWidget mimicing the chromium yellow toolbar.
*/
class FRESH_EXPORT pQueuedMessageToolBar : public QToolBar
{
	Q_OBJECT
	
public:
	/*!
		Create an instance of message toolbar having parent \a parent.
	*/
	pQueuedMessageToolBar( QWidget* parent = 0 );
	/*!
		Return a pointer to the internal pQueuedMessageWidget widget.
	*/
	pQueuedMessageWidget* queuedMessageWidget() const;
	/*!
		\sa pQueuedMessageWidget::append( const QString&, int )
	*/
	pQueuedMessage appendMessage( const QString& message, int milliSeconds = -1 );

protected:
	pQueuedMessageWidget* mQueuedWidget;
	
	/*!
		Reimplemented.
	*/
	void paintEvent( QPaintEvent* event );

public slots:
	/*!
		\sa pQueuedMessageWidget::append( const pQueuedMessage& )
	*/
	void appendMessage( const pQueuedMessage& message );
	/*!
		\sa pQueuedMessageWidget::remove( const pQueuedMessage& )
	*/
	void removeMessage( const pQueuedMessage& message );

protected slots:
	void messageShown( const pQueuedMessage& message );
	void messageFinished();
};

#endif // PQUEUEDMESSAGETOOLBAR_H
