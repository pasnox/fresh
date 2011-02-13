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
	\ingroup Gui
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
