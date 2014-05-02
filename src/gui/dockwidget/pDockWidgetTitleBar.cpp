/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pDockWidgetTitleBar.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
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
#include "pDockWidgetTitleBar.h"
#include "pDockWidget.h"
#include "pToolButton.h"
#include "pGuiUtils.h"

#include <QAction>
#include <QStyleOptionToolButton>
#include <QMenu>
#include <QEvent>
#include <QPainter>

bool pDockWidgetTitleBar::mUseNativePaintDefault = true;

pDockWidgetTitleBar::pDockWidgetTitleBar( QDockWidget* parent )
    : QToolBar( parent )
{
    Q_ASSERT( parent );
    mDock = parent;
    mUseNativePaint = mUseNativePaintDefault;
    
    // a fake spacer widget
    QWidget* spacer = new QWidget( this );
    spacer->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum ) );
    
    tbOrientation = new pToolButton( this );
    tbOrientation->installEventFilter( this );
    tbFloat = new pToolButton( this );
    tbFloat->installEventFilter( this );
    tbClose = new pToolButton( this );
    tbClose->installEventFilter( this );
    
    addWidget( spacer );
    aOrientation = addWidget( tbOrientation );
    aFloat = addWidget( tbFloat );
    aClose = addWidget( tbClose );
    
    tbOrientation->setDefaultAction( aOrientation );
    tbFloat->setDefaultAction( aFloat );
    tbClose->setDefaultAction( aClose );
    
    setMovable( false );
    setFloatable( false );
    
    updateStyleChange();
    dockWidget_featuresChanged( mDock->features() );
    
    connect( mDock, SIGNAL( featuresChanged( QDockWidget::DockWidgetFeatures ) ), this, SLOT( dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures ) ) );
    connect( aOrientation, SIGNAL( triggered() ), this, SLOT( aOrientation_triggered() ) );
    connect( aFloat, SIGNAL( triggered() ), this, SLOT( aFloat_triggered() ) );
    connect( aClose, SIGNAL( triggered() ), mDock, SLOT( close() ) );
}

QIcon pDockWidgetTitleBar::icon() const
{
    QIcon icon = mDock->toggleViewAction()->icon();
    
    if ( icon.isNull() ) {
        icon = mDock->windowIcon();
    }
    
    if ( icon.isNull() ) {
        icon =  toggleViewAction()->icon();
    }
    
    if ( icon.isNull() ) {
        icon =  windowIcon();
    }
    
    if ( icon.isNull() ) {
        icon = window()->windowIcon();
    }
    
    return icon;
}

QSize pDockWidgetTitleBar::windowIconSize() const
{
    const int size = orientation() == Qt::Horizontal ? height() -2 : width() -2;
    return icon().isNull() ? QSize() : QSize( size, size );
}

bool pDockWidgetTitleBar::event( QEvent* event )
{
    switch ( event->type() ) {
        case QEvent::StyleChange:
            updateStyleChange();
            break;
        default:
            break;
    }
    
    return QToolBar::event( event );
}

bool pDockWidgetTitleBar::eventFilter( QObject* object, QEvent* event )
{
    pToolButton* button = qobject_cast<pToolButton*>( object );
    
    if ( button && event->type() == QEvent::Paint ) {
        QStyleOptionToolButton option;
        button->initStyleOption( &option );
        option.icon = QIcon();
        
        button->paint( &option );
        QPainter painter( button );
        button->icon().paint( &painter, button->rect(), Qt::AlignCenter, QIcon::Normal, QIcon::Off );
        
        event->accept();
        return true;
    }
    
    return QToolBar::eventFilter( object, event );
}

void pDockWidgetTitleBar::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );
    
    QRect rect = this->rect();
    QPainter painter( this );
    
    // native background paint for not common style / native paint
    if ( ( mUseNativePaint || style()->inherits( "QMacStyle" ) || style()->inherits( "Oxygen::Style" ) ) && qobject_cast<pDockWidget*>( mDock ) ) {
        QStyleOptionDockWidgetV2 optionDw;
        ((pDockWidget*)mDock)->initStyleOption( &optionDw );
        optionDw.title.clear();
        optionDw.closable = false;
        optionDw.floatable = false;
        optionDw.movable = false;
        optionDw.rect = rect;
        
        style()->drawControl( QStyle::CE_DockWidgetTitle, &optionDw, &painter, mDock );
    }
    // custom background
    else {
        QColor topColor = palette().color( QPalette::Highlight ).lighter( 130 );
        QColor bottomColor = palette().color( QPalette::Highlight ).darker( 130 );
        QLinearGradient gradient( rect.topLeft(), rect.bottomLeft() );
        
        topColor.setAlphaF( .7 );
        bottomColor.setAlphaF( .7 );
        
        gradient.setColorAt( 0, topColor );
        gradient.setColorAt( 1, bottomColor );
        
        if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar ) {
            gradient.setFinalStop( rect.topRight() );
        }
        
        painter.setPen( Qt::NoPen );
        painter.setBrush( gradient );
        painter.drawRect( rect );
        
        painter.setPen( bottomColor.darker( 130 ) );
        painter.setBrush( Qt::NoBrush );
        painter.drawRect( rect.adjusted( 0, 0, -1, -1 ) );
    }
    
    if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar ) {
        QTransform transform;
        
        rect.setSize( QSize( rect.height(), rect.width() ) );
        transform.rotate( -90 );
        transform.translate( -rect.width(), 0 );
        
        painter.setTransform( transform );
    }
    
    // icon / title
    QStyleOptionButton optionB;
    optionB.initFrom( mDock );
    optionB.rect = rect.adjusted( 2, 0, -( orientation() == Qt::Horizontal ? minimumSizeHint().width() : minimumSizeHint().height() ), 0 );
    optionB.text = mDock->windowTitle();
    optionB.iconSize = windowIconSize();
    optionB.icon = icon();
    
    style()->drawControl( QStyle::CE_PushButtonLabel, &optionB, &painter, mDock );
}

