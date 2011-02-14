#include "pFileSystemUtils.h"

#include <QMap>
#include <QHash>
#include <QTextCodec>
#include <QLocale>
#include <QDebug>

QString pFileSystemUtils::findFile( QDir& dir, const QString& fileName, bool recursive )
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

QStringList pFileSystemUtils::findFiles( QDir& dir, const QStringList& filters, bool recursive )
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

bool pFileSystemUtils::isEmptyDirectory( const QString& path )
{
	return QFileInfo( path ).isDir()
		? QDir( path ).entryList( QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot ).isEmpty()
		: false
		;
}

QStringList pFileSystemUtils::findFiles( QDir& dir, const QString& filter, bool recursive )
{
	return findFiles( dir, QStringList( filter ), recursive );
}

QStringList pFileSystemUtils::textCodecs()
{
	QMap<QString, QString> codecs;
	
	foreach ( const QByteArray& codec, QTextCodec::availableCodecs() ) {
		const QString entry = QString::fromAscii( codec );
		codecs[ entry.toLower() ] = entry;
	}
	
	return codecs.values();
}

QString pFileSystemUtils::toTitleCase( const QString& _string )
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

uint pFileSystemUtils::qHash( const QLocale& locale )
{
	return qHash( locale.name() );
}

#if QT_VERSION < 0x040700
uint pFileSystemUtils::qHash( const QUrl& url )
{
	return qHash( url.toString() );
}
#endif
