/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pConsole.cpp
** Date      : 2011-02-20T00:41:09
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
#include "pConsole.h"

#include <QTextBlock>
#include <QApplication>
#include <QClipboard>
#include <QFile>

class pInternalCommands : public pConsoleCommand
{
public:
	pInternalCommands( pConsole* console )
		: pConsoleCommand( QStringList() << "clear" << "cls" << "reset" << "help" )
	{
		Q_ASSERT( console );
		mConsole = console;
		
		setDescription( "clear", pConsole::tr( "Clear the screen" ) );
		setDescription( "cls", pConsole::tr( "An alias for clear command" ) );
		setDescription( "reset", pConsole::tr( "Reset the console" ) );
		setDescription( "help", pConsole::tr( "List available commands" ) );
	}
	
	virtual QString interpret( const QString& command, int* exitCode ) const
	{
		int ec = pConsoleCommand::NotFound;
		QString output = pConsoleCommand::interpret( command, &ec );
		QStringList parts = parseCommand( command );
		const QString cmd = parts.isEmpty() ? QString::null : parts.takeFirst();
		
		if ( ec != pConsoleCommand::NotFound ) {
			// nothing to do
		}
		else if ( cmd == "clear" || cmd == "cls" ) {
			ec = parts.isEmpty() ? pConsoleCommand::Success : pConsoleCommand::Error;
			output = parts.isEmpty() ? QString::null : pConsole::tr( "%1 take no parameter" ).arg( cmd );
			
			if ( parts.isEmpty() ) {
				mConsole->clear();
			}
		}
		else if ( cmd == "reset" ) {
			ec = parts.isEmpty() ? pConsoleCommand::Success : pConsoleCommand::Error;
			output = parts.isEmpty() ? QString::null : pConsole::tr( "%1 take no parameter" ).arg( cmd );
			
			if ( parts.isEmpty() ) {
				mConsole->reset();
			}
		}
		else if ( cmd == "help" ) {
			ec = parts.isEmpty() ? pConsoleCommand::Success : pConsoleCommand::Error;
			output = parts.isEmpty() ? QString::null : pConsole::tr( "%1 take no parameter" ).arg( cmd );
			
			if ( parts.isEmpty() ) {
				const pConsoleCommand::List commands = pConsoleCommand::List() << const_cast<pInternalCommands*>( this ) << mConsole->availableCommands();
				QStringList help;
				
				foreach ( const pConsoleCommand* command, commands ) {
					foreach ( const QString& cmd, command->commands() ) {
							help << QString( "%1\t\t%2" ).arg( cmd ).arg( command->description( cmd ) );
					}
				}
				
				if ( help.isEmpty() ) {
					help << pConsole::tr( "No help available" );
				}
				else {
					help.prepend( pConsole::tr( "Available commands:" ).append( "\n" ) );
					help << pConsole::tr( "For specific command details, type: <command> -h/--help" ).prepend( "\n" );
				}
				
				output = help.join( "\n" );
			}
		}
		
		if ( exitCode ) {
			*exitCode = ec;
		}
		
		return output;
	}

protected:
	pConsole* mConsole;
};

pConsole::pConsole( QWidget* parent )
	: QPlainTextEdit( parent )
{
	mInternalCommands = new pInternalCommands( this );
	mNoPromptCommands << "cls" << "clear" << "reset";
	
	reset();
}

pConsole::pConsole( const QString& promptText, QWidget* parent )
	: QPlainTextEdit( parent )
{
	mInternalCommands = new pInternalCommands( this );
	mNoPromptCommands << "cls" << "clear" << "reset";
	
	reset( promptText );
}

pConsole::~pConsole()
{
	delete mInternalCommands;
	qDeleteAll( mAvailableCommands );
	mAvailableCommands.clear();
}

QString pConsole::prompt() const
{
	return mPrompt;
}

void pConsole::setPrompt( const QString& prompt )
{
	mPrompt = prompt;
	
	if ( isPromptVisible() ) {
		displayPrompt();
	}
}

bool pConsole::isPromptVisible() const
{
	return !isReadOnly();
}

void pConsole::setPromptVisible( bool visible )
{
	setReadOnly( !visible );
}

QStringList pConsole::history() const
{
	return mHistory;
}

void pConsole::setHistory( const QStringList& history )
{
	mHistory = history;
	mHistoryIndex = mHistory.count();
}

QColor pConsole::color( pConsole::ColorType type ) const
{
	return mColors.value( type );
}

void pConsole::setColor( pConsole::ColorType type, const QColor& color )
{
	mColors[ type ] = color;
}

