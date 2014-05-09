/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQtResourceDialog.cpp
** Date      : 2011-02-20T00:44:25
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
#include "pQtResourceDialog.h"
#include "ui_pQtResourceDialog.h"
#include "pQtResourceModel.h"

#include <QPushButton>
#include <QDebug>

pQtResourceDialog::pQtResourceDialog( QWidget* parent )
  : QDialog( parent ),
    ui( new Ui::pQtResourceDialog ),
    m_model( new pQtResourceModel( this ) )
{
  ui->setupUi( this );
  ui->tvFiles->setModel( m_model );
  ui->tvFiles->setColumnHidden( 1, true );

  view_selectionModel_selectionChanged();

  connect( ui->tvFiles->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( view_selectionModel_selectionChanged() ) );
}

pQtResourceDialog::~pQtResourceDialog()
{
  delete ui;
}

pQtResourceModel* pQtResourceDialog::model() const
{
  return m_model;
}

QTreeView*pQtResourceDialog::view() const
{
  return ui->tvFiles;
}

QLabel*pQtResourceDialog::label() const
{
  return ui->lLabel;
}

QStringList pQtResourceDialog::selectedFiles() const
{
  const QModelIndexList indexes =  ui->tvFiles->selectionModel()->selectedIndexes();
  QStringList files;

  foreach ( const QModelIndex& index, indexes ) {
    files << model()->fileInfo( index ).absoluteFilePath();
  }

  return files;
}

void pQtResourceDialog::setSelectedFiles( const QStringList& files )
{
  QModelIndexList indexes = m_model->modelIndexList( files );

  qSort( indexes );

  if ( indexes.isEmpty() ) {
    return;
  }

  const QItemSelection selection( indexes.first(), indexes.last() );
  ui->tvFiles->selectionModel()->select( selection, QItemSelectionModel::ClearAndSelect );
  ui->tvFiles->scrollTo( indexes.first() );
}

QString pQtResourceDialog::selectedFile() const
{
  return selectedFiles().value( 0 );
}

void pQtResourceDialog::setSelectedFile( const QString& file )
{
  if ( !file.isEmpty() ) {
    setSelectedFiles( QStringList( file ) );
  }
}

QString pQtResourceDialog::getExistingDirectory( QWidget* parent, const QString& caption, const QString& dir )
{
  const QFileInfo fileInfo( dir );
  pQtResourceDialog dlg( parent );
  delete dlg.layout()->takeAt( 0 );
  dlg.setWindowTitle( caption.isEmpty() ? tr( "Select a folder" ) : caption );
  dlg.view()->setSelectionMode( QAbstractItemView::SingleSelection );
  dlg.model()->setFilter( QDir::AllDirs );
  dlg.setSelectedFile( fileInfo.isDir() ? fileInfo.absoluteFilePath() : fileInfo.absolutePath() );

  if ( dlg.exec() == QDialog::Accepted ) {
    return dlg.selectedFile();
  }

  return QString::null;
}

QString pQtResourceDialog::getOpenFileName( QWidget* parent, const QString& caption, const QString& dir, const QStringList& filters )
{
  pQtResourceDialog dlg( parent );
  delete dlg.layout()->takeAt( 0 );
  dlg.setWindowTitle( caption.isEmpty() ? tr( "Select a file" ) : caption );
  dlg.view()->setSelectionMode( QAbstractItemView::SingleSelection );
  dlg.model()->setFilter( QDir::Files );
  dlg.model()->setNameFilters( filters );
  dlg.setSelectedFile( dir );

  if ( dlg.exec() == QDialog::Accepted ) {
    return dlg.selectedFile();
  }

  return QString::null;
}

QStringList pQtResourceDialog::getOpenFileNames( QWidget* parent, const QString& caption, const QString& dir, const QStringList& filters )
{
  pQtResourceDialog dlg( parent );
  delete dlg.layout()->takeAt( 0 );
  dlg.setWindowTitle( caption.isEmpty() ? tr( "Select one or more files" ) : caption );
  dlg.view()->setSelectionMode( QAbstractItemView::ExtendedSelection );
  dlg.model()->setFilter( QDir::Files );
  dlg.model()->setNameFilters( filters );
  dlg.setSelectedFile( dir );

  if ( dlg.exec() == QDialog::Accepted ) {
    return dlg.selectedFiles();
  }

  return QStringList();
}

void pQtResourceDialog::view_selectionModel_selectionChanged()
{
  const int count = ui->tvFiles->selectionModel()->selectedIndexes().count();
  ui->dbbButtons->button( QDialogButtonBox::Open )->setEnabled( count > 0 );
}
