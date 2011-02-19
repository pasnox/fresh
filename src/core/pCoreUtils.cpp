/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pCoreUtils.cpp
** Date      : 2011-02-20T00:44:21
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
#include "pCoreUtils.h"

#include <QMap>
#include <QHash>
#include <QTextCodec>
#include <QLocale>
#include <QDebug>

QString pCoreUtils::findFile( QDir& dir, const QString& fileName, bool recursive )
{
	foreach ( const QFileInfo& fi, dir.entryInfoList( QStringList( fileName ) ) ) {
		/*if ( fi.fileName() == fileName )*/ {
			return QDir::cleanPath( fi.absoluteFilePath() );
		}
	}

	if ( recursive ) {
		foreach ( const QFileInfo& fi, dir.entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot ) ) {
			dir.cd( fi.fileName() );
			const QString fn = findFile( dir, fileName, true );
			dir.cdUp();
			
			if ( !fn.isNull() ) {
				return fn;
			}
		}
	}

	return QString::null;
}

QStringList pCoreUtils::findFiles( QDir& dir, const QStringList& filters, bool recursive )
{
	QStringList files;
	
	foreach ( const QFileInfo& fi, dir.entryInfoList( filters ) ) {
		files << QDir::cleanPath( fi.absoluteFilePath() );
	}

	if ( recursive ) {
		foreach ( const QFileInfo& fi, dir.entryInfoList( QDir::AllDirs | QDir::NoDotAndDotDot ) ) {
			dir.cd( fi.fileName() );
			files << findFiles( dir, filters, true );
			dir.cdUp();
		}
	}
	
	return files;
}

bool pCoreUtils::isEmptyDirectory( const QString& path )
{
	return QFileInfo( path ).isDir()
		? QDir( path ).entryList( QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot ).isEmpty()
		: false
		;
}

QStringList pCoreUtils::findFiles( QDir& dir, const QString& filter, bool recursive )
{
	return findFiles( dir, QStringList( filter ), recursive );
}

QStringList pCoreUtils::textCodecs()
{
	QMap<QString, QString> codecs;
	
	foreach ( const QByteArray& codec, QTextCodec::availableCodecs() ) {
		const QString entry = QString::fromAscii( codec );
		codecs[ entry.toLower() ] = entry;
	}
	
	return codecs.values();
}

QString pCoreUtils::toTitleCase( const QString& _string )
{
	QString string = _string;
	bool lastWasLetter = false;
	
	for ( int i = 0; i < string.length(); i++ ) {
		const QChar c = string.at( i );
		
		if ( c.isLetter() || c == '?' ) {
			if ( lastWasLetter ) {
				string[ i ] = c.toLower();
			}
			else {
				string[ i ] = c.toTitleCase();
			}
			
			lastWasLetter = true;
		}
		else if ( c != '\'' ) {
			lastWasLetter = false;
		}
	}
	
	return string;
}

uint pCoreUtils::qHash( const QLocale& locale )
{
	return qHash( locale.name() );
}

#if QT_VERSION < 0x040700
uint pCoreUtils::qHash( const QUrl& url )
{
	return qHash( url.toString() );
}
#endif
