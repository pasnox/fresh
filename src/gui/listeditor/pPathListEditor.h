#ifndef PPATHLISTEDITOR_H
#define PPATHLISTEDITOR_H

/*!
	\file pPathListEditor.h
	\brief a pStringListEditor that handle paths name.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pFileListEditor.h"

/*!
	\ingroup Gui
	\class pPathListEditor
	\brief a pStringListEditor that handle paths name.
*/
class FRESH_EXPORT pPathListEditor : public pFileListEditor
{
	Q_OBJECT
	
public:
	/*!
		Create an editor instance having parent \a parent.
	*/
	pPathListEditor( QWidget* parent = 0 );
	/*!
		Create an editor instance having parent \a parent and title \a title.
		The default path for the file dialog will be \a path.
	*/
	pPathListEditor( const QString& title, const QString& path, QWidget* parent = 0 );

protected:
	virtual void init();
	
protected slots:
	virtual void onAddItem();
	virtual void onEditItem();
};

#endif // PPATHLISTEDITOR_H
