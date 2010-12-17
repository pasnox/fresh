/*!
	\file pFileListEditor.h
	\date 2008-01-14T00:27:48
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief a pStringListEditor that handle files name.
*/

/*!
	\brief a pPathListEditor that handle paths name.
	\details 
*/
class pPathListEditor : pFileListEditor
{
%TypeHeaderCode
#include <../src/gui/listeditor/pPathListEditor.h>
%End


public:
	pPathListEditor( QWidget* parent = 0 );
	pPathListEditor( const QString& title, const QString& path, QWidget* parent = 0 );

protected:
	virtual void init();
	
protected slots:
	virtual void onAddItem();
	virtual void onEditItem();
};
