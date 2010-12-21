
class pLineEdit : QLineEdit
{
%TypeHeaderCode
#include <gui/pLineEdit.h>
%End
	
public:
	pLineEdit( QWidget* parent = 0 );
	pLineEdit( const QString& contents, QWidget* parent = 0 );
	
	QMenu* menu() const;
	void setMenu( QMenu* menu );
	
	bool isSearchButtonVisible() const;
	QString promptText() const;

public slots:
	void setSearchButtonVisible( bool visible );
	void setPromptText( const QString& prompt );
	
	void clickSearchButton();
	void clickClearButton();

protected:
	virtual void paintEvent( QPaintEvent* event );
	virtual void resizeEvent( QResizeEvent* event );
	
	void init();
	void setClearButtonVisible( bool visible );

protected slots:
	void _q_textChanged( const QString& text );
	void timer_timeout();

signals:
	void searchButtonClicked();
	void searchButtonActionTriggered( QAction* action );
	void clearButtonClicked();
	void searchTextChanged( const QString& text );
};
