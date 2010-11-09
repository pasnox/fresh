#include "pPathListEditor.h"
#include "gui/pIconManager.h"

#include <QAction>
#include <QFileDialog>
#include <QModelIndex>

/*!
	\details Create a new pPathListEditor instance
	\param parent The parent widget
*/
pPathListEditor::pPathListEditor( QWidget* parent )
	: pFileListEditor( parent )
{
	init();
}

/*!
	\details Create a new pPathListEditor instance
	\param parent The parent widget
	\param title The editor title
	\param  path The editor path
*/
pPathListEditor::pPathListEditor( const QString& title, const QString& path, QWidget* parent )
	: pFileListEditor( title, path, QString::null, parent )
{
	init();
}

void pPathListEditor::init()
{
	aEdit->setIcon( pIconManager::icon( "folder.png", ":/fresh/icons" ) );
}

void pPathListEditor::onAddItem()
{
	const QString path = QFileDialog::getExistingDirectory( window(), tr( "Choose directory" ), mPath );
	
	if ( !path.isEmpty() ) {
		appendValue( path );
	}
}

void pPathListEditor::onEditItem()
{
	const QModelIndex index = selectedIndex();
	const QString path = QFileDialog::getExistingDirectory( window(), tr( "Choose directory" ), mPath );
	
	if ( !path.isEmpty() ) {
		setSelectedIndexValue( path );
	}
}
