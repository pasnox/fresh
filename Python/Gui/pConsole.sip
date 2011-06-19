class pConsole : QPlainTextEdit
{
%TypeHeaderCode
#include <gui/consolemanager/pConsole.h>
%End

public:
	enum ColorType {
		Command,
		Error,
		Output,
		Completion
	};

	pConsole( QWidget* parent = 0 );
	pConsole( const QString& promptText, QWidget* parent = 0 );
	virtual ~pConsole();

	QString prompt() const;
	void setPrompt( const QString& prompt );

	bool isPromptVisible() const;
	void setPromptVisible( bool visible );

	QStringList history() const;
	void setHistory( const QStringList& history );

	QColor color( pConsole::ColorType type ) const;
	void setColor( pConsole::ColorType type, const QColor& color );

	void executeCommand( const QString& command, bool writeCommand = true, bool showPrompt = true );

	bool saveScript( const QString& fileName );
	bool loadScript( const QString& fileName );

	void clear();
	void reset( const QString& promptText = QString::null );

	pConsoleCommand::List availableCommands() const;
	void setAvailableCommands( const pConsoleCommand::List& commands );

	void addAvailableCommand( pConsoleCommand* command );
	void removeAvailableCommand( pConsoleCommand* command );

protected:

	virtual void keyPressEvent( QKeyEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );
	virtual void contextMenuEvent( QContextMenuEvent* event );

	virtual bool isCommandComplete( const QString& command );
	virtual QString interpretCommand( const QString& command, int* exitCode );
	virtual QStringList autoCompleteCommand( const QString& command );

	bool replaceCommand( const QString& command );
	QString currentCommand() const;
	void focusCommand();

	void useColor( pConsole::ColorType type );
	void displayPrompt();
	bool showHistoryItem( int index );

signals:
	void commandExecuted( const QString& command, int exitCode );
};
