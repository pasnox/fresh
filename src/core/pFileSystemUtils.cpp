#include "pFileSystemUtils.h"

#include <QMap>
#include <QTextCodec>

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
