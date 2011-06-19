class pUpdateItem
{
%TypeHeaderCode
#include <gui/gc-updatechecker/pUpdateCheckerDialog.h>
%End

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
};

class pUpdateCheckerDialog : QDialog
{
%TypeHeaderCode
#include <gui/gc-updatechecker/pUpdateCheckerDialog.h>
%End

public:
	pUpdateCheckerDialog( pUpdateChecker* updateChecker, QWidget* parent = 0 );
	
	virtual bool event( QEvent* event );

protected:
	void localeChanged();

protected slots:
	void accessManager_finished( QNetworkReply* reply );
	void on_lwVersions_itemSelectionChanged();
	void on_lwVersions_itemDoubleClicked( QListWidgetItem* item );
	virtual void accept();
};
