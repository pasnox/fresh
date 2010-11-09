#ifndef PFILESYSTEMUTILS_H
#define PFILESYSTEMUTILS_H

#include "core/FreshExport.h"

#include <QDir>

namespace pFileSystemUtils
{
	FRESH_EXPORT QString findFile( QDir& dir, const QString& fileName, bool recursive = true );
};

#endif // PFILESYSTEMUTILS_H
