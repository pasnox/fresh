/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pUpdateChecker.h
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
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
#ifndef PUPDATECHECKER_H
#define PUPDATECHECKER_H

/*!
	\file pUpdateChecker.h
	\brief A class for checking available update for your application.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QObject>
#include <QWeakPointer>
#include <QDateTime>
#include <QUrl>

class QAction;

/*!
	\ingroup FreshGui
	\class pUpdateChecker
	\brief A class for checking available update for your application.
	
	A class for checking available update for your application based on a download feed.
	
	Currently only google code projects is supported.
*/
class FRESH_EXPORT pUpdateChecker : public QObject
{
	Q_OBJECT

public:
	/*!
		Create an instance of pUpdateChecker having parent \a parent.
	*/
	pUpdateChecker( QWidget* parent = 0 );
	/*!
		Return a QAction that can be put in menu to show the update checker dialog.
	*/
	QAction* menuAction() const;
	/*!
		Return the datetime of the update attribute of the downloads feed.
	*/
	QDateTime lastUpdated() const;
	/*!
		Return the last datetime the feed was checked.
	*/
	QDateTime lastChecked() const;
	/*!
		Return the url of the feed to download.
	*/
	QUrl downloadsFeedUrl() const;
	/*!
		The current version of the application.
	*/
	QString version() const;
	/*!
		The current version string of the application.
	*/
	QString versionString() const;
	/*!
		The regular expression pattern to detect version from releases file name.
	*/
	QString versionDiscoveryPattern() const;

public slots:
	/*!
		Set the datetime of the last feed's update.
	*/
	void setLastUpdated( const QDateTime& dateTime );
	/*!
		Set the datetime of the last time the feed was downloaded.
	*/
	void setLastChecked( const QDateTime& dateTime );
	/*!
		Set the url of the feed to download.
	*/
	void setDownloadsFeedUrl( const QUrl& url );
	/*!
		Set the application current version. (ie: 1.8.4.0a)
	*/
	void setVersion( const QString& version );
	/*!
		Set the application version string. (ie: 1.8.4.b Code Banana)
	*/
	void setVersionString( const QString& versionString );
	/*!
		Set the regular expression pattern to detect version from release file name. (ie: ".*qwbfsmanager-([0-9\\.]+).*" )
	*/
	void setVersionDiscoveryPattern( const QString& pattern );
	/*!
		Do a silent check in the background.
		
		If new versions are available a pUpdateCheckerDialog will be shown to the user.
	*/
	void silentCheck();

protected:
	QAction* mAction;
	QDateTime mLastUpdated;
	QDateTime mLastChecked;
	QUrl mDownloadsFeed;
	QString mVersion;
	QString mVersionString;
	QString mVersionDiscoveryPattern;
	QWeakPointer<QWidget> mParent;
	
	/*!
		Reimplemented.
	*/
	virtual bool eventFilter( QObject* object, QEvent* event );
	virtual void localeChanged();
	
protected slots:
	void checkForUpdate_triggered( bool show = true );
};

#endif // PUPDATECHECKER_H
