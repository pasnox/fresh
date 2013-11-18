/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pLocaleModel.cpp
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
#include "pLocaleModel.h"
#if defined( QT_GUI_LIB )
#include "pIconManager.h"
#endif

#include <QLocale>
#include <QDebug>

#define pLocaleModelColumnCount 1

bool caseInsensitiveLessThanStringLocale( const QString& left, const QString& right )
{
    return pLocaleModel::localeDisplayText( left ).toLower() < pLocaleModel::localeDisplayText( right ).toLower();
}

pLocaleModel::pLocaleModel( QObject* parent )
    : QAbstractItemModel( parent )
{
    mIsCheckable = false;

    populate();
}

QModelIndex pLocaleModel::index( int row, int column, const QModelIndex& parent ) const
{
    if ( parent.isValid() ) {
        const QString parentLocale = indexToLocale( parent );
        QStringList* list = mChildrenLocales.contains( parentLocale ) ? &mChildrenLocales[ parentLocale ] : 0;

        if ( !list || row < 0 || row >= list->count() || column < 0 || column >= pLocaleModelColumnCount ) {
            return QModelIndex();
        }

        return createIndex( row, column, &(*list)[ row ] );
    }

    if ( row < 0 || row >= mLocales.count() || column < 0 || column >= pLocaleModelColumnCount ) {
        return QModelIndex();
    }

    return createIndex( row, column, &mLocales[ row ] );
}

QModelIndex pLocaleModel::parent( const QModelIndex& child ) const
{
    const QLocale childLocale( indexToLocale( child ) );
    const QLocale parentLocale( childLocale.language() );
    return parentLocale.name() == childLocale.name() ? QModelIndex() : localeToIndex( parentLocale.name() );
}

int pLocaleModel::rowCount( const QModelIndex& parent ) const
{
    const QString locale = indexToLocale( parent );
    return parent == QModelIndex() ? mLocales.count() : mChildrenLocales.value( locale ).count();
}

int pLocaleModel::columnCount( const QModelIndex& parent ) const
{
    Q_UNUSED( parent );
    return pLocaleModelColumnCount;
}

QVariant pLocaleModel::data( const QModelIndex& index, int role ) const
{
    if ( index.isValid() ) {
        const QString name = indexToLocale( index );
        const QString countryCode = name.section( '_', 1 );

        switch ( role ) {
#if defined( QT_GUI_LIB )
            case Qt::DecorationRole:
                return pIconManager::icon( QString( "%1.png" ).arg( countryCode.toLower() ), ":/fresh/country-flags" );
#endif
            case Qt::DisplayRole:
                return pLocaleModel::localeDisplayText( name );
            case Qt::ToolTipRole:
                return name;
            case Qt::CheckStateRole: {
                if ( !mIsCheckable ) {
                    return QVariant();
                }

                Qt::CheckState state = Qt::CheckState( mData.value( name ).value( role, Qt::Unchecked ).toInt() );

                if ( state == Qt::Unchecked ) {
                    const QStringList locales = mChildrenLocales.value( name );
                    bool hasChildrenChecked = false;

                    foreach ( const QString& locale, locales ) {
                        if ( mData.value( locale ).value( Qt::CheckStateRole ).toInt() == Qt::Checked ) {
                            hasChildrenChecked = true;
                            break; // foreach
                        }
                    }

                    if ( hasChildrenChecked ) {
                        state = Qt::PartiallyChecked;
                    }
                }

                return state;
            }
            default:
                break;
        }

        return mData.value( name ).value( role );
    }

    return QVariant();
}

QVariant pLocaleModel::headerData( int section, Qt::Orientation orientation, int role ) const
{
    if ( section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
        return tr( "Locales" );
    }

    return QAbstractItemModel::headerData( section, orientation, role );
}

Qt::ItemFlags pLocaleModel::flags( const QModelIndex& index ) const
{
    Qt::ItemFlags flags = QAbstractItemModel::flags( index );

    if ( mIsCheckable ) {
        flags |= Qt::ItemIsUserCheckable;
    }

    return flags;
}

