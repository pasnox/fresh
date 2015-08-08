/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pUpdateCheckerDialog.cpp
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
#include "pUpdateCheckerDialog.h"
#include "ui_pUpdateCheckerDialog.h"
#include "pUpdateChecker.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QPushButton>
#include <QDesktopServices>
#include <QDebug>

// pUpdateItem

pUpdateItem::pUpdateItem( pUpdateChecker* updateChecker, const QDomElement& element )
{
    mUpdateChecker = updateChecker;

    QDomNodeList nodes = element.childNodes();

    for ( int i = 0; i < nodes.count(); i++ ) {
        const QDomElement el = nodes.at( i ).toElement();
        const QString name = el.tagName();

        if ( name == QSL( "updated" ) ) {
            mDatas[ pUpdateItem::Updated ] = el.firstChild().toText().data();
        }
        else if ( name == QSL( "id" ) ) {
            mDatas[ pUpdateItem::Id ] = el.firstChild().toText().data();
        }
        else if ( name == QSL( "link" ) ) {
            mDatas[ pUpdateItem::Link ] = el.attribute( QSL( "href" ) );
        }
        else if ( name == QSL( "title" ) ) {
            mDatas[ pUpdateItem::Title ] = el.firstChild().toText().data().trimmed();
        }
        else if ( name == QSL( "author" ) ) {
            mDatas[ pUpdateItem::Author ] = el.firstChild().firstChild().toText().data();
        }
        else if ( name == QSL( "content" ) ) {
            mDatas[ pUpdateItem::Content ] = el.firstChild().toText().data().trimmed();
        }
    }
}

bool pUpdateItem::operator<( const pUpdateItem& other ) const
{
    return pVersion( version() ) < pVersion( other.version() );
}

bool pUpdateItem::operator>( const pUpdateItem& other ) const
{
    return pVersion( version() ) > pVersion( other.version() );
}

bool pUpdateItem::operator<( const pVersion& other ) const
{
    return pVersion( version() ) < other;
}

bool pUpdateItem::operator>( const pVersion& other ) const
{
    return pVersion( version() ) > other;
}

QDateTime pUpdateItem::updated() const
{
    return QDateTime::fromString( mDatas.value( pUpdateItem::Updated ), Qt::ISODate );
}

QString pUpdateItem::id() const
{
    return mDatas.value( pUpdateItem::Id );
}

QUrl pUpdateItem::link() const
{
    return QUrl( mDatas.value( pUpdateItem::Link ) );
}

QString pUpdateItem::title() const
{
    return mDatas.value( pUpdateItem::Title );
}

QString pUpdateItem::author() const
{
    return mDatas.value( pUpdateItem::Author );
}

QString pUpdateItem::content() const
{
    return mDatas.value( pUpdateItem::Content );
}

QString pUpdateItem::toolTip() const
{
    return content().replace(
        QRegExp( QSL( "<a.*</a>" ) ), pUpdateCheckerDialog::tr( QT_TRANSLATE_NOOP( "pUpdateCheckerDialog", "Updated on %1 by %2" ) )
            .arg( updated().toString( Qt::DefaultLocaleLongDate ) )
            .arg( author() )
    );
}

bool pUpdateItem::isFeatured() const
{
    return content().contains( QSL( "Featured" ), Qt::CaseInsensitive );
}

QString pUpdateItem::displayText() const
{
    return content().split( QSL( "\n" ) ).value( 1 ).trimmed().append( QSL( " ( " ) ).append( title() ).append( QSL( " ) " ) );
}

QString pUpdateItem::versionString() const
{
    if ( !mUpdateChecker ) {
        return QString::null;
    }

    const QString text = title();
    QRegExp rx( mUpdateChecker->versionDiscoveryPattern() );

    if ( rx.exactMatch( text ) ) {
        return rx.cap( 1 );
    }

    return QString::null;
}

pVersion pUpdateItem::version() const
{
    return pVersion( versionString() );
}

bool pUpdateItem::isValid() const
{
    return !mDatas.isEmpty();
}

// pUpdateCheckerDialog

