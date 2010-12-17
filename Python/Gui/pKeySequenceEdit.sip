/*!
	\file pKeySequenceEdit.h
	\date 2008-01-14T00:27:45
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief A special QLineEdit that allow to defin QShortcut
*/

/*!
	\brief A special QLineEdit that allow to defin QShortcut
	\details When the user press some combinaison keys, the result is
	\details wrotten in the QLineEdit
*/
class pKeySequenceEdit : pLineEdit
{
%TypeHeaderCode
#include <../src/gui/pKeySequenceEdit.h>
%End

public:
	pKeySequenceEdit( QWidget* parent = 0 );
	pKeySequenceEdit( const QString& contents, QWidget* parent = 0 );

protected:
	
	virtual void keyPressEvent( QKeyEvent* event );
	virtual void keyReleaseEvent( QKeyEvent* event );
	
	QString keySequence( QKeyEvent* event ) const;
};
