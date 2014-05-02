/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pPaypalButton.h
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
#ifndef PPAYPALBUTTON_H
#define PPAYPALBUTTON_H

/*!
    \file pPaypalButton.h
    \brief A custom button providing paypal donation.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include <QPushButton>
#include <QUrl>
#include <QHash>

#define PAYPAL_DOMAIN "https://www.paypal.com"
#define PAYPAL_MASK "https://www.paypal.com/%1/i/btn/btn_donate_LG.gif"

/*!
    \ingroup FreshGui
    \class pPaypalButton
    \brief A custom button providing paypal donation.

    This button allow your application to easily receives paypal donations.
    
    The network traffic is handled by the global instance of pNetworkAccessManager.
    
    \sa pNetworkAccessManager::instance().
*/
class pPaypalButton : public QPushButton
{
    Q_OBJECT
    
public:
    /*!
        Create an instance of button having \a parent as parent.
    */
    pPaypalButton( QWidget* parent = 0 );
    /*!
        Reimplemented.
    */
    virtual QSize minimumSizeHint() const;
    /*!
        Reimplemented.
    */
    virtual QSize sizeHint() const;
    /*!
        Reimplemented.
    */
    virtual bool event( QEvent* event );
    /*!
        Return the http post action url.
    */
    QString actionPost() const;
    /*!
        Return your donation item business id.
    */
    QString businessId() const;
    /*!
        Return your donation item name.
    */
    QString itemName() const;
    /*!
        Return your donation item id.
    */
    QString itemId() const;
    /*!
        Return the currency used by your donation item.
    */
    QString currencyCode() const;
    /*!
        Return true if the http request should be automatically opened using QDesktopServices::openUrl() else false.
    */
    bool autoOpenUrl() const;
    /*!
        Return the final generated url that will be opened when clicking the button.
    */
    QUrl url() const;
    /*!
        Return the pixmap url for \a locale.
    */
    static QUrl pixmapUrl( const QString& locale );

public slots:
    /*!
        Set the http post action to \a url.
    */
    void setActionPost( const QString& value );
    /*!
        Set the donation item business id to \a value.
    */
    void setBusinessId( const QString& value );
    /*!
        Set the donation item name to \a value.
    */
    void setItemName( const QString& value );
    /*!
        Set the donation item id to \a value.
    */
    void setItemId( const QString& value );
    /*!
        Set the donation item currency code to \a value..
    */
    void setCurrencyCode( const QString& value );
    /*!
        Set if the button should automatically open url() when the user click on the button.
        Give true to automatically open it, else give false and handle the clicked( const QUrl& ) signal.
    */
    void setAutoOpenUrl( bool open );

protected:
    QHash<QString, QString> mQueryItems;
    QPixmap mPixmap;
    bool mAutoOpenUrl;
    
    QPixmap pixmap( const QUrl& url ) const;
    /*!
        Reimplemented.
    */
    virtual void paintEvent( QPaintEvent* event );
    
    void localeChanged();

protected slots:
    void updatePixmap();
    void _q_clicked();
    void networkAccessManager_cached( const QUrl& url );
    void networkAccessManager_error( const QUrl& url, const QString& message );
    void networkAccessManager_cacheCleared();

signals:
    /*!
        This signal is emit when the user click the button and that autoOpenUrl() is false.
    */
    void clicked( const QUrl& url );
};

#endif // PPAYPALBUTTON_H
