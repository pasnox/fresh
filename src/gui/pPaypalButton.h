#ifndef PPAYPALBUTTON_H
#define PPAYPALBUTTON_H

#include <QLabel>
#include <QUrl>
#include <QHash>

class pPaypalButton : public QLabel
{
	Q_OBJECT
	
public:
	pPaypalButton( QWidget* parent = 0 );
	virtual ~pPaypalButton();
	
	virtual bool event( QEvent* event );
	
	QString actionPost() const;
	QString businessId() const;
	QString itemName() const;
	QString itemId() const;
	QString currencyCode() const;
	
	QPixmap pixmap( const QUrl& url ) const;
	QUrl url() const;
	
	static QUrl pixmapUrl( const QString& locale );

public slots:
	void setActionPost( const QString& value );
	void setBusinessId( const QString& value );
	void setItemName( const QString& value );
	void setItemId( const QString& value );
	void setCurrencyCode( const QString& value );

protected:
	QHash<QString, QString> mQueryItems;
	
	virtual void mousePressEvent( QMouseEvent* event );
	
	void localeChanged();
	void updatePixmap();

protected slots:
	void networkAccessManager_cached( const QUrl& url );
	void networkAccessManager_error( const QUrl& url, const QString& message );
	void networkAccessManager_cacheCleared();

signals:
	void clicked();
};

#endif // PPAYPALBUTTON_H
