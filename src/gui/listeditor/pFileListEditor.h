#ifndef PFILELISTEDITOR_H
#define PFILELISTEDITOR_H

/*!
	\file pFileListEditor.h
	\brief a pStringListEditor that handle files name.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"
#include "pStringListEditor.h"

/*!
	\ingroup Gui
	\class pFileListEditor
	\brief a pStringListEditor that handle files name.
*/
class FRESH_EXPORT pFileListEditor : public pStringListEditor
{
	Q_OBJECT
	
public:
	/*!
		Create an editor instance having parent \a parent.
	*/
	pFileListEditor( QWidget* parent = 0 );
	/*!
		Create an editor instance having parent \a parent and title \a title.
		The path and filter for the file dialog will be \a path and \a filter.
	*/
	pFileListEditor( const QString& title, const QString& path, const QString& filter, QWidget* parent = 0 );
	
	/*!
		Return the path used in files dialogs.
	*/
	QString path() const;
	/*!
		Set the path used in files dialogs to \a path.
	*/
	void setPath( const QString& path );
	/*!
		Return the filter applied in files dialogs.
	*/
	QString filter() const;
	/*!
		Set the filter used in files dialogs.
	*/
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
