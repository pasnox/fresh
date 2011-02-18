#ifndef PLINEEDIT_H
#define PLINEEDIT_H

/*!
	\file pLineEdit.h
	\brief An extended QLineEdit.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QLineEdit>

class QAction;
class QMenu;
class QToolButton;
class QTimer;

/*!
	\ingroup Gui
	\class pLineEdit
	\brief An extended QLineEdit.

	This line edit can have a text place holder (ie: a temporary grey text when the text is empty ),
	a button menu at left and a clear button at right.
*/
class FRESH_EXPORT pLineEdit : public QLineEdit
{
	Q_OBJECT
	
public:
	/*!
		Create an instance of the widget having \a parent as parent.
	*/
	pLineEdit( QWidget* parent = 0 );
	/*!
		Create an instance of the widget having \a parent as parent and text set to \a contents.
	*/
	pLineEdit( const QString& contents, QWidget* parent = 0 );
	
	/*!
		Return the menu of this line edit.
	*/
	QMenu* menu() const;
	/*!
		Set the menu of this line edit.
		Menu is accessible from the left button.
	*/
	void setMenu( QMenu* menu );
	
	/*!
		Return true if the search button is visible, else false.
	*/
	bool isSearchButtonVisible() const;
	/*!
		Return the prompt text (ie: grey text when the text is empty).
	*/
	QString promptText() const;

public slots:
	/*!
		Set the search button visibility to \a visible.
	*/
	void setSearchButtonVisible( bool visible );
	/*!
		Set the prompt text to \a prompt.
	*/
	void setPromptText( const QString& prompt );
	/*!
		Performs a click on the search button.
	*/
	void clickSearchButton();
	/*!
		Performs a click on the clear button.
	*/
	void clickClearButton();

protected:
	static const int mTimeOut;
	int mMargin;
	int mSpacing;
	QToolButton* tbSearch;
	QToolButton* tbClear;
	QString mPromptText;
	QTimer* mTimer;
	
	/*!
		Reimplemented.
	*/
	virtual void paintEvent( QPaintEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void resizeEvent( QResizeEvent* event );
	
	void init();
	void setClearButtonVisible( bool visible );

protected slots:
	void _q_textChanged( const QString& text );
	void timer_timeout();

signals:
	/*!
		This signal is emit when the search button is clicked.
	*/
	void searchButtonClicked();
	/*!
		This signal is emit when the search button menu \a action is triggered.
	*/
	void searchButtonActionTriggered( QAction* action );
	/*!
		This signal is emit when the clear button is clicked.
	*/
	void clearButtonClicked();
	/*!
		This signal is emit when the text in the line edit changed to \a text sometimes after the last typed character.
	*/
	void searchTextChanged( const QString& text );
};

#endif // PLINEEDIT_H
