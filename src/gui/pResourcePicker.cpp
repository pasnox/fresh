/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pResourcePicker.cpp
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
#include "pResourcePicker.h"
#include "ui_pResourcePicker.h"
#include "pQtResourceDialog.h"

#include <QMenu>
#include <QFileDialog>
#include <QInputDialog>
#include <QPixmapCache>
#include <QImageReader>

QFileIconProvider pResourcePicker::iconProvider_;

pResourcePicker::pResourcePicker( QWidget* parent )
  : QWidget( parent ),
  ui( new Ui::pResourcePicker ),
  preferUri_( true ),
  fileNameWildcards_( QLatin1String( "*" ) )
{
  ui->setupUi( this );

  QMenu* imageMenu = new QMenu( this );
  imageMenu->addAction( ui->aFilePath );
  imageMenu->addAction( ui->aResource );
  imageMenu->addAction( ui->aTheme );

  ui->tbResource->setMenu( imageMenu );

  connect( ui->tbResource, SIGNAL( clicked() ), ui->aFilePath, SLOT( trigger() ) );
  connect( ui->leResource, SIGNAL( editingFinished() ), this, SIGNAL( editingFinished() ) );
  connect( ui->leResource, SIGNAL( returnPressed() ), this, SIGNAL( returnPressed() ) );

  setPreviewEnabled( false );

  updatePreview();
}

pResourcePicker::~pResourcePicker()
{
  delete ui;
}

pResourcePicker::Actions pResourcePicker::enabledActions() const
{
  pResourcePicker::Actions actions = pResourcePicker::ActionNone;

  if ( ui->aFilePath->isEnabled() ) {
    actions |= pResourcePicker::ActionFilePath;
  }

  if ( ui->aResource->isEnabled() ) {
    actions |= pResourcePicker::ActionResource;
  }

  if ( ui->aTheme->isEnabled() ) {
    actions |= pResourcePicker::ActionTheme;
  }

  return actions;
}

void pResourcePicker::setEnabledActions( pResourcePicker::Actions actions )
{
  ui->aFilePath->setEnabled( actions.testFlag( pResourcePicker::ActionFilePath ) );
  ui->aResource->setEnabled( actions.testFlag( pResourcePicker::ActionResource ) );
  ui->aTheme->setEnabled( actions.testFlag( pResourcePicker::ActionTheme ) );

  disconnect( ui->tbResource, SIGNAL( clicked() ), 0, 0 );

  if ( ui->aFilePath->isEnabled() ) {
    connect( ui->tbResource, SIGNAL( clicked() ), ui->aFilePath, SLOT( trigger() ) );
  }
  else if ( ui->aResource->isEnabled() ) {
    connect( ui->tbResource, SIGNAL( clicked() ), ui->aResource, SLOT( trigger() ) );
  }
  else if ( ui->aTheme->isEnabled() ) {
    connect( ui->tbResource, SIGNAL( clicked() ), ui->aTheme, SLOT( trigger() ) );
  }
}

bool pResourcePicker::preferUri() const
{
  return preferUri_;
}

void pResourcePicker::setPreferUri( bool prefer )
{
  preferUri_ = prefer;

  ui->leResource->setText( ui->leResource->text() );
}

bool pResourcePicker::isPreviewEnabled() const
{
  return ui->fPreview->isVisible();
}

void pResourcePicker::setPreviewEnabled( bool enabled )
{
  ui->fPreview->setVisible( enabled );
}

QStringList pResourcePicker::fileNameWildcards() const
{
  return fileNameWildcards_;
}

void pResourcePicker::setFileNameWildcards( const QStringList& wildcards )
{
  fileNameWildcards_ = wildcards;
}

QString pResourcePicker::text() const
{
  return ui->leResource->text();
}

void pResourcePicker::setText( const QString& text )
{
  ui->leResource->setText( preferUri_ ? pResourcePicker::toUri( text ) : pResourcePicker::fromUri( text ) );
}

QString pResourcePicker::referenceLocalFilePath() const
{
  return referenceLocalFilePath_;
}

void pResourcePicker::setReferenceLocalFilePath( const QString& filePath )
{
  if ( QFileInfo( filePath ).isDir() && !QDir::isRelativePath( filePath ) )
  {
    referenceLocalFilePath_ = filePath;
  }
}

QString pResourcePicker::toUri( const QString& filePath )
{
  const QUrl url( filePath );

  if ( !url.scheme().isEmpty() || filePath.isEmpty() ) {
    return url.toString();
  }

  // Qt Resource
  if ( filePath.startsWith( QLatin1String( ":" ) ) ) {
    return QString( filePath ).remove( 0, 1 ).prepend( QLatin1String( "qrc://" ) );
  }
  else {
    QFileInfo fileInfo( filePath );

    // Icon theme
    if ( filePath == fileInfo.fileName() && fileInfo.fileName() == fileInfo.completeBaseName() ) {
      return QString( filePath ).prepend( QLatin1String( "icon://" ) );
    }
  }

  // local file system
  return QUrl::fromLocalFile( filePath ).toString();
}

