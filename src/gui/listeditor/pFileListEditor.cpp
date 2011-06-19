/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pFileListEditor.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pFileListEditor.h"
#include "pIconManager.h"

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
