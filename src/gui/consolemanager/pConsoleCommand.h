#ifndef PCONSOLECOMMAND_H
#define PCONSOLECOMMAND_H

/*!
	\file pConsoleCommand.h
	\brief pConsoleCommand is the command interpreter to be used with pConsole.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QStringList>
#include <QHash>

/*!
	\ingroup Gui
	\class pConsoleCommand
	\brief pConsoleCommand is the command interpreter to be used with pConsole.
*/
class FRESH_EXPORT pConsoleCommand
{
public:
	/*! This enumeration represents some standard exit code */ 
	enum ExitCode {
		NotFound = -2, /*!< The command to be executed was not found. */ 
		NoExitCode = -1, /*!< Undefined exit code, this is the case when a command is executed and a pointer for the exit code is not given. */ 
		Success = 0, /*!< The command was successfuly executed. */ 
		Error = 1 /*!< An error occurs while executing the command. */ 
	};
	
	/*!< A QList of pConsoleCommand. */ 
	typedef QList<pConsoleCommand*> List;
	
	/*!
		Create a pConsoleCommand that can handle \a commands.
	*/
	pConsoleCommand( const QStringList& commands = QStringList() );
	/*!
		Destroys the pConsoleCommand.
	*/
	virtual ~pConsoleCommand();
	/*!
		Create a new pConsoleCommand that is a copy of \a other.
	*/
	pConsoleCommand( const pConsoleCommand& other );
	/*!
		Copy the data of \a other pConsoleCommand.
	*/
	pConsoleCommand& operator=( const pConsoleCommand& other );
	/*!
		Returns true if this pConsoleCommand is equals to \a other else false.
	*/
	bool operator==( const pConsoleCommand& other ) const;
	/*!
		Return true if this pConsoleCommand is differents than \a other else false.
	*/
	bool operator!=( const pConsoleCommand& other ) const;
	/*!
		Return the list of handled commands.
	*/
	QStringList commands() const;
	/*!
		Return a list of commands matching \a command.
	*/
	QStringList autoCompleteList( const QString& command ) const;
	/*!
		Return the description associated with \a command.
	*/
	virtual QString description( const QString& command ) const;
	/*!
		Set the \a description for \a command.
	*/
	void setDescription( const QString& command, const QString& description );
	/*!
		Returns true if the \a command is complete or not.
		
		A complete command is a command defined in the \a commands() list.
	*/
	virtual bool isComplete( const QString& command ) const;
	/*!
		Return the usage description for \a command.
	*/
	virtual QString usage( const QString& command ) const;
	/*!
		Execute the \a command line (which may have parameters) and set the exit code in \a exitCode.
	*/
	virtual QString interpret( const QString& command, int* exitCode ) const;
	/*!
		Return a QStringList where each entries is quoted if needed and internal quote escaped.
	*/
	static QStringList quotedStringList( const QStringList& list );
	/*!
		Parse the \a command line and add each parts in a QStringList.
	*/
	static QStringList parseCommand( const QString& command );

protected:
	QStringList mCommands;
	QHash<QString, QString> mDescriptions;
};

#endif // PCONSOLECOMMAND_H
