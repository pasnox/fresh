#include "pToolButton.h"

#include <QPaintEvent>
#include <QMouseEvent>
#include <QStyleOptionToolButton>
#include <QToolBar>
#include <QMenu>
#include <QPainter>
#include <QApplication>

pToolButton::pToolButton( QWidget* parent, QBoxLayout::Direction direction )
	: QToolButton( parent )
{
	mMenuDown = false;

	setDirection( direction );
}

void pToolButton::paintEvent( QPaintEvent* event )
{
	Q_UNUSED( event );
	// calcul angle rotation
	const QSize size = QToolButton::sizeHint();
	int rotation = 0;
	QPoint pos = QPoint();
	
	switch ( mDirection ) {
		case QBoxLayout::TopToBottom:
			rotation = 90;
			pos = QPoint( 0, -size.height() );
			break;
		case QBoxLayout::BottomToTop:
			rotation = -90;
			pos = QPoint( -size.width(), 0 );
			break;
		default:
			break;
	}
	
	// get style options
	QStyleOptionToolButton option;
	initStyleOption( &option );

	// fix backport bugs :|
	switch ( cursorArea() ) {
		case pToolButton::caArrow:
		case pToolButton::caButton:
		case pToolButton::caArrowClicked:
			break;
		case pToolButton::caButtonClicked:
			option.activeSubControls |= QStyle::SC_ToolButton;
			
			if ( popupMode() == QToolButton::MenuButtonPopup ) {
				option.state |= QStyle::State_MouseOver;
				option.activeSubControls |= QStyle::SC_ToolButtonMenu;
			}
			
			break;
		case pToolButton::caNone:
		default:
			break;
	}
	
	// force to do horizontal paint
	option.rect.setSize( size );
	option.rect.moveTopLeft( pos );
	
	// don't paint icon and text as it's not working for all rotations
	option.icon = QIcon();
	option.text = QString::null;
	
	// calcule rects
	const int margin = 2;
	const int spacing = 1;
	const int effect = isDown() || isChecked() ? spacing : 0;
	QRect iconRect;
	QRect textRect;

	QPainter painter( this );
	painter.rotate( rotation );
	
	// draw button
	style()->drawComplexControl( QStyle::CC_ToolButton, &option, &painter, this );
	
	// draw icon
	if ( !icon().isNull() ) {
		const QIcon::State state = isChecked() ? QIcon::On : QIcon::Off;
		QIcon::Mode mode = isEnabled() ? QIcon::Normal : QIcon::Disabled;
		
		if ( isEnabled() ) {
			if ( hasFocus() ) {
				mode = QIcon::Selected;
			}
			
			if ( isDown() ) {
				mode = QIcon::Active;
			}
		}
		
		iconRect = QRect( pos, iconSize() +QSize( margin *2, margin *2 ) );
		
		if ( iconRect.width() > size.width() ) {
			iconRect.setWidth( size.width() );
		}
		
		if ( iconRect.height() > size.height() ) {
			iconRect.setHeight( size.height() );
		}
		
		iconRect = iconRect.adjusted( margin, margin, margin, -margin );
		iconRect.moveCenter( QPoint( iconRect.center().x(), pos.y() +( size.height() /2 ) -1 ) );
		iconRect = iconRect.adjusted( effect, effect, effect, effect );
		
		icon().paint( &painter, iconRect, Qt::AlignCenter, mode, state );
	}
	
	// draw text
	if ( !text().isEmpty() ) {
		const QPalette::ColorRole role = QPalette::ButtonText;
		QPalette::ColorGroup group = isEnabled() ? QPalette::Normal : QPalette::Disabled;
		
		if ( isEnabled() ) {
			if ( isActiveWindow() && hasFocus() ) {
				group = QPalette::Active;
			}
			else if ( !isActiveWindow() ) {
				group = QPalette::Inactive;
			}
		}
		
		textRect = QRect( pos, size ).adjusted( iconRect.width() +margin, margin, -margin, -margin );
		textRect.moveCenter( QPoint( textRect.center().x(), pos.y() +( size.height() /2 ) ) );
		textRect = textRect.adjusted( effect, effect, effect, effect );
		
		const int flags = style()->visualAlignment( layoutDirection(), Qt::AlignCenter );
		const QString text = this->text();
		
		painter.setPen( palette().color( group, role ) );
		painter.drawText( textRect, flags, text );
	}
	
	painter.rotate( -rotation );
}

