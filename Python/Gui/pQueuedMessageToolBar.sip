class pQueuedMessageToolBar : QToolBar
{
%TypeHeaderCode
#include <gui/queuedmessage/pQueuedMessageToolBar.h>
%End

public:
	pQueuedMessageToolBar( QWidget* parent = 0 );
	virtual ~pQueuedMessageToolBar();
	
	pQueuedMessageWidget* queuedMessageWidget() const;
	
	pQueuedMessage appendMessage( const QString& message, int milliSeconds = -1 );

protected:
	
	void paintEvent( QPaintEvent* event );

public slots:
	void appendMessage( const pQueuedMessage& message );
	void removeMessage( const pQueuedMessage& message );

protected slots:
	void messageShown( const pQueuedMessage& message );
	void messageFinished();
};
