#include "pColorButton.h"
#include "pDrawingUtils.h"

#include <QColorDialog>

pColorButton::pColorButton( QWidget* parent )
	: QToolButton( parent )
{
	init( QColor( Qt::black ) );
}

pColorButton::pColorButton( const QColor& color, QWidget* parent )
	: QToolButton( parent )
{
	init( color );
}

void pColorButton::init( const QColor& color )
{
	connect( this, SIGNAL( clicked() ), this, SLOT( _q_clicked() ) );
	setIconSize( QSize( 32, 32 ) );
	setColor( color );
}

const QColor& pColorButton::color() const
{
	return mColor;
}

void pColorButton::setColor( const QColor& color )
{
	mColor = color;
	
	const QStringList texts = QStringList()
		<< QString( "RGBA #%1%2%3%4" ).arg( mColor.red(), 2, 16, QChar( '0' ) ).arg( mColor.green(), 2, 16, QChar( '0' ) ).arg( mColor.blue(), 2, 16, QChar( '0' ) ).arg( mColor.alpha(), 2, 16, QChar( '0' ) )
		<< QString( "RGBA %1, %2, %3, %4" ).arg( mColor.red() ).arg( mColor.green() ).arg( mColor.blue() ).arg( mColor.alpha() )
		;
	
	setText( texts.first() );
	setToolTip( texts.join( "\n" ) );
	
	setIcon( QIcon( pDrawingUtils::filledPixmap( mColor, iconSize() ) ) );
	
	emit colorChanged( mColor );
}

void pColorButton::_q_clicked()
{
	const QColor color = QColorDialog::getColor( mColor, window(), tr( "Choose a color" ), QColorDialog::ShowAlphaChannel );
	
	if ( color.isValid() ) {
		setColor( color );
	}
}
