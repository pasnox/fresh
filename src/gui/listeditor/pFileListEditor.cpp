#include "pFileListEditor.h"
#include "gui/pIconManager.h"

#include <QAction>
#include <QFileDialog>
#include <QModelIndex>

pFileListEditor::pFileListEditor( QWidget* parent )
	: pStringListEditor( parent )
{
	init();
}

pFileListEditor::pFileListEditor( const QString& title, const QString& path, const QString& filter, QWidget* parent )
	: pStringListEditor( title, parent ), mPath( path ), mFilter( filter )
{
	init();
}

void pFileListEditor::init()
{
	aEdit->setIcon( pIconManager::icon( "file.png", ":/fresh/icons" ) );
}

QString pFileListEditor::path() const
{
	return mPath;
}

void pFileListEditor::setPath( const QString& path )
{
	mPath = path;
}

QString pFileListEditor::filter() const
{
	return mFilter;
}

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
