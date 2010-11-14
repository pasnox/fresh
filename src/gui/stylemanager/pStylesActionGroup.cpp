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

QString pStylesActionGroup::systemStyle() const
{
	return mSystemStyle;
}

QString pStylesActionGroup::applicationStyle() const
{
	return QApplication::style()->objectName();
}

void pStylesActionGroup::installInMenuBar( pActionsNodeMenuBar* menuBar, const QString& path )
{
	foreach ( QAction* action, mActions ) {
		menuBar->addAction( QString( "%1/%2" ).arg( path ).arg( action->objectName() ), action );
	}
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
	mSystemStyle = QApplication::style()->objectName();
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
	const QStringList styles = QStyleFactory::keys();
	const QStringList::const_iterator cend = styles.constEnd();
	
	// Make sure ObjectName  is unique in case toolbar solution is used.
	QString objNamePrefix = QLatin1String( "__qt_designer_style_" );
	
	// Create styles. Set style name string as action data.
	for ( QStringList::const_iterator it = styles.constBegin(); it !=  cend ;++it ) {
		QAction* action = new QAction( this );
		QString objName = objNamePrefix;
		objName += ( *it ).toLower().replace( ' ', '_' );
		QString text = mTextFormat.arg( *it );
		
		if ( QString::compare( ( *it ), mSystemStyle, Qt::CaseInsensitive ) == 0 ) {
			text.append( tr( "(System)" ).prepend( " " ) );
		}
		
		action->setObjectName( objName );
		action->setData( ( *it ).toLower() );
		action->setText( text );
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
