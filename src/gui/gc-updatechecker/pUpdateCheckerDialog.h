#ifndef PUPDATECHECKERDIALOG_H
#define PUPDATECHECKERDIALOG_H

/*!
	\file pUpdateCheckerDialog.h
	\brief A update checker dialog.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"
#include "core/pVersion.h"

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
	\ingroup Gui
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
