#include "pConsoleCommand.h"
#include "pConsole.h"

pConsoleCommand::pConsoleCommand( const QStringList& commands )
{
	mCommands = commands;
}

pConsoleCommand::~pConsoleCommand()
{
}

pConsoleCommand::pConsoleCommand( const pConsoleCommand& other )
{
	operator=( other );
}

pConsoleCommand& pConsoleCommand::operator=( const pConsoleCommand& other )
{
	if( *this != other ) {
		mCommands = other.mCommands;
	}

	return *this;
}

bool pConsoleCommand::operator==( const pConsoleCommand& other ) const
{
	return mCommands == other.mCommands;
}

bool pConsoleCommand::operator!=( const pConsoleCommand& other ) const
{
	return !operator==( other );
}

QStringList pConsoleCommand::commands() const
{
	return mCommands;
}

QStringList pConsoleCommand::autoCompleteList( const QString& command ) const
{
	QStringList result;
	
	foreach ( const QString& cmd, mCommands ) {
		if ( cmd.startsWith( command ) ) {
			result << cmd;
		}
	}
	
	return result;
}

QString pConsoleCommand::description( const QString& command ) const
{
	const QString cmd = parseCommand( command ).value( 0 );
	return mDescriptions.value( cmd, pConsole::tr( "No description available" ) );
}

void pConsoleCommand::setDescription( const QString& command, const QString& description )
{
	const QString cmd = parseCommand( command ).value( 0 );
	
	if ( !cmd.isEmpty() ) {
		mDescriptions[ cmd ] = description;
	}
}

bool pConsoleCommand::isComplete( const QString& command ) const
{
	return mCommands.contains( parseCommand( command ).value( 0 ) );
}

QString pConsoleCommand::usage( const QString& command ) const
{
	return description( command );
}

QString pConsoleCommand::interpret( const QString& command, int* exitCode ) const
{
	const QStringList parts = parseCommand( command );
	
	if ( parts.value( 1 ).compare( "help", Qt::CaseInsensitive ) == 0 ) {
		if ( exitCode ) {
			*exitCode = pConsoleCommand::Success;
		}
		
		return usage( command );
	}
	
	if ( exitCode ) {
		*exitCode = pConsoleCommand::NotFound;
	}
	
	return QString::null;
}

QStringList pConsoleCommand::quotedStringList( const QStringList& list )
{
	QStringList entries;
	
	foreach ( QString string, list ) {
		if ( string.contains( " " ) && !string.startsWith( '"' ) && !string.endsWith( '"' ) ) {
			string.replace( "\"", "\\\"" ).prepend( '"' ).append( '"' );
		}
		
		entries << string;
	}
	
	return entries;
}

QStringList pConsoleCommand::parseCommand( const QString& command )
{
	QString cmd = command;
	QStringList result;
	bool isExtended = false;
	int pos = 0;
	
	while ( ( pos = cmd.indexOf( QRegExp( "\"|\\s" ), pos ) ) != -1 ) {
		QChar pc = pos > 0 ? cmd[ pos -1 ] : QChar();
		QChar c = cmd[ pos ];
		
		if ( c == '"' ) {
			pos++;
			
			if ( pc != '\\' ) {
				if ( isExtended ) {
					isExtended = false;
					result << cmd.left( pos -1 ).replace( "\\\"", "\"" );
					cmd.remove( 0, pos );
					pos = 0;
				}
				else {
					isExtended = true;
					cmd.remove( 0, 1 );
					pos = 0;
				}
			}
		}
		else {
			pos++;
			
			if ( !isExtended ) {
				result << cmd.left( pos -1 );
				cmd.remove( 0, pos );
				pos = 0;
			}
		}
		
		cmd = cmd.trimmed();
	}
	
	if ( !cmd.isEmpty() ) {
		result << cmd;
	}
	
	return result;
}
