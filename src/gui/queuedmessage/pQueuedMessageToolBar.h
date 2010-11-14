#ifndef PQUEUEDMESSAGETOOLBAR_H
#define PQUEUEDMESSAGETOOLBAR_H

#include "core/FreshExport.h"
#include "pQueuedMessageWidget.h"

#include <QToolBar>

class FRESH_EXPORT pQueuedMessageToolBar : public QToolBar
{
	Q_OBJECT
	
public:
	pQueuedMessageToolBar( QWidget* parent = 0 );
	virtual ~pQueuedMessageToolBar();
	
	pQueuedMessageWidget* queuedMessageWidget() const;
	
	pQueuedMessage appendMessage( const QString& message, int milliSeconds = -1 );

protected:
	pQueuedMessageWidget* mQueuedWidget;
	
	void paintEvent( QPaintEvent* event );

public slots:
	void appendMessage( const pQueuedMessage& message );
	void removeMessage( const pQueuedMessage& message );

protected slots:
	void messageShown( const pQueuedMessage& message );
	void messageFinished();
};

#endif // PQUEUEDMESSAGETOOLBAR_H