void pConsole::executeCommand( const QString& _command, bool writeCommand, bool showPrompt )
{
	const QString command = _command.trimmed();
	
	// write command to execute
	if ( writeCommand ) {
		if ( !currentCommand().isEmpty() ) {
			displayPrompt();
		}
		
		insertPlainText( command );
	}
	
	// execute command
	int exitCode = pConsoleCommand::NotFound;
	QString output = interpretCommand( command, &exitCode );
	
	// write output in different colors if needed
	if ( exitCode == pConsoleCommand::Success ) {
		useColor( pConsole::Output );
	}
	else {
		useColor( pConsole::Error );
	}
	
	if ( !output.isEmpty() ) {
		appendPlainText( output );
	}
	
	useColor( pConsole::Command );
	
	// display the prompt again if needed
	if ( showPrompt && !mNoPromptCommands.contains( command ) ) {
		displayPrompt();
	}
}

bool pConsole::saveScript( const QString& fileName )
{
	QFile file( fileName );
	
	if ( !file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
		return false;
	}
	
	file.resize( 0 );
	
	foreach ( const QString& command, mRecordedScript ) {
		if ( file.write( QString( command ).append( "\n" ).toUtf8() ) == -1 ) {
			file.close();
			return false;
		}
	}
	
	file.close();
	return true;
}

bool pConsole::loadScript( const QString& fileName )
{
	QFile file( fileName );
	
	if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		return false;
	}
	
	while ( file.canReadLine() ) {
		executeCommand( QString::fromUtf8( file.readLine() ), true, false );
	}
	
	file.close();
	return true;
}

void pConsole::clear()
{
	QPlainTextEdit::clear();
	displayPrompt();
}

void pConsole::reset( const QString& promptText )
{
	QPlainTextEdit::clear();
	
	setTextInteractionFlags( Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard | Qt::LinksAccessibleByMouse | Qt::LinksAccessibleByKeyboard | Qt::TextEditable );
	setUndoRedoEnabled( false );
	setTabStopWidth( 40 );
	
	QFont font = QFont( "Bitstream Vera Sans Mono" );
	font.setPixelSize( 11 );
	setFont( font );
	
	QPalette pal = viewport()->palette();
	pal.setColor( viewport()->backgroundRole(), QColor( Qt::black ) );
	pal.setColor( viewport()->foregroundRole(), QColor( Qt::white ) );
	viewport()->setPalette( pal );
	
	mColors[ pConsole::Command ] = Qt::white;
	mColors[ pConsole::Error ] = Qt::red;
	mColors[ pConsole::Output ] = Qt::gray;
	mColors[ pConsole::Completion ] = Qt::green;
	
	mRecordedScript.clear();
	
	QString prompt = promptText.isEmpty() ? mPrompt : promptText;
	
	if ( prompt.isEmpty() ) {
		prompt = "@:/> ";
	}
	
	setHistory( QStringList() );
	setPromptVisible( true );
	appendPlainText( tr( "Press 'Tab' key to list or auto complete commands, type help for commands help." ) );
	setPrompt( prompt );
}

pConsoleCommand::List pConsole::availableCommands() const
{
	return mAvailableCommands;
}

void pConsole::setAvailableCommands( const pConsoleCommand::List& commands )
{
	qDeleteAll( mAvailableCommands );
	mAvailableCommands.clear();
	mAvailableCommands = commands;
}

void pConsole::addAvailableCommand( pConsoleCommand* command )
{
	if ( !mAvailableCommands.contains( command ) ) {
		mAvailableCommands << command;
	}
}

void pConsole::removeAvailableCommand( pConsoleCommand* command )
{
	mAvailableCommands.removeOne( command );
}