void pDockWidgetTitleBar::updateStyleChange()
{
    setIconSize( QSize( 13, 13 ) );
    layout()->setSpacing( 0 );
    layout()->setMargin( 2 );
    
    QIcon icon;
    
    icon = style()->standardIcon( QStyle::SP_TitleBarShadeButton, 0, widgetForAction( aOrientation ) );
    aOrientation->setIcon( icon );
    
    icon = style()->standardIcon( QStyle::SP_TitleBarNormalButton, 0, widgetForAction( aFloat ) );
    aFloat->setIcon( icon );
    
    icon = style()->standardIcon( QStyle::SP_TitleBarCloseButton, 0, widgetForAction( aClose ) );
    aClose->setIcon( icon );
    
    if ( orientation() == Qt::Horizontal ) {
        tbOrientation->setDirection( QBoxLayout::LeftToRight );
        tbFloat->setDirection( QBoxLayout::LeftToRight );
        tbClose->setDirection( QBoxLayout::LeftToRight );
    }
    else {
        tbOrientation->setDirection( QBoxLayout::BottomToTop );
        tbFloat->setDirection( QBoxLayout::BottomToTop );
        tbClose->setDirection( QBoxLayout::BottomToTop );
    }
}

QSize pDockWidgetTitleBar::minimumSizeHint() const
{
    return QToolBar::sizeHint();
}

QSize pDockWidgetTitleBar::sizeHint() const
{
    const QSize wis = windowIconSize();
    QSize size = QToolBar::sizeHint();
    QFontMetrics fm( font() );

    if ( mDock->features() & QDockWidget::DockWidgetVerticalTitleBar ) {
        size.rheight() += fm.width( mDock->windowTitle() ) +wis.width();
    }
    else {
        size.rwidth() += fm.width( mDock->windowTitle() ) +wis.width();
    }

    return size;
}

QWidget* pDockWidgetTitleBar::addAction( QAction* action, int index )
{
    if ( index != -1 ) {
        index++;
    }
    
    if ( index >= 0 && index < actions().count() ) {
        QToolBar::insertAction( actions().value( index ), action );
    }
    else {
        QToolBar::addAction( action );
    }
    
    return widgetForAction( action );
}

void pDockWidgetTitleBar::addSeparator( int index )
{
    if ( index != -1 ) {
        index++;
    }
    
    if ( index >= 0 && index < actions().count() ) {
        QToolBar::insertSeparator( actions().value( index ) );
    }
    else {
        QToolBar::addSeparator();
    }
}

void pDockWidgetTitleBar::setNativeRendering( bool native )
{
    mUseNativePaint = native;
    update();
}

bool pDockWidgetTitleBar::nativeRendering() const
{
    return mUseNativePaint;
}

void pDockWidgetTitleBar::setDefaultNativeRendering( bool native )
{
    mUseNativePaintDefault = native;
}

bool pDockWidgetTitleBar::defaultNativeRendering()
{
    return mUseNativePaintDefault;
}

void pDockWidgetTitleBar::aOrientation_triggered()
{
    const QDockWidget::DockWidgetFeatures features = mDock->features();
    
    if ( features & QDockWidget::DockWidgetVerticalTitleBar ) {
        mDock->setFeatures( features ^ QDockWidget::DockWidgetVerticalTitleBar );
    }
    else {
        mDock->setFeatures( features | QDockWidget::DockWidgetVerticalTitleBar );
    }
}

void pDockWidgetTitleBar::aFloat_triggered()
{
    mDock->setFloating( !mDock->isFloating() );
}

void pDockWidgetTitleBar::dockWidget_featuresChanged( QDockWidget::DockWidgetFeatures features )
{
    aFloat->setVisible( features & QDockWidget::DockWidgetFloatable );
    aClose->setVisible( features & QDockWidget::DockWidgetClosable );
    
    // update toolbar orientation
    if ( features & QDockWidget::DockWidgetVerticalTitleBar ) {
        if ( orientation() == Qt::Vertical ) {
            return;
        }
        
        setOrientation( Qt::Vertical );
    }
    else {
        if ( orientation() == Qt::Horizontal ) {
            return;
        }
        
        setOrientation( Qt::Horizontal );
    }
    
    // re-order the actions
    const QList<QAction*> actions = this->actions();
    QList<QAction*> items;
    
    for ( int i = actions.count() -1; i > -1; i-- ) {
        items << actions.at( i );
    }
    
    clear();
    addActions( items );
    updateStyleChange();
}
