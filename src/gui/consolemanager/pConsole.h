#ifndef PCONSOLE_H
#define PCONSOLE_H

/*!
	\file pConsole.h
	\brief The pConsole widget is a terminal emulator providing a gui for pConsoleCommand.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"
#include "pConsoleCommand.h"

#include <QPlainTextEdit>
#include <QHash>

/*!
	\ingroup Gui
	\class pConsole
	\brief The pConsole widget is a terminal emulator providing a gui for pConsoleCommand.
*/
class FRESH_EXPORT pConsole : public QPlainTextEdit
{
	Q_OBJECT
	friend class pInternalCommands;

public:
	/*! Thie enumeration represent the different possible output color in the pConsole widget. */
	enum ColorType {
		Command, /*!< \a Command color is the color used when the user is typing a command. */ 
		Error, /*!< \a Error color is the color used when the execution of a command failed and it produce an output. */ 
		Output, /*!< \a Output color is the color used when the execution of a command success and it produce an output. */ 
		Completion /*!< \a Completion color is the color used when the user press the key to auto complete a command and that produce an output. */ 
	};

	/*!
		Create a pConsole widget having \a parent as parent.
	*/
	pConsole( QWidget* parent = 0 );
	/*!
		Create a pConsole widget having \a parent parent and prompt text \a promptText.
	*/
	pConsole( const QString& promptText, QWidget* parent = 0 );
	/*!
		Destroys the console.
	*/
	virtual ~pConsole();

	/*!
		Return the current prompt text of the console.
	*/
	QString prompt() const;
	/*!
		Set the current prompt text to \a prompt.
	*/
	void setPrompt( const QString& prompt );
	/*!
		Tells if the prompt text is visible or not.
	*/
	bool isPromptVisible() const;
	/*!
		set the prompt text visibility to \a visible.
	*/
	void setPromptVisible( bool visible );
	/*!
		Return the console commands history. ie: all the commands the user typed.
	*/
	QStringList history() const;
	/*!
		Set the console commands history.
	*/
	void setHistory( const QStringList& history );
	/*!
		Return the color associated with ColorType \a type.
	*/
	QColor color( pConsole::ColorType type ) const;
	/*!
		Set the color of the \a type to \a color.
	*/
	void setColor( pConsole::ColorType type, const QColor& color );
	/*!
		Execute the command \a command. The command is added to the console according to \a writeCommand and the prompt text is shown according to \a showPrompt.
	*/
	void executeCommand( const QString& command, bool writeCommand = true, bool showPrompt = true );
	/*!
		Save the successfuly executed commands to the file named \a fileName.
	*/
	bool saveScript( const QString& fileName );
	/*!
		Load the file \a fileName containing a list of commands to execute.
	*/
	bool loadScript( const QString& fileName );
	/*!
		Clear the console output.
	*/
	void clear();
	/*!
		reset the internal state of the console and set the prompt text to \a promptText.
		
		If \a promptText is empty, the current one will be used again.
	*/
	void reset( const QString& promptText = QString::null );
	/*!
		Return the list of available pConsoleCommand.
	*/
	pConsoleCommand::List availableCommands() const;
	/*!
		Set the list of available pConsoleCommand.
		
		\note pConsole take ownership of the commands and will delete them in its destructor.
	*/
	void setAvailableCommands( const pConsoleCommand::List& commands );
	/*!
		Add \a command command to the availalbe commands.
		
		\note pConsole take ownership of the command and will delete it in its destructor.
	*/
	void addAvailableCommand( pConsoleCommand* command );
	/*!
		Remove \a command from the available commands.
	*/
	void removeAvailableCommand( pConsoleCommand* command );

protected:
	QString mPrompt;
	QPoint mPromptPosition;
	QStringList mHistory;
	int mHistoryIndex;
	QHash<ColorType, QColor> mColors;
	QStringList mRecordedScript;
	pConsoleCommand::List mAvailableCommands;
	pConsoleCommand* mInternalCommands;
	QSet<QString> mNoPromptCommands;

	/*!
		Reimplemented.
	*/
	virtual void keyPressEvent( QKeyEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void mousePressEvent( QMouseEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void mouseReleaseEvent( QMouseEvent* event );
	/*!
		Reimplemented.
	*/
	virtual void contextMenuEvent( QContextMenuEvent* event );
	/*!
		Tell if the command \a command is complete or not.
	*/
	virtual bool isCommandComplete( const QString& command );
	/*!
		Interpret the \a command and return the exit code in the passed \a exitCode.
	*/
	virtual QString interpretCommand( const QString& command, int* exitCode );
	/*!
		Return a list of possible commands to complete matching \a command.
	*/
	virtual QStringList autoCompleteCommand( const QString& command );
	/*!
		Replace the partially written command by \a command.
	*/
	bool replaceCommand( const QString& command );
	/*!
		Return the current typed (partial) command.
	*/
	QString currentCommand() const;
	/*!
		Set the text cursor position on the last know prompt position.
	*/
	void focusCommand();
	/*!
		Set the current foreground color to use to \a type.
	*/
	void useColor( pConsole::ColorType type );
	/*!
		Display the prompt on a new line.
	*/
	void displayPrompt();
	/*!
		Repalce the current command by the history command at \a index and return true on success, else false.
	*/
	bool showHistoryItem( int index );

signals:
	/*!
		This signal is emited after the command \a command has been executed.
	*/
	void commandExecuted( const QString& command, int exitCode );
};

#endif // PCONSOLE_H
