#ifndef PCOLORBUTTON_H
#define PCOLORBUTTON_H

#include "core/FreshExport.h"

#include <QToolButton>

class FRESH_EXPORT pColorButton : public QToolButton
{
	Q_OBJECT
	Q_PROPERTY( QColor color READ color WRITE setColor NOTIFY colorChanged )
	
public:
	pColorButton( QWidget* parent = 0 );
	pColorButton( const QColor& color, QWidget* parent = 0 );
	
	const QColor& color() const;

protected:
	QColor mColor;
	
	void init( const QColor& color );

public slots:
	void setColor( const QColor& color );

protected slots:
	void _q_clicked();

signals:
	void colorChanged( const QColor& color );
};

#endif // PCOLORBUTTON_H
