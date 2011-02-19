/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pConsoleCommand.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
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
	
	if ( parts.value( 1 ).compare( "-h", Qt::CaseInsensitive ) == 0 || parts.value( 1 ).compare( "--help", Qt::CaseInsensitive ) == 0 ) {
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
