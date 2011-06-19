class pStylesActionGroup : QActionGroup
{
%TypeHeaderCode
#include <gui/stylemanager/pStylesActionGroup.h>
%End

public:
	pStylesActionGroup( QObject* parent = 0 );
	pStylesActionGroup( const QString& textFormat, QObject* parent = 0 );
	
	bool isCheckable() const;
	QString currentStyle() const;
	
	QAction* systemAction() const;
	QAction* applicationAction() const;
	
	QString systemStyle() const;
	QString applicationStyle() const;
	
	void installInMenuBar( pActionsMenuBar* menuBar, const QString& path );

public slots:
	void setCheckable( bool checkable );
	void setCurrentStyle( const QString& style );

protected:
	void init( const QString& textFormat );
	void updateActions();

protected slots:
	void actionToggled( bool checked );
	void actionTriggered( QAction* action );

signals:
	void styleSelected( const QString& style );
};
