#ifndef PKEYSEQUENCEEDIT_H
#define PKEYSEQUENCEEDIT_H

/*!
	\file pKeySequenceEdit.h
	\date 2008-01-14T00:27:45
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
	\brief A special QLineEdit that allow to defin QShortcut
*/

#include "core/FreshExport.h"
#include "pLineEdit.h"

/*!
	\brief A special QLineEdit that allow to defin QShortcut
	\details When the user press some combinaison keys, the result is
	\details wrotten in the QLineEdit
*/
class FRESH_EXPORT pKeySequenceEdit : public pLineEdit
{
	Q_OBJECT

public:
	pKeySequenceEdit( QWidget* parent = 0 );
	pKeySequenceEdit( const QString& contents, QWidget* parent = 0 );

protected:
	mutable bool mFinished;
	
	virtual void keyPressEvent( QKeyEvent* event );
	virtual void keyReleaseEvent( QKeyEvent* event );
	
	QString keySequence( QKeyEvent* event ) const;
};

#endif // PKEYSEQUENCEEDIT_H