bool pLocaleModel::setData( const QModelIndex& index, const QVariant& value, int role )
{
    if ( !index.isValid() ) {
        return false;
    }

    switch ( role ) {
        case Qt::DecorationRole:
        case Qt::DisplayRole:
        case Qt::ToolTipRole:
            return false;
        default:
            break;
    }

    const QString name = indexToLocale( index );

    if ( !value.isNull() ) {
        mData[ name ][ role ] = value;
    }
    else if ( mData.value( name ).contains( role ) ) {
        mData[ name ].remove( role );

        if ( mData[ name ].isEmpty() ) {
            mData.remove( name );
        }
    }

    emit dataChanged( index, index );

    // update parent partially check state
    if ( role == Qt::CheckStateRole ) {
        const QModelIndex parent = index.parent();
        emit dataChanged( parent, parent );
    }

    return true;
}

QModelIndex pLocaleModel::localeToIndex( const QString& locale ) const
{
    int row = mLocales.indexOf( locale );

    if ( row != -1 ) {
        return createIndex( row, 0, &mLocales[ row ] );
    }

    const QString parentLocale = QLocale( QLocale( locale ).name().section( '_', 0, 0 ) ).name();
    row = mChildrenLocales.value( parentLocale ).indexOf( locale );
    QStringList* list = row != -1 ? &mChildrenLocales[ parentLocale ] : 0;

    if ( !list || row < 0 || row >= list->count() ) {
        return QModelIndex();
    }

    return createIndex( row, 0, &(*list)[ row ] );
}

QString pLocaleModel::indexToLocale( const QModelIndex& index ) const
{
    return index.isValid() ? *static_cast<QString*>( index.internalPointer() ) : QString::null;
}

bool pLocaleModel::isCheckable() const
{
    return mIsCheckable;
}

void pLocaleModel::setCheckable( bool checkable )
{
    mIsCheckable = checkable;
    emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
}

QStringList pLocaleModel::checkedLocales() const
{
    QStringList names;

    foreach ( const QString& name, mData.keys() ) {
        if ( mData[ name ].value( Qt::CheckStateRole ).toInt() == Qt::Checked ) {
            names << name;
        }
    }

    return names;
}

void pLocaleModel::setCheckedLocales( const QStringList& locales, bool checked )
{
    foreach ( QString name, locales ) {
        // fix not complet locale ( only language and not country )
        if ( name.count( "_" ) == 0 ) {
            name = QLocale( name ).name();
        }

        if ( checked ) {
            mData[ name ][ Qt::CheckStateRole ] = Qt::Checked;
        }
        else if ( mData.value( name ).contains( Qt::CheckStateRole ) ) {
            mData[ name ].remove( Qt::CheckStateRole );

            if ( mData[ name ].isEmpty() ) {
                mData.remove( name );
            }
        }
    }

    emit dataChanged( index( 0, 0 ), index( rowCount() -1, columnCount() -1 ) );
}

QString pLocaleModel::localeDisplayText( const QString& name )
{
    const QLocale locale( name );
    const QString language = QLocale::languageToString( locale.language() );
    const QString country = QLocale::countryToString( locale.country() );
    return QString( "%1 (%2)" ).arg( language ).arg( country );
}

void pLocaleModel::populate()
{
    QSet<QString> names;
    QHash<QString, QSet<QString> > childNames;

    for ( int i = QLocale::C +1; i < QLocale::LastLanguage; i++ ) {
        const QLocale::Language language = QLocale::Language( i );

        if ( language == QLocale::C ) {
            continue;
        }

        foreach ( const QLocale& locale, QLocale::matchingLocales( language, QLocale::AnyScript, QLocale::AnyCountry ) ) {
            const QString languageCode = locale.name().section( '_', 0, 0 );
            const QLocale localeParent( languageCode );
            const QString localeParentName = localeParent.name();

            names << localeParentName;

            if ( locale.name() != localeParent.name() ) {
                childNames[ localeParentName ] << locale.name();
            }
        }
    }

    mLocales = names.toList();
    qSort( mLocales.begin(), mLocales.end(), caseInsensitiveLessThanStringLocale );

    foreach ( const QString& name, childNames.keys() ) {
        QStringList locales = childNames[ name ].toList();
        qSort( locales.begin(), locales.end(), caseInsensitiveLessThanStringLocale );
        mChildrenLocales[ name ] = locales;
    }
}
