/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pFileDialog.cpp
** Date      : 2011-02-20T00:41:35
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
#include "pFileDialog.h"
#include "pCoreUtils.h"

#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QTextCodec>
#include <QCheckBox>
#include <QAbstractItemView>
#include <QToolButton>

pFileDialog::pFileDialog( QWidget* parent, const QString& caption, const QString& directory, const QString& filter, bool textCodecEnabled, bool openReadOnlyEnabled )
    : QFileDialog( parent, caption, directory, filter )
{
    setFileMode( QFileDialog::AnyFile );
    setOption( QFileDialog::DontUseNativeDialog );
    
    // get grid layout
    glDialog = qobject_cast<QGridLayout*>( layout() );
    
    // assert on gridlayout
    Q_ASSERT( glDialog );
    
    // relook the dialog to be more friendly
    QLabel* lLookIn = findChild<QLabel*>( "lookInLabel" );
    QComboBox* cbLookIn = findChild<QComboBox*>( "lookInCombo" );
    QToolButton* tbNewFolder = findChild<QToolButton*>( "newFolderButton" );
    QAbstractItemView* sidebar = findChild<QAbstractItemView*>( "sidebar" );
    QFrame* fFrame = findChild<QFrame*>( "frame" );
    QBoxLayout* hLayout = 0;
    
    // search layout containing tbNewFolder
    foreach ( QLayout* layout, findChildren<QLayout*>() ) {
        if ( layout->indexOf( tbNewFolder ) != -1 ) {
            hLayout = qobject_cast<QBoxLayout*>( layout );
            break;
        }
    }
    
    if ( lLookIn ) {
        lLookIn->setVisible( false );
    }
    
    if ( hLayout ) {
        hLayout->setSpacing( 3 );
        hLayout->insertStretch( hLayout->indexOf( tbNewFolder ) );
    }
    
    if ( cbLookIn && fFrame ) {
        QBoxLayout* vLayout = qobject_cast<QBoxLayout*>( fFrame->layout() );
        
        if ( vLayout ) {
            vLayout->setSpacing( 3 );
            vLayout->insertWidget( 0, cbLookIn );
            
            if ( hLayout ) {
                glDialog->removeItem( hLayout );
                hLayout->setParent( 0 );
                vLayout->insertLayout( 0, hLayout );
            }
        }
    }
    
    if ( sidebar ) {
        QWidget* viewport = sidebar->viewport();
        QPalette pal = viewport->palette();
        pal.setColor( viewport->backgroundRole(), QColor( Qt::transparent ) );
        viewport->setPalette( pal );
        sidebar->setFrameStyle( QFrame::NoFrame | QFrame::Plain );
        sidebar->setIconSize( QSize( 16, 16 ) );
    }
    
    // text codec
    mTextCodecEnabled = true;
    
    lCodec = new QLabel( tr( "Codec:" ), this );
    cbCodec = new QComboBox( this );
    cbCodec->addItems( pCoreUtils::textCodecs() );
    setTextCodec( QTextCodec::codecForLocale()->name() );
    
    glDialog->addWidget( lCodec, 4, 0 );
    glDialog->addWidget( cbCodec, 4, 1 );
    
    // read only
    mOpenReadOnlyEnabled = true;
    
    cbOpenReadOnly = new QCheckBox( tr( "Open in read only." ), this );
    
    glDialog->addWidget( cbOpenReadOnly, 5, 1 );
    
    // configuration
    setTextCodecEnabled( textCodecEnabled );
    setOpenReadOnlyEnabled( openReadOnlyEnabled );
}

QString pFileDialog::textCodec() const
{
    return cbCodec->currentText();
}

void pFileDialog::setTextCodec( const QString& codec )
{
    cbCodec->setCurrentIndex( cbCodec->findText( codec ) );
}

bool pFileDialog::textCodecEnabled() const
{
    return mTextCodecEnabled;
}

void pFileDialog::setTextCodecEnabled( bool enabled )
{
    mTextCodecEnabled = enabled;
    lCodec->setEnabled( enabled );
    lCodec->setVisible( enabled );
    cbCodec->setEnabled( enabled );
    cbCodec->setVisible( enabled );
}

bool pFileDialog::openReadOnly() const
{
    return cbOpenReadOnly->isChecked();
}

void pFileDialog::setOpenReadOnly( bool readOnly )
{
    cbOpenReadOnly->setChecked( readOnly );
}

bool pFileDialog::openReadOnlyEnabled() const
{
    return mOpenReadOnlyEnabled;
}

void pFileDialog::setOpenReadOnlyEnabled( bool enabled )
{
    mOpenReadOnlyEnabled = enabled;
    cbOpenReadOnly->setEnabled( enabled );
    cbOpenReadOnly->setVisible( enabled );
}

