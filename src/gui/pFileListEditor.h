#ifndef PFILELISTEDITOR_H
#define PFILELISTEDITOR_H

/*!
	\file pFileListEditor.h
	\date 2008-01-14T00:27:44
	\author Filipe AZEVEDO aka Nox P\@sNox <pasnox@gmail.com>
	\brief a pStringListEditor that handle files name.
*/

#include "core/FreshExport.h"
#include "pStringListEditor.h"

/*!
	\brief a pStringListEditor that handle files name.
	\details 
*/
class FRESH_EXPORT pFileListEditor : public pStringListEditor
{
	Q_OBJECT
	
public:
	pFileListEditor( QWidget* parent = 0 );
	pFileListEditor( const QString& title, const QString& path, const QString& filter, QWidget* parent = 0 );
	
	QString path() const;
	void setPath( const QString& path );
	
	QString filter() const;
	void setFilter( const QString& filter );
	
protected:
	QString mPath;
	QString mFilter;
	
	virtual void init();
	
protected slots:
	virtual void onAddItem();
	virtual void onEditItem();
};

#endif // PFILELISTEDITOR_H
