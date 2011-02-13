#include "pKeySequenceEdit.h"

#include <QKeySequence>
#include <QKeyEvent>

pKeySequenceEdit::pKeySequenceEdit( QWidget* parent )
	: pLineEdit( parent )
{
	mFinished = true;
	
	setSearchButtonVisible( false );
	setPromptText( tr( "Press a keybord shortcut..." ) );
}

pKeySequenceEdit::pKeySequenceEdit( const QString& contents, QWidget* parent )
	: pLineEdit( contents, parent )
{
}

void pKeySequenceEdit::keyPressEvent( QKeyEvent* event )
{	
	// return if auto repeat
	if ( event->isAutoRepeat() ) {
		return;
	}
	
	// if user press something, sequence is not finished
	mFinished = false;
	
	// show current sequence
	setText( keySequence( event ) );
}

void pKeySequenceEdit::keyReleaseEvent( QKeyEvent* event )
{
	// return if auto repeat
	if ( event->isAutoRepeat() ) {
		return;
	}
	
	// check if sequence is finished or not
	if ( mFinished ) {
		return;
	}
	
	// show current sequence
	setText( keySequence( event ) );
}

QString pKeySequenceEdit::keySequence( QKeyEvent* event ) const
{
	// is key pressed or key released ?
	const bool keyPressed = event->type() == QEvent::KeyPress;
	
	// or-ed keys
	int mKeys = 0;
	
	// check modifiers pressed
	if ( event->modifiers() & Qt::ControlModifier ) {
		mKeys |= Qt::ControlModifier;
	}
	
	if ( event->modifiers() & Qt::AltModifier ) {
		mKeys |= Qt::AltModifier;
	}
	
	if ( event->modifiers() & Qt::ShiftModifier ) {
		mKeys |= Qt::ShiftModifier;
	}
	
	if ( event->modifiers() & Qt::MetaModifier ) {
		mKeys |= Qt::MetaModifier;
	}
	
	if ( keyPressed ) {
		// get press key
		switch ( event->key() ) {
			// this keys can't be used
			case Qt::Key_Control:
			case Qt::Key_Alt:
			case Qt::Key_AltGr:
			case Qt::Key_Shift:
			case Qt::Key_Meta:
			case Qt::Key_Super_L:
			case Qt::Key_Super_R:
			case Qt::Key_Menu:
			case Qt::Key_Hyper_L:
			case Qt::Key_Hyper_R:
			case Qt::Key_Help:
			case Qt::Key_Direction_L:
			case Qt::Key_Direction_R:
				break;
			default:
				// add pressed key
				mKeys |= event->key();
				
				// set sequence finished
				mFinished = true;
				
				break;
		}
	}
	
	// return human readable key sequence
	return QKeySequence( mKeys ).toString();
}
