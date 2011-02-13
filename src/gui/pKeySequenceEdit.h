#ifndef PKEYSEQUENCEEDIT_H
#define PKEYSEQUENCEEDIT_H

/*!
	\file pKeySequenceEdit.h
	\brief A special QLineEdit that allow to defin QShortcut.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"
#include "pLineEdit.h"

/*!
	\ingroup Gui
	\class pKeySequenceEdit
	\brief A special QLineEdit that allow to define QShortcut.

	When the user press some combinaison keys, the result is wrotten in the pLineEdit.
*/
class FRESH_EXPORT pKeySequenceEdit : public pLineEdit
{
	Q_OBJECT

public:
	/*!
		Create a instance of pKeySequenceEdit having parent \a parent.
	*/
	pKeySequenceEdit( QWidget* parent = 0 );
	/*!
		Create a instance of pKeySequenceEdit having parent \a parent.
		The widget text is set to \a content.
	*/
	pKeySequenceEdit( const QString& contents, QWidget* parent = 0 );

protected:
	mutable bool mFinished;
	
	/*!
		Reimplemented.
	*/
	virtual void keyPressEvent( QKeyEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void keyReleaseEvent( QKeyEvent* event );
	
	QString keySequence( QKeyEvent* event ) const;
};

#endif // PKEYSEQUENCEEDIT_H
