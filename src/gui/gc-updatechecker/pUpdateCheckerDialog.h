/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pUpdateCheckerDialog.h
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
#ifndef PUPDATECHECKERDIALOG_H
#define PUPDATECHECKERDIALOG_H

/*!
    \file pUpdateCheckerDialog.h
    \brief A update checker dialog.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
#include "pVersion.h"

#include <QDialog>
#include <QDomDocument>
#include <QDateTime>
#include <QHash>
#include <QMetaType>

class Ui_pUpdateCheckerDialog;
class pUpdateChecker;
class QNetworkAccessManager;
class QNetworkReply;
class QListWidgetItem;
class QUrl;

class FRESH_EXPORT pUpdateItem
{
public:
    enum Type {
        Updated,
        Id,
        Link,
        Title,
        Author,
        Content
    };
    
    pUpdateItem( pUpdateChecker* updateChecker = 0, const QDomElement& element = QDomElement() );
    
    bool operator<( const pUpdateItem& other ) const;
    bool operator>( const pUpdateItem& other ) const;
    bool operator<( const pVersion& other ) const;
    bool operator>( const pVersion& other ) const;
    
    QDateTime updated() const;
    QString id() const;
    QUrl link() const;
    QString title() const;
    QString author() const;
    QString content() const;
    QString toolTip() const;
    bool isFeatured() const;
    QString displayText() const;
    QString versionString() const;
    pVersion version() const;
    bool isValid() const;

protected:
    pUpdateChecker* mUpdateChecker;
    QHash<pUpdateItem::Type, QString> mDatas;
};

/*!
    \ingroup FreshGui
    \class pUpdateCheckerDialog
    \brief A update checker dialog.

    This dialog is used by pUpdateChecker.
*/
class FRESH_EXPORT pUpdateCheckerDialog : public QDialog
{
    Q_OBJECT

public:
    /*!
        Create an instance of pUpdateCheckerDialog for \a updateChecker having parent \a parent.
    */
    pUpdateCheckerDialog( pUpdateChecker* updateChecker, QWidget* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual bool event( QEvent* event );

protected:
    Ui_pUpdateCheckerDialog* ui;
    pUpdateChecker* mUpdateChecker;
    QNetworkAccessManager* mAccessManager;
    
    void localeChanged();

protected slots:
    void accessManager_finished( QNetworkReply* reply );
    void on_lwVersions_itemSelectionChanged();
    void on_lwVersions_itemDoubleClicked( QListWidgetItem* item );
    /*!
        Reimplemented.
    */
    virtual void accept();
};

Q_DECLARE_METATYPE( pUpdateItem )

#endif // PUPDATECHECKERDIALOG_H
