#ifndef PUPDATECHECKERDIALOG_H
#define PUPDATECHECKERDIALOG_H

#include "core/FreshExport.h"
#include "ui_pUpdateCheckerDialog.h"
#include "core/pVersion.h"

#include <QDomDocument>
#include <QDateTime>

class QNetworkAccessManager;
class QNetworkReply;
class pUpdateChecker;

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

class FRESH_EXPORT pUpdateCheckerDialog : public QDialog, public Ui::pUpdateCheckerDialog
{
	Q_OBJECT

public:
	pUpdateCheckerDialog( pUpdateChecker* updateChecker, QWidget* parent = 0 );
	
	virtual bool event( QEvent* event );

protected:
	pUpdateChecker* mUpdateChecker;
	QNetworkAccessManager* mAccessManager;
	
	void localeChanged();

protected slots:
	void accessManager_finished( QNetworkReply* reply );
	void on_lwVersions_itemSelectionChanged();
	void on_lwVersions_itemDoubleClicked( QListWidgetItem* item );
	virtual void accept();
};

Q_DECLARE_METATYPE( pUpdateItem )

#endif // PUPDATECHECKERDIALOG_H
