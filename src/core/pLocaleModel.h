/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pLocaleModel.h
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
#ifndef PLOCALEMODEL_H
#define PLOCALEMODEL_H

/*!
    \file pLocaleModel.h
    \brief A hierarchical model presenting all Qt QLocale.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QAbstractItemModel>
#include <QStringList>

/*!
    \ingroup FreshCore
    \class pLocaleModel
    \brief A hierarchical model presenting all Qt QLocale.

    This model provide a powerfull ready to use hierarchical model presenting
    all Qt QLocale with icons being their country flags.
*/
class FRESH_EXPORT pLocaleModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    /*!
        Construct a pLocaleModel having parent \a parent.
    */
    pLocaleModel( QObject* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    /*!
        Reimplemented.
    */
    virtual QModelIndex parent( const QModelIndex& child ) const;
    /*!
        Reimplemented.
    */
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    /*!
        Reimplemented.
    */
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    /*!
        Reimplemented.
    */
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    /*!
        Reimplemented.
    */
    virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
    /*!
        Reimplemented.
    */
    virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
    /*!
        Reimplemented.
    */
    virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
    /*!
        Return the model index for \a locale.
    */
    QModelIndex localeToIndex( const QString& locale ) const;
    /*!
        Return the locale string for \a index.
    */
    QString indexToLocale( const QModelIndex& index ) const;
    /*!
        Return true if the model is checkable, else false.
    */
    bool isCheckable() const;
    /*!
        Set the model checkable according to \a checkable.
    */
    void setCheckable( bool checkable );
    /*!
        Return the list of checked locale names.
    */
    QStringList checkedLocales() const;
    /*!
        Change the checked state of \a locales to \a checked.
    */
    void setCheckedLocales( const QStringList& locales, bool checked = true );
    /*!
        Return the display text for the locale \a name.
    */
    static QString localeDisplayText( const QString& name );

protected:
    typedef QHash<int, QVariant> IntVariantMap;
    mutable QStringList mLocales; // top level locales
    mutable QHash<QString, QStringList> mChildrenLocales; // children locales of a top level locale
    QHash<QString, pLocaleModel::IntVariantMap> mData;
    bool mIsCheckable;

    void populate();

signals:
    void localeCheckStateChanged( const QString& locale );
};

#endif // PLOCALEMODEL_H
