/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pStylesActionGroup.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : http://bettercodes.org/projects/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pStylesActionGroup.h"
#include "actionmanager/pActionsMenuBar.h"
#include "actionmanager/pActionsModel.h"

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

void pStylesActionGroup::installInMenuBar( pActionsMenuBar* menuBar, const QString& path )
{
	foreach ( QAction* action, mActions ) {
		menuBar->model()->addAction( QString( "%1/%2" ).arg( path ).arg( action->objectName() ), action );
	}
}

void pStylesActionGroup::installInMenu( QMenu* menu )
{
	menu->addActions( mActions.values() );
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
