#include "pStylesToolButton.h"
#include "pStylesActionGroup.h"

#include <QMenu>

pStylesToolButton::pStylesToolButton( QWidget* parent )
	: QToolButton( parent )
{
	init( QLatin1String( "%1" ) );
}

pStylesToolButton::pStylesToolButton( const QString& textFormat, QWidget* parent )
	: QToolButton( parent )
{
	init( textFormat );
}

bool pStylesToolButton::isCheckableActions() const
{
	return mActions->isCheckable();
}

QString pStylesToolButton::currentStyle() const
{
	return mActions->currentStyle();
}

QAction* pStylesToolButton::systemAction() const
{
	return mActions->systemAction();
}

QAction* pStylesToolButton::applicationAction() const
{
	return mActions->applicationAction();
}

QString pStylesToolButton::systemStyle() const
{
	return mActions->systemStyle();
}

QString pStylesToolButton::applicationStyle() const
{
	return mActions->applicationStyle();
}

void pStylesToolButton::setCheckableActions( bool checkable )
{
	mActions->setCheckable( checkable );
}

void pStylesToolButton::setCurrentStyle( const QString& style )
{
	mActions->setCurrentStyle( style );
}

void pStylesToolButton::init( const QString& textFormat )
{
	mActions = new pStylesActionGroup( textFormat, this );
	mMenu = new QMenu( this );
	
	mActions->updateActions();
	mMenu->addActions( mActions->actions() );
	
	setMenu( mMenu );
	setText( mActions->systemAction()->text() );
	setPopupMode( QToolButton::MenuButtonPopup );
	
	connect( this, SIGNAL( clicked() ), mActions->systemAction(), SLOT( toggle() ) );
	connect( mActions, SIGNAL( styleSelected(const QString& ) ), this, SIGNAL( styleSelected(const QString& ) ) );
}
