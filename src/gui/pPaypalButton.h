#ifndef PPAYPALBUTTON_H
#define PPAYPALBUTTON_H

#include <QPushButton>
#include <QUrl>
#include <QHash>

#define PAYPAL_DOMAIN "https://www.paypal.com"
#define PAYPAL_MASK "https://www.paypal.com/%1/i/btn/btn_donate_LG.gif"

class pPaypalButton : public QPushButton
{
	Q_OBJECT
	
public:
	pPaypalButton( QWidget* parent = 0 );
	virtual ~pPaypalButton();
	
	virtual QSize minimumSizeHint() const;
	virtual QSize sizeHint() const;
	
	virtual bool event( QEvent* event );
	
	QString actionPost() const;
	QString businessId() const;
	QString itemName() const;
	QString itemId() const;
	QString currencyCode() const;
	bool autoOpenUrl() const;
	
	QPixmap pixmap( const QUrl& url ) const;
	QUrl url() const;
	
	static QUrl pixmapUrl( const QString& locale );

public slots:
	void updatePixmap();
	void setActionPost( const QString& value );
	void setBusinessId( const QString& value );
	void setItemName( const QString& value );
	void setItemId( const QString& value );
	void setCurrencyCode( const QString& value );
	void setAutoOpenUrl( bool open );

protected:
	QHash<QString, QString> mQueryItems;
	QPixmap mPixmap;
	bool mAutoOpenUrl;
	
	virtual void paintEvent( QPaintEvent* event );
	
	void localeChanged();

protected slots:
	void _q_clicked();
	void networkAccessManager_cached( const QUrl& url );
	void networkAccessManager_error( const QUrl& url, const QString& message );
	void networkAccessManager_cacheCleared();
};

#endif // PPAYPALBUTTON_H