QString pResourcePicker::fromUri( const QString& uri )
{
  const QUrl url( uri );
  const QString scheme = url.scheme();

  if ( scheme.isEmpty() || uri.isEmpty() ) {
    return uri;
  }

  if ( url.isLocalFile() ) {
    return url.toLocalFile();
  }
  else if ( scheme == QLatin1String( "icon" ) ) {
    return url.toString( QUrl::RemoveScheme ).remove( 0, 2 );
  }
  else if ( scheme == QLatin1String( "qrc" ) ) {
    return url.toString( QUrl::RemoveScheme ).remove( 0, 2 ).prepend( QLatin1String( ":" ) );
  }

  return url.toString( QUrl::RemoveScheme ).remove( 0, 2 );
}

QPixmap pResourcePicker::pixmapFromString( const QString& string, const QSize& size, Qt::AspectRatioMode aspect, Qt::TransformationMode transformation )
{
  if ( string.isEmpty() ) {
    return QPixmap();
  }

  const QString key = QString( QLatin1String( "%1x%2x%3" ) ).arg( size.width() ).arg( size.height() ).arg( string );
  QPixmap pixmap;

  if ( !QPixmapCache::find( key, &pixmap ) ) {
    const QString uri = pResourcePicker::toUri( string );
    const QString filePath = pResourcePicker::fromUri( string );
    const QString scheme( QUrl( uri ).scheme() );

    if ( scheme == QLatin1String( "file" ) || scheme == QLatin1String( "qrc" ) ) {
      if ( QImageReader::imageFormat( filePath ).isEmpty() ) {
        const QIcon icon( pResourcePicker::iconProvider_.icon( QFileInfo( filePath ) ) );
        pixmap = icon.pixmap( icon.actualSize( size ) );
      }
      else {
        pixmap = QPixmap( filePath );
      }
    }
    else if ( scheme == QLatin1String( "icon" ) ) {
      const QIcon icon( QIcon::fromTheme( filePath ) );
      pixmap = icon.pixmap( icon.actualSize( size ) );
    }

    if ( pixmap.isNull() ) {
      const QIcon icon( QIcon::fromTheme( QLatin1String( "image-missing" ) ) );
      pixmap = icon.pixmap( icon.actualSize( size ) );
    }

    const QSize aspectSize = pixmap.size().scaled( size, aspect );

    if ( pixmap.size() != aspectSize ) {
      pixmap = pixmap.scaled( size, aspect, transformation );
    }

    if ( !QPixmapCache::insert( key, pixmap ) ) {
      qWarning( "%s: Can't insert pixmap with key: %s", Q_FUNC_INFO, qPrintable( key ) );
    }
  }

  return pixmap;
}

QString pResourcePicker::fileNameFilter() const
{
  if ( fileNameWildcards_.isEmpty() ) {
    return QString::null;
  }

  QStringList items = fileNameWildcards_;

  items.prepend( tr( "Usable Files" ).append( QLatin1String( " (" ) ) );
  items.append( QLatin1String( " )" ) );

  return items.join( QLatin1String( " " ) );
}

void pResourcePicker::updatePreview()
{
  // make sure lPreview has it's final size
  ui->fPreview->layout()->activate();

  QSize size = ui->lPreview->size();
  QPixmap pixmap;

  emit customPreview( ui->leResource->text(), size, pixmap );

  if ( pixmap.isNull() ) {
    pixmap = pResourcePicker::pixmapFromString( ui->leResource->text(), size, Qt::KeepAspectRatio, Qt::SmoothTransformation );
  }
  else {
    size = pixmap.size().scaled( size, Qt::KeepAspectRatio );

    if ( pixmap.size() != size ) {
      pixmap = pixmap.scaled( size, Qt::KeepAspectRatio, Qt::SmoothTransformation );
    }
  }

  ui->lPreview->setPixmap( pixmap );

  emit previewUpdated();
}

void pResourcePicker::on_leResource_textChanged( const QString& text )
{
  updatePreview();
  emit textChanged( text );
}

void pResourcePicker::on_leResource_textEdited( const QString& text )
{
  emit textEdited( text );
}

void pResourcePicker::on_aFilePath_triggered()
{
  const QString text = pResourcePicker::fromUri( ui->leResource->text() );
  QString filePath = QFileDialog::getOpenFileName( this, ui->aFilePath->text(), text, fileNameFilter() );

  if ( filePath.isEmpty() ) {
    return;
  }

  if ( !referenceLocalFilePath_.isEmpty() || !QFileInfo( filePath ).isRelative() ) {
    const QDir referenceDir( referenceLocalFilePath_ );
    filePath = referenceDir.relativeFilePath( filePath );
  }

  if ( filePath.isEmpty() ) {
    return;
  }

  setText( filePath );
  emit textEdited( ui->leResource->text() );
}

void pResourcePicker::on_aResource_triggered()
{
  const QString text = pResourcePicker::fromUri( ui->leResource->text() );
  const QString filePath = pQtResourceDialog::getOpenFileName( this, ui->aResource->text(), text, fileNameWildcards_ );

  if ( filePath.isEmpty() ) {
    return;
  }

  setText( filePath );
  emit textEdited( ui->leResource->text() );
}

void pResourcePicker::on_aTheme_triggered()
{
  bool ok;
  const QString text = pResourcePicker::fromUri( ui->leResource->text() );
  const QString filePath = QInputDialog::getText( this, ui->aTheme->text(), tr( "Write the name of the theme icon" ), QLineEdit::Normal, text, &ok );

  if ( !ok || filePath.isEmpty() ) {
    return;
  }

  setText( filePath );
  emit textEdited( ui->leResource->text() );
}