void pToolButton::mousePressEvent( QMouseEvent* event )
{
	switch ( cursorArea( event->pos() ) ) {
		case pToolButton::caArrowClicked:
			mMenuDown = true;
			showMenu();
			mMenuDown = false;
			break;
		case pToolButton::caButtonClicked:
			setDown( !isDown() );
			break;
		case pToolButton::caNone:
			break;
		default:
			QAbstractButton::mousePressEvent( event );
			break;
	}
	
	// update button
	update();
}

void pToolButton::mouseMoveEvent( QMouseEvent* event )
{
	QAbstractButton::mouseMoveEvent( event );

	if ( event->buttons() != Qt::NoButton ) {
		setDown( hitButton( event->pos() ) );
	}
}

void pToolButton::mouseReleaseEvent( QMouseEvent* event )
{
	mMenuDown = false;

	switch ( cursorArea( event->pos() ) ) {
		case pToolButton::caButton:
			click();
			break;
		case pToolButton::caArrow:
		case pToolButton::caNone:
			break;
		default:
			QAbstractButton::mouseReleaseEvent( event );
			break;
	}

	// update button
	update();
}

pToolButton::CursorArea pToolButton::cursorArea( const QPoint& _pos ) const
{
	// cursor pos
	const QPoint pos = _pos.isNull() ? mapFromGlobal( QCursor::pos() ) : _pos;

	// if not contain is button return none
	if ( !hitButton( pos ) ) {
		return pToolButton::caNone;
	}

	// is arrow type
	bool arrowType = popupMode() == QToolButton::MenuButtonPopup;

	// is mouse pressed ?!
	bool mousePressed = QApplication::mouseButtons() & Qt::LeftButton;

	// check if we are a arrow button
	if ( arrowType ) {
		// get bounding rectangle
		QRect rect = this->rect();
	
		// get style options
		QStyleOptionToolButton opt;
		initStyleOption( &opt );
	
		// force to do horizontal calcul
		opt.rect.setSize( QToolButton::sizeHint() );

		// get arraow bounding rectangle
		QSize size = style()->subControlRect( QStyle::CC_ToolButton, &opt, QStyle::SC_ToolButtonMenu, this ).size();

		switch ( mDirection ) {
			case QBoxLayout::BottomToTop:
				size.transpose();
				break;
			case QBoxLayout::TopToBottom:
				size.transpose();
				rect.setY( rect.height() -size.height() );
				break;
			default:
				rect.setX( rect.width() -size.width() );
				break;
		}
	
		// get valid bounding rectangle size
		rect.setSize( size );

		// in arrow bounding rect
		if ( rect.isValid() && rect.contains( pos ) ) {
			return mousePressed ? pToolButton::caArrowClicked : pToolButton::caArrow;
		}
	}

	// in button
	return mousePressed ? pToolButton::caButtonClicked : pToolButton::caButton;
}

QMenu* pToolButton::hasMenu() const
{
	QMenu* menu = this->menu();
	
	if ( !menu && defaultAction() ) {
		menu = defaultAction()->menu();
	}
	
	return menu;
}

bool pToolButton::menuButtonDown() const
{
#ifndef QT_NO_MENU
	return hasMenu() && mMenuDown;
#else
	return false;
#endif
}

QSize pToolButton::minimumSizeHint() const
{
	return QSize();
}

QSize pToolButton::sizeHint() const
{
	//get default size
	QSize size = QToolButton::sizeHint();

	// calcul new size hint
	switch ( mDirection ) {
		case QBoxLayout::LeftToRight:
		case QBoxLayout::RightToLeft:
			break;
		case QBoxLayout::TopToBottom:
		case QBoxLayout::BottomToTop:
			size.transpose();
			break;
	}

	// return new size hint;
	return size;
}

QBoxLayout::Direction pToolButton::direction() const
{
	return mDirection;
}

void pToolButton::setDirection( QBoxLayout::Direction direction )
{
	if ( mDirection == direction ) {
		return;
	}
	
	mDirection = direction;
	updateGeometry();
}

QVariant pToolButton::userData() const
{
	return mUserData;
}

void pToolButton::setUserData( const QVariant& data )
{
	mUserData = data;
}
