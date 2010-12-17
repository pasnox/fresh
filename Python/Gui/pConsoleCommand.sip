class pConsoleCommand
{
%TypeHeaderCode
#include <../src/gui/consolemanager/pConsoleCommand.h>
%End

public:
	enum ExitCode {
		NotFound = -2,
		NoExitCode = -1,
		Success = 0,
		Error = 1
	};
	
	typedef QList<pConsoleCommand*> List;
	
	pConsoleCommand( const QStringList& commands = QStringList() );
	virtual ~pConsoleCommand();

	pConsoleCommand( const pConsoleCommand& other );
	bool operator==( const pConsoleCommand& other ) const;
	bool operator!=( const pConsoleCommand& other ) const;

	QStringList commands() const;
	QStringList autoCompleteList( const QString& command ) const;
	
	virtual QString description( const QString& command ) const;
	void setDescription( const QString& command, const QString& description );

	virtual bool isComplete( const QString& command ) const;
	virtual QString usage( const QString& command ) const;
	virtual QString interpret( const QString& command, int* exitCode ) const;
	
	static QStringList quotedStringList( const QStringList& list );
	static QStringList parseCommand( const QString& command );

};
