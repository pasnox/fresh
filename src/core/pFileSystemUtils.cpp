#include "pFileSystemUtils.h"

#include <QMap>
#include <QHash>
#include <QTextCodec>
#include <QLocale>

QString pFileSystemUtils::findFile( QDir& dir, const QString& fileName, bool recursive )
{
	foreach ( const QFileInfo& fi, dir.entryInfoList( QStringList( fileName ), QDir::Files | QDir::CaseSensitive ) ) {
		if ( fi.fileName() == fileName ) {
			return fi.canonicalFilePath();
		}
	}

	if ( recursive ) {
		foreach ( const QFileInfo& fi, dir.entryInfoList( QDir::AllDirs ) ) {
			dir.setPath( fi.absoluteFilePath() );
			const QString fn = findFile( dir, fileName, true );

			if ( !fn.isNull() ) {
				return fn;
			}
		}
	}

	return QString::null;
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
