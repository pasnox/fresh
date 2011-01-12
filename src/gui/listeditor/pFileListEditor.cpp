#include "pFileListEditor.h"
#include "gui/pIconManager.h"

#include <QAction>
#include <QFileDialog>
#include <QModelIndex>

/*!
	\details Create a new pFileListEditor instance
	\param parent The parent widget
*/
pFileListEditor::pFileListEditor( QWidget* parent )
	: pStringListEditor( parent )
{
	init();
}

/*!
	\details Create a new pFileListEditor instance
	\param parent The parent widget
	\param title The editor title
	\param path The editor path
	\param filter The filter used against file names
*/
pFileListEditor::pFileListEditor( const QString& title, const QString& path, const QString& filter, QWidget* parent )
	: pStringListEditor( title, parent ), mPath( path ), mFilter( filter )
{
	init();
}

void pFileListEditor::init()
{
	aEdit->setIcon( pIconManager::icon( "file.png", ":/fresh/icons" ) );
}

/*!
	\details Return the default path
*/
QString pFileListEditor::path() const
{
	return mPath;
}

/*!
	\details Set the default path used by QFileDialog
	\param path The path to make default
*/
void pFileListEditor::setPath( const QString& path )
{
	mPath = path;
}

/*!
	\details Return the filter
*/
QString pFileListEditor::filter() const
{
	return mFilter;
}

/*!
	\details Set the filter used by QFileDialog
	\param filter The filter to make default
*/
void pFileListEditor::setFilter( const QString& filter )
{
	mFilter = filter;
}

void pFileListEditor::onAddItem()
{
	const QStringList files = QFileDialog::getOpenFileNames( window(), tr( "Choose file(s)" ), mPath, mFilter );
	
	if ( !files.isEmpty() ) {
		foreach ( const QString& file, files ) {
			append( file );
		}
	}
}

void pFileListEditor::onEditItem()
{
	const QModelIndex index = selectedIndex();
	const QString file = QFileDialog::getOpenFileName( window(), tr( "Choose file" ), mPath, mFilter );
	
	if ( !file.isEmpty() ) {
		setValue( file );
	}
}
