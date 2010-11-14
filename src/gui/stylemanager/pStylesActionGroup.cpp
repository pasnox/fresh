#include "pStylesActionGroup.h"
#include "gui/actionmanager/pActionsNodeMenuBar.h"

#include <QStyleFactory>
#include <QStyle>
#include <QApplication>

pStylesActionGroup::pStylesActionGroup( QObject* parent )
	: QActionGroup( parent )
{
	init( QLatin1String( "%1" ) );
}

pStylesActionGroup::pStylesActionGroup( const QString& textFormat, QObject* parent )
	: QActionGroup( parent )
{
	init( textFormat );
}

bool pStylesActionGroup::isCheckable() const
{
	return mCheckable;
}

QString pStylesActionGroup::currentStyle() const
{
	return checkedAction() ? checkedAction()->data().toString() : systemStyle();
}

QAction* pStylesActionGroup::systemAction() const
{
	return mActions.value( systemStyle().toLower() );
}

QAction* pStylesActionGroup::applicationAction() const
{
	return mActions.value( applicationStyle().toLower() );
}

void pStylesActionGroup::installInMenuBar( pActionsNodeMenuBar* menuBar, const QString& path )
{
	foreach ( QAction* action, mActions ) {
		menuBar->addAction( QString( "%1/%2" ).arg( path ).arg( action->objectName() ), action );
	}
}

QStringList pStylesActionGroup::availableStyles()
{
	return QStyleFactory::keys();
}

QString pStylesActionGroup::systemStyle()
{
	const QStringList styles = availableStyles();
	QString style;
	
#if defined( Q_OS_WIN )
	const QStringList possibleStyles = QStringList()
		<< "WindowsVista"
		<< "WindowsXP"
		<< "Windows";
	
	for ( int i = possibleStyles.count() -1; i > -1; i-- ) {
		if ( styles.contains( possibleStyles.at( i ), Qt::CaseInsensitive ) ) {
			style = possibleStyles.at( i );
			break;
		}
	}
#elif defined( Q_OS_MAC )
	style = "Macintosh (aqua)";
#else
	const QString desktop = qgetenv( "DESKTOP_SESSION" ).toLower();
	const QString version = qgetenv( QString( "%1_SESSION_VERSION" ).arg( desktop.toUpper() ).toLocal8Bit() );
	
	if ( desktop == "kde" /*&& version == "4"*/ ) {
		style = "Oxygen";
	}
	else if ( desktop == "gnome" || desktop == "xfce" ) {
		style = styles.contains( "GTK+", Qt::CaseInsensitive ) ? "GTK+" : "Cleanlooks";
	}
#endif
	
	if ( styles.contains( style, Qt::CaseInsensitive ) ) {
		return style;
	}
	
	return applicationStyle();
}

QString pStylesActionGroup::applicationStyle()
{
	return QApplication::style()->objectName();
}

void pStylesActionGroup::setCheckable( bool checkable )
{
	mCheckable = checkable;
	
	foreach ( QAction* action, mActions ) {
		action->setCheckable( mCheckable );
	}
}

void pStylesActionGroup::setCurrentStyle( const QString& style )
{
	QAction* action = mActions.value( style.toLower() );
	
	if ( action ) {
		action->setChecked( true );
	}
}

void pStylesActionGroup::init( const QString& textFormat )
{
	mCheckable = true;
	mTextFormat = textFormat;
	
	updateActions();
	
	connect( this, SIGNAL( triggered( QAction* ) ), this, SLOT( actionTriggered( QAction* ) ) );
}

void pStylesActionGroup::updateActions()
{
	const QString curStyle = currentStyle().toLower();
	
	qDeleteAll( mActions.values() );
	mActions.clear();
	
	// Add style actions
	const QStringList styles = availableStyles();
	const QStringList::const_iterator cend = styles.constEnd();
	
	// Make sure ObjectName  is unique in case toolbar solution is used.
	QString objNamePrefix = QLatin1String( "__qt_designer_style_" );
	
	// Create styles. Set style name string as action data.
	for ( QStringList::const_iterator it = styles.constBegin(); it !=  cend ;++it ) {
		QAction* action = new QAction( mTextFormat.arg( *it ), this );
		QString objName = objNamePrefix;
		objName += ( *it ).toLower().replace( ' ', '_' );
		//objName += objNamePostfix;
		
		action->setObjectName( objName );
		action->setData( ( *it ).toLower() );
		action->setCheckable( true );
		action->setChecked( ( *it ).toLower() == curStyle );
		
		mActions[ ( *it ).toLower() ] = action;
		
		addAction( action );
		
		connect( action, SIGNAL( toggled( bool ) ), this, SLOT( actionToggled( bool ) ) );
	}
}

void pStylesActionGroup::actionToggled( bool checked )
{
	if ( mCheckable && checked ) {
		QAction* action = qobject_cast<QAction*>( sender() );
		emit styleSelected( action->data().toString() );
	}
}

void pStylesActionGroup::actionTriggered( QAction* action )
{
	if ( !mCheckable ) {
		emit styleSelected( action->data().toString() );
	}
}
