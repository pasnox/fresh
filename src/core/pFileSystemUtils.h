#ifndef PFILESYSTEMUTILS_H
#define PFILESYSTEMUTILS_H

#include "core/FreshExport.h"

#include <QDir>

namespace pFileSystemUtils
{
	FRESH_EXPORT QString findFile( QDir& dir, const QString& fileName, bool recursive = true );
	FRESH_EXPORT QStringList textCodecs();
};

#endif // PFILESYSTEMUTILS_H
