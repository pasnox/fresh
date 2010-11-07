#ifndef FILESYSTEMUTILS_H
#define FILESYSTEMUTILS_H

#include "core/FreshExport.h"

#include <QDir>

namespace FileSystemUtils
{
	FRESH_EXPORT QString findFile( QDir& dir, const QString& fileName, bool recursive = true );
};

#endif // FILESYSTEMUTILS_H
