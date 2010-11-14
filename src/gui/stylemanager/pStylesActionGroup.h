#ifndef PSTYLESACTIONGROUP_H
#define PSTYLESACTIONGROUP_H

#include "core/FreshExport.h"

#include <QActionGroup>
#include <QHash>

class pActionsNodeMenuBar;

class FRESH_EXPORT pStylesActionGroup : public QActionGroup
{
	Q_OBJECT
	friend class pStylesToolButton;

public:
	pStylesActionGroup( QObject* parent = 0 );
	pStylesActionGroup( const QString& textFormat, QObject* parent = 0 );
	
	bool isCheckable() const;
	QString currentStyle() const;
	
	QAction* systemAction() const;
	QAction* applicationAction() const;
	
	void installInMenuBar( pActionsNodeMenuBar* menuBar, const QString& path );
	
	static QString systemStyle();
	static QString applicationStyle();
	static QStringList availableStyles();

public slots:
	void setCheckable( bool checkable );
	void setCurrentStyle( const QString& style );

protected:
	bool mCheckable;
	QString mTextFormat;
	QHash<QString, QAction*> mActions;
	
	void init( const QString& textFormat );
	void updateActions();

protected slots:
	void actionToggled( bool checked );
	void actionTriggered( QAction* action );

signals:
	void styleSelected( const QString& style );
};

#endif // PSTYLESACTIONGROUP_H
