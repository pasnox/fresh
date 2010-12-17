class pConsole : QPlainTextEdit
{
%TypeHeaderCode
#include <../src/gui/consolemanager/pConsole.h>
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

signals:
	void commandExecuted( const QString& command, int exitCode );
};
