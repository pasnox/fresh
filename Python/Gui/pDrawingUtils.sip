namespace pDrawingUtils
{
%TypeHeaderCode
#include <gui/pDrawingUtils.h>
%End

	QPixmap filledPixmap( const QColor& color, const QSize& size );
	QPixmap scaledPixmap( const QString& filePath, const QSize& size = QSize() );
	QPixmap scaledPixmap( const QPixmap& pixmap, const QString& key, const QSize& size = QSize() );
};