pUpdateCheckerDialog::pUpdateCheckerDialog( pUpdateChecker* updateChecker, QWidget* parent )
    : QDialog( parent )
{
    Q_ASSERT( updateChecker );

    ui = new Ui_pUpdateCheckerDialog;
    mUpdateChecker = updateChecker;

    ui->setupUi( this );
    setAttribute( Qt::WA_DeleteOnClose );
    setAttribute( Qt::WA_MacSmallSize );
    ui->dbbButtons->button( QDialogButtonBox::Yes )->setEnabled( false );

    foreach ( QWidget* widget, findChildren<QWidget*>() ) {
        widget->setAttribute( Qt::WA_MacSmallSize );
    }

    mAccessManager = new QNetworkAccessManager( this );

    localeChanged();

    connect( mAccessManager, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( accessManager_finished( QNetworkReply* ) ) );

    mAccessManager->get( QNetworkRequest( mUpdateChecker->downloadsFeedUrl() ) );
}

bool pUpdateCheckerDialog::event( QEvent* event )
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

void pUpdateCheckerDialog::localeChanged()
{
    ui->retranslateUi( this );
    ui->lVersion->setText( tr( "You are using version <b>%1</b> (%2)." ).arg( mUpdateChecker->version() ).arg( mUpdateChecker->versionString() ) );
    ui->dbbButtons->button( QDialogButtonBox::Yes )->setText( tr( "Download" ) );
}

void pUpdateCheckerDialog::accessManager_finished( QNetworkReply* reply )
{
    localeChanged();

    const pVersion currentVersion( mUpdateChecker->version() );
    const QDateTime lastUpdated = mUpdateChecker->lastUpdated();
    //const QDateTime lastCheck = mUpdateChecker->lastChecked();

    if ( reply->error() != QNetworkReply::NoError ) {
        ui->lwVersions->addItem( new QListWidgetItem( tr( "An error occur: %1" ).arg( reply->errorString() ) ) );
    }
    else {
        QDomDocument document;

        if ( document.setContent( reply->readAll() ) ) {
            const QString updatedText = document.elementsByTagName( QSL( "updated" ) ).at( 0 ).firstChild().toText().data();
            const QDateTime updated = QDateTime::fromString( updatedText, Qt::ISODate );
            const QDomNodeList entries = document.elementsByTagName( QSL( "entry" ) );

            for ( int i = 0; i < entries.count(); i++ ) {
                const QDomElement element = entries.at( i ).toElement();

                const pUpdateItem updateItem( mUpdateChecker, element );

                if ( updateItem.isFeatured() && updateItem > currentVersion ) {
                    QListWidgetItem* item = new QListWidgetItem( updateItem.displayText() );

                    item->setToolTip( updateItem.toolTip() );
                    item->setData( Qt::UserRole, QVariant::fromValue( updateItem ) );
                    ui->lwVersions->addItem( item );
                }
            }

            mUpdateChecker->setLastUpdated( updated );

            if ( ui->lwVersions->count() > 0 ) {
                if ( !isVisible() && lastUpdated < updated ) {
                    open();
                }
            }
            else {
                QListWidgetItem* item = new QListWidgetItem( tr( "You are running the last available version." ) );

                item->setFlags( Qt::NoItemFlags );
                ui->lwVersions->addItem( item );

                if ( !isVisible() ) {
                    close();
                }
            }
        }
        else {
            ui->lwVersions->addItem( new QListWidgetItem( tr( "An error occur while parsing xml, retry later." ) ) );
        }
    }

    mUpdateChecker->setLastChecked( QDateTime::currentDateTime() );
}

void pUpdateCheckerDialog::on_lwVersions_itemSelectionChanged()
{
    QListWidgetItem* item = ui->lwVersions->selectedItems().value( 0 );
    const pUpdateItem updateItem = item ? item->data( Qt::UserRole ).value<pUpdateItem>() : pUpdateItem();

    ui->dbbButtons->button( QDialogButtonBox::Yes )->setEnabled( updateItem.isValid() );
}

void pUpdateCheckerDialog::on_lwVersions_itemDoubleClicked( QListWidgetItem* item )
{
    Q_UNUSED( item );
    ui->dbbButtons->button( QDialogButtonBox::Yes )->click();
}

void pUpdateCheckerDialog::accept()
{
    QListWidgetItem* item = ui->lwVersions->selectedItems().value( 0 );
    const pUpdateItem updateItem = item->data( Qt::UserRole ).value<pUpdateItem>();

    QDesktopServices::openUrl( updateItem.link() );
    QDialog::accept();
}
