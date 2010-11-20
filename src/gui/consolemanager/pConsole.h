#ifndef PCONSOLE_H
#define PCONSOLE_H

#include "core/FreshExport.h"
#include "pConsoleCommand.h"

#include <QPlainTextEdit>
#include <QHash>

class FRESH_EXPORT pConsole : public QPlainTextEdit
{
	Q_OBJECT
	friend class pInternalCommands;

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
	QString mPrompt;
	QPoint mPromptPosition;
	QStringList mHistory;
	int mHistoryIndex;
	QString mTypedCommand;
	QHash<ColorType, QColor> mColors;
	QStringList mRecordedScript;
	pConsoleCommand::List mAvailableCommands;
	pConsoleCommand* mInternalCommands;
	QSet<QString> mNoPromptCommands;

	virtual void keyPressEvent( QKeyEvent* event );
	virtual void mousePressEvent( QMouseEvent* event );
	virtual void mouseReleaseEvent( QMouseEvent* event );

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

#endif // PCONSOLE_H
