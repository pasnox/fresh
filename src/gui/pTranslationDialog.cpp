/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pTranslationDialog.cpp
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
#include "pTranslationDialog.h"
#include "ui_pTranslationDialog.h"
#include "pTranslationManager.h"
#include "pIconManager.h"
#include "listeditor/pPathListEditor.h"

#include <QFileDialog>
#include <QDebug>

pTranslationDialog::pTranslationDialog( QWidget* parent )
	: QDialog( parent ), ui( 0 )
{
	init( 0 );
}

pTranslationDialog::pTranslationDialog( pTranslationManager* translationManager, QWidget* parent )
	: QDialog( parent ), ui( 0 )
{
	init( translationManager );
}

pTranslationDialog::~pTranslationDialog()
{
	delete ui;
}

bool pTranslationDialog::event( QEvent* event )
{
	switch ( event->type() ) {
		case QEvent::LocaleChange:
			localeChanged();
			break;
		default:
			break;
	}
	
	return QDialog::event( event );
}

pTranslationManager* pTranslationDialog::translationManager() const
{
	return mTranslationManager;
}

void pTranslationDialog::setTranslationManager( pTranslationManager* translationManager )
{
	init( translationManager );
}

QString pTranslationDialog::selectedLocale() const
{
	const QTreeWidgetItem* currentItem = ui->twLocales->selectedItems().value( 0 );
	return currentItem ? currentItem->data( 0, Qt::UserRole ).toString() : ( mTranslationManager ? mTranslationManager->currentLocale().name() : QString::null );
}

QString pTranslationDialog::getLocale( pTranslationManager* translationManager, QWidget* parent )
{
	pTranslationDialog dlg( translationManager, parent );
	
	if ( dlg.exec() == QDialog::Accepted ) {
		return dlg.selectedLocale();
	}
	
	return QString::null;
}

void pTranslationDialog::localeChanged()
{
	ui->retranslateUi( this );
}

void pTranslationDialog::on_tbLocate_clicked()
{
	if ( !mTranslationManager ) {
		return;
	}
	
	QDialog dlg( this );
	pPathListEditor* editor = new pPathListEditor( tr( "Choose folders containing your application translations" ), QApplication::applicationDirPath(), &dlg );
	QDialogButtonBox* buttons = new QDialogButtonBox( &dlg );
	QVBoxLayout* vl = new QVBoxLayout( &dlg );
	vl->addWidget( editor );
	vl->addWidget( buttons );
	
	buttons->setStandardButtons( QDialogButtonBox::Cancel | QDialogButtonBox::Ok );
	editor->setValues( mTranslationManager->translationsPaths() );
	
	connect( buttons, SIGNAL( rejected() ), &dlg, SLOT( reject() ) );
	connect( buttons, SIGNAL( accepted() ), &dlg, SLOT( accept() ) );
	
	if ( dlg.exec() == QDialog::Rejected ) {
		return;
	}
	
	mTranslationManager->setTranslationsPaths( editor->values() );
	ui->tbReload->click();
}

void pTranslationDialog::init( pTranslationManager* translationManager )
{
	if ( mTranslationManager == translationManager ) {
		return;
	}
	
	mTranslationManager = translationManager;
	mOriginalLocale = mTranslationManager ? mTranslationManager->currentLocale().name() : locale().name();
	
	if ( !ui ) {
		ui = new Ui::pTranslationDialog;
		ui->setupUi( this );
	}
	
	ui->tbReload->click();
	localeChanged();
}

QTreeWidgetItem* pTranslationDialog::newItem( const QLocale& locale )
{
	const QString language = QLocale::languageToString( locale.language() );
	const QString country = QLocale::countryToString( locale.country() );
	const QString countryCode = locale.name().section( '_', 1 );
	
	QTreeWidgetItem* item = new QTreeWidgetItem;
	item->setIcon( 0, pIconManager::icon( QString( "%1.png" ).arg( countryCode.toLower() ), ":/fresh/country-flags" ) );
	item->setText( 0, QString( "%1 (%2)" ).arg( language ).arg( country ) );
	item->setToolTip( 0, locale.name() );
	item->setData( 0, Qt::UserRole, locale.name() );
	return item;
}

QTreeWidgetItem* pTranslationDialog::rootItem( const QLocale& locale )
{
	QTreeWidgetItem* item = mRootItems.value( locale.name() );
	
	if ( !item ) {
		item = newItem( locale );
		ui->twLocales->addTopLevelItem( item );
		mRootItems[ locale.name() ] = item;
	}
	
	return item;
}

void pTranslationDialog::on_tbReload_clicked()
{
	// reload translations if needed
	if ( mTranslationManager && mTranslationManager->availableLocales().isEmpty() ) {
		mTranslationManager->reloadTranslations();
	}
	
	// keep current locale
	const QString currentLocale = selectedLocale();
	
	// clear items
	ui->twLocales->clear();
	mRootItems.clear();
	
	// create new ones
	if ( mTranslationManager ) {
		foreach ( const QLocale& _locale, mTranslationManager->availableQLocales() ) {
			const QLocale locale = _locale.language() == QLocale::C ? QLocale( QLocale::English ) : _locale;
			QTreeWidgetItem* rootItem = this->rootItem( QLocale( locale.language() ) );
			
			if ( rootItem->data( 0, Qt::UserRole ).toString() == locale.name() ) {
				continue;
			}
			
			rootItem->addChild( newItem( locale ) );
		}
	}
	
	// sort items
	ui->twLocales->sortByColumn( 0, Qt::AscendingOrder );
	
	// restore locale
	QAbstractItemModel* model = ui->twLocales->model();
	QModelIndex index = model->match( model->index( 0, 0 ), Qt::UserRole, currentLocale, 1, Qt::MatchFixedString | Qt::MatchWrap ).value( 0 );
	
	if ( !index.isValid() ) {
		index = model->match( model->index( 0, 0 ), Qt::UserRole, currentLocale, 1, Qt::MatchStartsWith | Qt::MatchWrap ).value( 0 );
	}
	
	ui->twLocales->setCurrentIndex( index );
}

void pTranslationDialog::on_twLocales_itemSelectionChanged()
{
	if ( mTranslationManager ) {
		mTranslationManager->setCurrentLocale( selectedLocale() );
		mTranslationManager->reloadTranslations();
		setLocale( selectedLocale() );
	}
}

void pTranslationDialog::reject()
{
	if ( mTranslationManager ) {
		mTranslationManager->setCurrentLocale( mOriginalLocale );
		mTranslationManager->reloadTranslations();
	}
	
	QDialog::reject();
}

void pTranslationDialog::accept()
{
	QDialog::accept();
}
