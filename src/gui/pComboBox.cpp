/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pComboBox.cpp
** Date      : 2012-11-20T00:44:25
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
#include "pComboBox.h"

#include <QToolButton>
#include <QHBoxLayout>
#include <QWheelEvent>
#include <QStylePainter>

#if defined( Q_OS_MACX )
bool isInQAbstractItemView( const QWidget* widget ) {
  const QWidget* parent = widget ? widget->parentWidget() : 0;
  parent = parent ? parent->parentWidget() : 0;
  return parent ? parent->inherits( "QAbstractItemView" ) : false;
}

int truncatedSizeHintHeight( const QWidget* widget, bool noQAV ) {
  if ( widget && ( noQAV ? !isInQAbstractItemView( widget ) : true ) ) {
    if ( widget->testAttribute( Qt::WA_MacMiniSize ) ) {
      return 9;
    }
    else if ( widget->testAttribute( Qt::WA_MacSmallSize ) ) {
      return 8;
    }
    else /*if ( widget->testAttribute( Qt::WA_MacNormalSize ) )*/ {
      return 5;
    }
  }

  return 0;
}
#else
int truncatedSizeHintHeight( const QWidget* widget, bool noQAV ) {
  Q_UNUSED( widget );
  Q_UNUSED( noQAV );
  return 0;
}
#endif

// ResetToolButton

class ResetToolButton : public QToolButton {
  Q_OBJECT

public:
    ResetToolButton( pComboBox* comboBox )
    : QToolButton( comboBox )
    , m_comboBox( comboBox )
    {
        Q_ASSERT( comboBox );

        setIcon( QIcon::fromTheme( QLatin1String( "edit-clear-locationbar-rtl" ) ) );
        setToolTip( QLatin1String( "Clear selected index" ) );
    }

    virtual QSize minimumSizeHint() const {
        if ( m_comboBox ) {
            const int pheight = m_comboBox->QComboBox::minimumSizeHint().height() -truncatedSizeHintHeight( m_comboBox, false );
            return QSize( pheight, pheight );
        }

        return QToolButton::minimumSizeHint();
    }

    virtual QSize sizeHint() const {
        if ( m_comboBox ) {
            const int pheight = m_comboBox->QComboBox::sizeHint().height() -truncatedSizeHintHeight( m_comboBox, false );
            return QSize( pheight, pheight );
        }

        return QToolButton::sizeHint();
    }

private:
    pComboBox* m_comboBox;
};

// pComboBox

pComboBox::pComboBox( QWidget* parent )
    : QComboBox( parent )
    , spacing_( 1 )
    , toolButton_( new ResetToolButton( this ) )
{
    QHBoxLayout* hl = new QHBoxLayout( this );
    hl->setMargin( 0 );
    hl->setSpacing( spacing_ );
    hl->addStretch();
    hl->addWidget( toolButton_ );

    connect( toolButton_, SIGNAL( clicked() ), this, SLOT( reset() ) );
}

void pComboBox::hidePopup()
{
    emit popupAboutToHide();
    QComboBox::hidePopup();
}

void pComboBox::showPopup()
{
    emit popupAboutToShow();
    QComboBox::showPopup();
}

QSize pComboBox::sizeHint() const
{
    QSize sh = QComboBox::sizeHint();
    sh.rheight() -= truncatedSizeHintHeight( this, true );

    if ( isResetButtonEnabled() ) {
        sh.setWidth( sh.width() +spacing_ +toolButton_->sizeHint().width() );
    }

    return sh;
}

QSize pComboBox::minimumSizeHint() const
{
    QSize sh = QComboBox::minimumSizeHint();
    sh.rheight() -= truncatedSizeHintHeight( this, true );

    if ( isResetButtonEnabled() ) {
        sh.setWidth( sh.width() +spacing_ +toolButton_->minimumSizeHint().width() );
    }

    return sh;
}

bool pComboBox::isResetButtonEnabled() const
{
    return toolButton_->isEnabled();
}

void pComboBox::setResetButtonEnabled( bool enabled )
{
    toolButton_->setEnabled( enabled );
    toolButton_->setVisible( enabled );
    updateGeometry();
}

void pComboBox::paintEvent( QPaintEvent* event )
{
    Q_UNUSED( event );

    QStyleOptionComboBox opt;
    initStyleOption( &opt );

    paint( opt );
}

void pComboBox::mousePressEvent( QMouseEvent* event )
{
    if ( hitComboBox( event->pos() ) ) {
        QComboBox::mousePressEvent( event );
    }
}

void pComboBox::mouseReleaseEvent( QMouseEvent* event )
{
    if ( hitComboBox( event->pos() ) ) {
        QComboBox::mouseReleaseEvent( event );
    }
}

void pComboBox::wheelEvent( QWheelEvent* event )
{
    if ( hitComboBox( event->pos() ) ) {
        QComboBox::wheelEvent( event );
    }
}

bool pComboBox::hitComboBox( const QPoint& pos ) const
{
    if ( isResetButtonEnabled() ) {
        return rect().adjusted( 0, 0, -( spacing_ +toolButton_->size().width() ), 0 ).contains( pos );
    }
    else {
        return true;
    }
}

void pComboBox::paint( QStyleOptionComboBox& opt )
{
    QStylePainter painter( this );
    painter.setPen( palette().color( QPalette::Text ) );

    // fix width
    if ( isResetButtonEnabled() ) {
        opt.rect = opt.rect.adjusted( 0, 0, -( spacing_ +toolButton_->size().width() ), 0 );
    }

#if defined( Q_OS_MACX )
    // Fix left/right positions
    if ( testAttribute( Qt::WA_MacMiniSize ) ) {
        opt.rect.adjust( -3, 0, 3, 0 );
    }
    else if ( testAttribute( Qt::WA_MacSmallSize ) ) {
        opt.rect.adjust( -3, 0, 2, 0 );
    }
    else /*if ( testAttribute( Qt::WA_MacNormalSize ) )*/ {
        opt.rect.adjust( -2, 0, 2, 0 );
    }

    if ( !isInQAbstractItemView( this ) ) {
        // Fix top/bottom positions
        if ( testAttribute( Qt::WA_MacMiniSize ) ) {
            opt.rect.adjust( 0, 1, 0, 2 );
        }
        else if ( testAttribute( Qt::WA_MacSmallSize ) ) {
            opt.rect.adjust( 0, 0, 0, 0 );
        }
        else /*if ( testAttribute( Qt::WA_MacNormalSize ) )*/ {
            opt.rect.adjust( 0, -3, 0, 2 );
        }
    }
#endif

    // draw the combobox frame, focusrect and selected etc.
    painter.drawComplexControl( QStyle::CC_ComboBox, opt );
    // draw the icon and text
#if defined( Q_OS_MACX )
    if ( !isInQAbstractItemView( this ) ) {
        // Fix bad valignment for mini size label
        if ( testAttribute( Qt::WA_MacMiniSize ) ) {
            opt.rect.adjust( 0, -3, 0, 3 );
        }
    }
#endif
    painter.drawControl( QStyle::CE_ComboBoxLabel, opt );
}

void pComboBox::reset()
{
    if ( currentIndex() != -1 ) {
        setCurrentIndex( -1 );
        emit activated( -1 );
    }
}

#include "pComboBox.moc"
