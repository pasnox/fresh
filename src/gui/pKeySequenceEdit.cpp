/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pKeySequenceEdit.cpp
** Date      : 2011-02-20T00:41:35
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
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

QKeySequence pKeySequenceEdit::shortcut() const
{
    return QKeySequence::fromString( text() );
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
