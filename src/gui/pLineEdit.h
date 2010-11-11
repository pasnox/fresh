#ifndef PLINEEDIT_H
#define PLINEEDIT_H

#include "core/FreshExport.h"

#include <QLineEdit>

class QAction;
class QMenu;
class QToolButton;
class QTimer;

class FRESH_EXPORT pLineEdit : public QLineEdit
{
	Q_OBJECT
	
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
	static const int mTimeOut;
	int mMargin;
	int mSpacing;
	QToolButton* tbSearch;
	QToolButton* tbClear;
	QString mPromptText;
	QTimer* mTimer;
	
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

#endif // PLINEEDIT_H