void pConsole::keyPressEvent( QKeyEvent* event )
{
	// filter out cut action
	if ( event->matches( QKeySequence::Cut ) ) {
		return;
	}
	
	// filter out paste action
	if ( event->matches( QKeySequence::Paste ) ) {
		QString command = QApplication::clipboard()->text()
			.replace( "\r\n", "\n" )
			.replace( "\r", "\n" );
		
		while ( command.contains( "\n\n" ) ) {
			command.replace( "\n\n", "\n" );
		}
		
		const QStringList commands = command.split( "\n" );
		
		switch ( commands.count() ) {
			case 0:
				break;
			case 1:
				insertPlainText( commands.first().trimmed() );
				break;
			default: {
				foreach ( QString command, commands ) {
					command = command.trimmed();
					
					if ( !command.isEmpty() /*&& isCommandComplete( command )*/ ) {
						executeCommand( command, true );
					}
				}
				break;
			}
		}
		
		return;
	}
	
	// some needed infos
	QTextCursor cursor = textCursor();
	int start = cursor.selectionStart();
	int end = cursor.selectionEnd();
	int promptStart = cursor.block().position() +mPrompt.length();
	int historyId = mHistoryIndex;
	bool processEvent = true;
	QString typedCommand;
	
	if ( start > end ) {
		qSwap( start, end );
	}
	
	// case
	switch ( event->key() ) {
		case Qt::Key_Enter:
		case Qt::Key_Return: {
			const QString command = currentCommand();
			
			if ( event->modifiers() == Qt::NoModifier && !command.isEmpty() /*&& isCommandComplete( command )*/ ) {
				executeCommand( command, false );
			}
			
			return;
		}
		case Qt::Key_Escape: {
			// stopCommand();
			
			return;
			break;
		}
		case Qt::Key_Left: {
			if ( cursor.position() <= promptStart ) {
				return;
			}
			
			break;
		}
		case Qt::Key_Backspace: {
			/*if ( event->modifiers() == Qt::NoModifier )*/ {
				if ( ( cursor.position() <= promptStart && !cursor.hasSelection() )
					|| ( cursor.hasSelection() && start < promptStart ) ) {
					return;
				}
			}
			
			break;
		}
		case Qt::Key_Delete: {
			/*if ( event->modifiers() == Qt::NoModifier )*/ {
				if ( cursor.hasSelection() && start < promptStart ) {
					return;
				}
			}
			
			break;
		}
		case Qt::Key_Right: {
			break;
		}
		case Qt::Key_Up: {
			processEvent = false;
			historyId--;
			
			break;
		}
		case Qt::Key_Down: {
			processEvent = false;
			historyId++;
			
			break;
		}
		case Qt::Key_Home: {
			if ( event->modifiers() == Qt::NoModifier ) {
				processEvent = false;
				QTextCursor cursor = textCursor();
				
				cursor.setPosition( cursor.block().position() +mPrompt.length() );
				setTextCursor( cursor );
			}
			
			break;
		}
		case Qt::Key_PageUp: {
			processEvent = false;
			historyId = -1;
			
			if ( !mHistory.isEmpty() ) {
				historyId = 0;
			}
			
			break;
		}
		case Qt::Key_PageDown: {
			processEvent = false;
			historyId = -1;
			
			if ( !mHistory.isEmpty() ) {
				historyId = mHistory.count() -1;
			}
			
			break;
		}
		case Qt::Key_Tab: {
			QString command = currentCommand();
			QStringList sl = autoCompleteCommand( command );
			QString str = sl.join( "    " );
			
			if ( sl.count() == 1 ) {
				replaceCommand( sl.at( 0 ) +" " );
			}
			else if ( !sl.isEmpty() ) {
				useColor( pConsole::Completion );
				appendPlainText( str );
				useColor( pConsole::Command );
				displayPrompt();
				insertPlainText( command );
			}
			
			return;
		}
		default:
			break;
	}
	
	if ( processEvent ) {
		if ( cursor.hasSelection() && event->modifiers() == Qt::NoModifier ) {
			const QChar c( event->text().isEmpty() ? QChar() : event->text().at( 0 ) );
			
			if ( c.isPrint() ) {
				focusCommand();
			}
		}
		
		QPlainTextEdit::keyPressEvent( event );
		
		// trunc the selection to remove the prompt
		if ( textCursor().hasSelection() && event->key() == Qt::Key_Home && event->modifiers() == Qt::ShiftModifier ) {
			QTextCursor cursor = textCursor();
			int start = cursor.selectionStart();
			int end = cursor.selectionEnd();
			
			start = cursor.block().position() +mPrompt.length();
			
			cursor.setPosition( end, QTextCursor::MoveAnchor );
			cursor.setPosition( start, QTextCursor::KeepAnchor );
			
			setTextCursor( cursor );
		}
		
		typedCommand = currentCommand();
	}
	
	if ( historyId != mHistoryIndex ) {
		if ( !showHistoryItem( historyId ) ) {
			if ( historyId < 0 ) {
				mHistoryIndex = 0;
			}
			else {
				mHistoryIndex = mHistory.count();
				
				replaceCommand( typedCommand );
			}
		}
	}
	
	mPromptPosition.setX( textCursor().columnNumber() );
}

void pConsole::mousePressEvent( QMouseEvent* event )
{
	QTextCursor cursor = textCursor();
	int length = mPrompt.length();
	int column = cursor.columnNumber() < length ? length : cursor.columnNumber();
	
	if ( event->buttons() == Qt::LeftButton ) {
		mPromptPosition = QPoint( column, cursor.blockNumber() );
	}
	
	setPromptVisible( false );
	
	QPlainTextEdit::mousePressEvent( event );
}

void pConsole::mouseReleaseEvent( QMouseEvent* event )
{
	QPlainTextEdit::mouseReleaseEvent( event );
	
	if ( event->button() == Qt::LeftButton && textCursor().hasSelection() ) {
		copy();
	}
	
	focusCommand();
	setPromptVisible( true );
}

