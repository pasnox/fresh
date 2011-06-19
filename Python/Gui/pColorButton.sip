class pColorButton : QToolButton
{
%TypeHeaderCode
#include <gui/pColorButton.h>
%End

public:
	pColorButton( QWidget* parent = 0 );
	pColorButton( const QColor& color, QWidget* parent = 0 );
	
	const QColor& color() const;

protected:
	void init( const QColor& color );

public slots:
	void setColor( const QColor& color );

protected slots:
	void _q_clicked();

signals:
	void colorChanged( const QColor& color );
};
