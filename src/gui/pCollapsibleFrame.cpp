/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pCollapsibleFrame.cpp
** Date      : 2011-02-20T00:41:35
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
#include "pCollapsibleFrame.h"

#include <QVBoxLayout>
#include <QToolButton>
#include <QPointer>
#include <QTimer>
#include <QIcon>
#include <QStyle>
#include <QChildEvent>
#include <QDebug>

class pCollapsibleFramePrivate: public QObject {
    Q_OBJECT

public:
    pCollapsibleFrame* editor;
    QVBoxLayout* layout;
    QToolButton* button;
    QPointer<QWidget> widget;
    QSize oldExpandedSize;
    QIcon collapsedIcon;
    QIcon expandedIcon;
    QString collapsedText;
    QString expandedText;
    bool collapsed;
    int indentation;

public:
    pCollapsibleFramePrivate( pCollapsibleFrame* _editor )
        : QObject( _editor ),
          editor( _editor ),
          layout( new QVBoxLayout( editor ) ),
          button( new QToolButton( editor ) ),
          collapsed( false ),
          indentation( 0 )
    {
        button->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        button->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
        button->setCheckable( true );

        layout->setContentsMargins( indentation, 3, 0, 0 );
        layout->setMenuBar( button );

        connect( button, SIGNAL( toggled( bool ) ), this, SLOT( button_toggled( bool ) ) );
    }

public slots:
    void button_toggled( bool toggled ) {
        QFont font = button->font();
        oldExpandedSize = collapsed ? oldExpandedSize : editor->size();
        collapsed = !toggled;

        font.setBold( toggled );

        if ( collapsed ) {
          emit editor->aboutToCollapse();
        }
        else {
          emit editor->aboutToExpand();
        }

        button->setIcon( collapsed ?  editor->collapsedIcon() : editor->expandedIcon() );
        button->setText( collapsed ?  editor->collapsedText() : editor->expandedText() );
        button->setChecked( toggled );
        button->setFont( font );

        if ( widget ) {
            widget->setVisible( toggled );
        }

        if ( !editor->parentWidget() ) {
            if ( collapsed ) {
                QTimer::singleShot( 0, this, SLOT( updateCollapsedSize() ) );
            }
            else {
                QTimer::singleShot( 0, this, SLOT( updateExpandedSize() ) );
            }
        }
        else {
          if ( collapsed ) {
            emit editor->collapsed();
          }
          else {
            emit editor->expanded();
          }
        }
    }

    void updateCollapsedSize() {
        layout->invalidate();
        editor->resize( editor->minimumSizeHint() );
        emit editor->collapsed();
    }

    void updateExpandedSize() {
        layout->invalidate();
        editor->resize( oldExpandedSize );
        emit editor->expanded();
    }
};

pCollapsibleFrame::pCollapsibleFrame( QWidget* parent )
    : QFrame(parent),
      d( new pCollapsibleFramePrivate( this ) )
{
    d->button_toggled( d->button->isChecked() );
}

QSize pCollapsibleFrame::minimumSizeHint() const
{
    const QMargins margins = d->layout->contentsMargins();
    QSize sh( d->button->minimumSizeHint() );

    sh.rwidth() += margins.left() +margins.right();
    sh.rheight() += margins.top() +margins.bottom();

    if ( d->collapsed ) {
    }
    else if ( d->widget ) {
        int spacing = d->layout->spacing();

        if ( spacing == -1 ) {
            spacing = style()->pixelMetric( QStyle::PM_LayoutVerticalSpacing );
        }

        if ( spacing == -1 ) {
            spacing = style()->layoutSpacing( d->button->sizePolicy().controlType(), d->widget->sizePolicy().controlType(), Qt::Vertical, 0, this );
        }

        sh.rwidth() = qMax( sh.width(), d->widget->minimumSizeHint().width() );
        sh.rheight() += d->widget->minimumSizeHint().height();
        sh.rheight() += spacing;
    }

    return sh;
}

QWidget* pCollapsibleFrame::widget() const
{
    return d->widget;
}

QWidget* pCollapsibleFrame::takeWidget()
{
    if ( !d->widget ) {
        return 0;
    }

    QWidget* widget = d->widget;
    d->widget = 0;
    d->layout->removeWidget( widget );
    widget->setParent( 0 );

    d->button_toggled( d->button->isChecked() );

    return widget;
}

void pCollapsibleFrame::setWidget( QWidget* widget )
{
    delete takeWidget();

    if ( widget ) {
        d->widget = widget;
        d->layout->addWidget( widget );
        d->button_toggled( d->button->isChecked() );
    }
}

QIcon pCollapsibleFrame::collapsedIcon() const
{
    return d->collapsedIcon.isNull() ? style()->standardIcon( QStyle::SP_ArrowRight ) : d->collapsedIcon;
}

QIcon pCollapsibleFrame::expandedIcon() const
{
    return d->expandedIcon.isNull() ? style()->standardIcon( QStyle::SP_ArrowDown ) : d->expandedIcon;
}

QString pCollapsibleFrame::collapsedText() const
{
    return d->collapsedText.isEmpty() ?  tr( "Collapsed" ) : d->collapsedText;
}

QString pCollapsibleFrame::expandedText() const
{
    return d->collapsedText.isEmpty() ? tr( "Expanded" ) : d->collapsedText;
}

bool pCollapsibleFrame::isCollapsed() const
{
    return d->collapsed;
}

bool pCollapsibleFrame::isExpanded() const
{
    return !isCollapsed();
}

int pCollapsibleFrame::indentation() const
{
    return d->layout->contentsMargins().left();
}

void pCollapsibleFrame::setCollapsedIcon( const QIcon& icon )
{
    d->collapsedIcon = icon;

    if ( d->collapsed ) {
        d->button->setIcon( collapsedIcon() );
    }
}

void pCollapsibleFrame::setExpandedIcon( const QIcon& icon )
{
    d->expandedIcon = icon;

    if ( !d->collapsed ) {
        d->button->setIcon( expandedIcon() );
    }
}

void pCollapsibleFrame::setIcon( const QIcon& icon )
{
    setCollapsedIcon( icon );
    setExpandedIcon( icon );
}

void pCollapsibleFrame::setCollapsedText( const QString& text )
{
    d->collapsedText = text;

    if ( d->collapsed ) {
        d->button->setText( collapsedText() );
    }
}

void pCollapsibleFrame::setExpandedText( const QString& text )
{
    d->expandedText = text;

    if ( !d->collapsed ) {
        d->button->setText( expandedText() );
    }
}

void pCollapsibleFrame::setText( const QString& text )
{
    setCollapsedText( text );
    setExpandedText( text );
}

void pCollapsibleFrame::setCollapsed( bool collapsed )
{
    if ( d->collapsed == collapsed ) {
        return;
    }

    d->button->setChecked( !collapsed );
}

void pCollapsibleFrame::setExpanded( bool expanded )
{
    setCollapsed( !expanded );
}

void pCollapsibleFrame::setIndentation( int indentation )
{
    QMargins margins = d->layout->contentsMargins();
    margins.setLeft( indentation );
    d->layout->setContentsMargins( margins );
}

void pCollapsibleFrame::childEvent( QChildEvent* event )
{
    QFrame::childEvent( event );

    if ( event->polished() && !widget() ) {
        if ( event->child() != d->button ) {
            setWidget( qobject_cast<QWidget*>( event->child() ) );
        }
    }
}

#include "pCollapsibleFrame.moc"