void pConsole::contextMenuEvent( QContextMenuEvent* event )
{
	// fake release event to reposition the cursor as it's not triggered when requesting context menu
	QMouseEvent me( QEvent::MouseButtonRelease, QPoint(), Qt::NoButton, Qt::NoButton, Qt::NoModifier );
	QApplication::sendEvent( viewport(), &me );
	
	QPlainTextEdit::contextMenuEvent( event );
	
	
}

bool pConsole::isCommandComplete( const QString& command )
{
	const pConsoleCommand::List commands = pConsoleCommand::List() << mInternalCommands << mAvailableCommands;
	
	foreach ( const pConsoleCommand* cmd, commands ) {
		if ( cmd->isComplete( command ) ) {
			return true;
		}
	}
	
	return false;
}

QString pConsole::interpretCommand( const QString& command, int* exitCode )
{
	const pConsoleCommand::List commands = pConsoleCommand::List() << mInternalCommands << mAvailableCommands;
	QString output;
	bool foundCommand = false;
	
	foreach ( const pConsoleCommand* cmd, commands ) {
		if ( cmd->isComplete( command ) ) {
			foundCommand = true;
			output = cmd->interpret( command, exitCode );
			break;
		}
	}
	
	if ( !foundCommand ) {
		output = tr( "%1: Command not found." ).arg( command );
	}
	
	// add the command to the recordedScript list
	if ( exitCode && *exitCode == pConsoleCommand::Success ) {
		mRecordedScript << command;
	}
	
	// update history
	mHistory << QString( command ).replace( "\n", "\\n" );
	mHistoryIndex = mHistory.count();
	
	// emit command executed
	emit commandExecuted( command, exitCode ? *exitCode : pConsoleCommand::NoExitCode );
	
	// return output
	return output;
}

QStringList pConsole::autoCompleteCommand( const QString& command )
{
	const pConsoleCommand::List commands = pConsoleCommand::List() << mInternalCommands << mAvailableCommands;
	QStringList result;
	
	foreach ( const pConsoleCommand* cmd, commands ) {
		const QStringList list = cmd->autoCompleteList( command );
		
		if ( !list.isEmpty() ) {
			result << list;
		}
	}
	
	return result;
}

bool pConsole::replaceCommand( const QString& command )
{
	QTextBlock block = document()->findBlockByNumber( mPromptPosition.y() );
	
	if ( !block.isValid() ) {
		return false;
	}
	
	QTextCursor cursor( block );
	cursor.beginEditBlock();
	cursor.movePosition( QTextCursor::StartOfBlock );
	cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor );
	cursor.removeSelectedText();
	cursor.insertText( mPrompt );
	cursor.insertText( command );
	cursor.endEditBlock();
	
	mPromptPosition.setX( cursor.columnNumber() );
	
	return true;
}

QString pConsole::currentCommand() const
{
	QTextBlock block = document()->findBlockByNumber( mPromptPosition.y() );
	
	if ( !block.isValid() ) {
		return QString::null;
	}
	
	return block.text().mid( mPrompt.length() ).trimmed();
}

void pConsole::focusCommand()
{
	QTextBlock block = document()->findBlockByNumber( mPromptPosition.y() );
	
	if ( !block.isValid() ) {
		return;
	}
	
	QTextCursor cursor( block );
	cursor.beginEditBlock();
	cursor.movePosition( QTextCursor::StartOfBlock );
	cursor.setPosition( cursor.position() +mPromptPosition.x(), QTextCursor::MoveAnchor );
	cursor.endEditBlock();
	
	setTextCursor( cursor );
}

void pConsole::useColor( ColorType type )
{
	QTextCursor cursor = textCursor();
	QTextCharFormat format = cursor.charFormat();
	
	format.setForeground( color( type ) );
	cursor.setCharFormat( format );
	setTextCursor( cursor );
}

void pConsole::displayPrompt()
{
	if ( mPrompt.isEmpty() ) {
		return;
	}
	
	useColor( pConsole::Command );
	
	appendPlainText( mPrompt );
	
	mHistoryIndex = mHistory.count();
	QTextBlock block = document()->lastBlock();
	QTextCursor cursor( block );
	
	cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::MoveAnchor );
	setTextCursor( cursor );
	
	mPromptPosition = QPoint( cursor.columnNumber(), cursor.blockNumber() );
}

bool pConsole::showHistoryItem( int index )
{
	if ( index >= 0 && index < mHistory.count() ) {
		mHistoryIndex = index;
		
		return replaceCommand( mHistory.at( index ) );
	}
	
	return false;
}
