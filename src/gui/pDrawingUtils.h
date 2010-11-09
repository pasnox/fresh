#ifndef PDRAWINGUTILS_H
#define PDRAWINGUTILS_H

#include "core/FreshExport.h"

#include <QPixmap>

namespace pDrawingUtils
{
	FRESH_EXPORT QPixmap filledPixmap( const QColor& color, const QSize& size );
};

#endif // PDRAWINGUTILS_H
