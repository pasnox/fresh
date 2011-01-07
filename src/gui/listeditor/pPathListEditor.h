#ifndef PPATHLISTEDITOR_H
#define PPATHLISTEDITOR_H

/*!
	\file pFileListEditor.h
	\date 2008-01-14T00:27:48
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
	\brief a pStringListEditor that handle files name.
*/

#include "core/FreshExport.h"
#include "pFileListEditor.h"

/*!
	\brief a pPathListEditor that handle paths name.
	\details 
*/
class FRESH_EXPORT pPathListEditor : public pFileListEditor
{
	Q_OBJECT
	
public:
	pPathListEditor( QWidget* parent = 0 );
	pPathListEditor( const QString& title, const QString& path, QWidget* parent = 0 );

protected:
	virtual void init();
	
protected slots:
	virtual void onAddItem();
	virtual void onEditItem();
};

#endif // PPATHLISTEDITOR_H