void pFileDialog::setDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, const QString& selectedFilter, QFileDialog::FileMode mode, QFileDialog::Options options )
{
#if defined( Q_OS_MAC ) && QT_VERSION < 0x040500
    if ( !options.testFlag( QFileDialog::DontUseSheet ) ) {
        // that's impossible to have a sheet in a sheet
        QWidget* parent = dlg->parentWidget();
        
        if ( parent && !parent->windowFlags().testFlag( Qt::Sheet ) ) {
            dlg->setWindowFlags( dlg->windowFlags() | Qt::Sheet );
        }
    }
#endif
    
    // dialog settings
    dlg->setWindowTitle( caption );
    dlg->setFileMode( mode );
    dlg->setDirectory( dir.isEmpty() ? QDir::currentPath() : dir );
    dlg->setTextCodecEnabled( enabledTextCodec );
    dlg->setOpenReadOnlyEnabled( enabledOpenReadOnly );
    dlg->setOptions( options );
    
    switch ( mode ) {
        case QFileDialog::Directory:
        case QFileDialog::DirectoryOnly:
        case QFileDialog::ExistingFile:
        case QFileDialog::ExistingFiles:
            dlg->setAcceptMode( AcceptOpen );
            break;
        case QFileDialog::AnyFile:
            dlg->setAcceptMode( AcceptSave );
            break;
    }
    
    // set filters if needed
    if ( !filter.isEmpty() ) {
        dlg->setNameFilter( filter );
    }
    
    // select file if needed )
    if ( !dir.isEmpty() ) {
        dlg->selectFile( dir );
    }
    
    // select correct filter if needed
    if ( !selectedFilter.isEmpty() ) {
        dlg->selectNameFilter( selectedFilter );
    }
}

pFileDialogResult pFileDialog::getExistingDirectory( QWidget* parent, const QString& caption, const QString& dir, bool enabledTextCodec, bool enabledOpenReadOnly, QFileDialog::Options options )
{
    pFileDialogResult result;
    pFileDialog fd( parent );
    setDialog( &fd, caption, dir, QString::null, enabledTextCodec, enabledOpenReadOnly, QString::null, QFileDialog::Directory, options );
    
    if ( fd.exec() == QDialog::Accepted ) {
        result[ pFileDialog::Directory ] = fd.selectedFiles().value( 0 );
        result[ pFileDialog::TextCodec ] = fd.textCodec();
        result[ pFileDialog::OpenReadOnly ] = fd.openReadOnly();
    }
    
    return result;
}

pFileDialogResult pFileDialog::getOpenFileName( QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, const QString& selectedFilter, QFileDialog::Options options )
{
    pFileDialogResult result;
    pFileDialog fd( parent );
    setDialog( &fd, caption, dir, filter, enabledTextCodec, enabledOpenReadOnly, selectedFilter, QFileDialog::ExistingFile, options );
    
    if ( fd.exec() == QDialog::Accepted ) {
        result[ pFileDialog::FileName ] = fd.selectedFiles().value( 0 );
        result[ pFileDialog::TextCodec ] = fd.textCodec();
        result[ pFileDialog::OpenReadOnly ] = fd.openReadOnly();
        result[ pFileDialog::SelectedFilter ] = fd.selectedNameFilter();
    }
    
    return result;
}

pFileDialogResult pFileDialog::getOpenFileNames( QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, const QString& selectedFilter, QFileDialog::Options options )
{
    pFileDialogResult result;
    pFileDialog fd( parent );
    setDialog( &fd, caption, dir, filter, enabledTextCodec, enabledOpenReadOnly, selectedFilter, QFileDialog::ExistingFiles, options );
    
    if ( fd.exec() == QDialog::Accepted ) {
        result[ pFileDialog::FileNames ] = fd.selectedFiles();
        result[ pFileDialog::TextCodec ] = fd.textCodec();
        result[ pFileDialog::OpenReadOnly ] = fd.openReadOnly();
        result[ pFileDialog::SelectedFilter ] = fd.selectedNameFilter();
    }
    
    return result;
}

pFileDialogResult pFileDialog::getSaveFileName( QWidget* parent, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, const QString& selectedFilter, QFileDialog::Options options )
{
    pFileDialogResult result;
    pFileDialog fd( parent );
    setDialog( &fd, caption, dir, filter, enabledTextCodec, false, selectedFilter, QFileDialog::AnyFile, options );
    
    if ( fd.exec() == QDialog::Accepted ) {
        result[ pFileDialog::FileName ] = fd.selectedFiles().value( 0 );
        result[ pFileDialog::TextCodec ] = fd.textCodec();
        result[ pFileDialog::OpenReadOnly ] = fd.openReadOnly();
        result[ pFileDialog::SelectedFilter ] = fd.selectedNameFilter();
    }
    
    return result;
}
