/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pSpinBox.cpp
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
#include "pSpinBox.h"
#include "ui_pSpinBox.h"

#include <QFrame>
#include <QSlider>

class Slider : public QSlider
{
public:
    Slider( QWidget* parent = 0 )
      : QSlider( parent )
    {
      setSingleStep( 1 );
      setPageStep( 1 );
    }
};

class Frame : public QFrame
{
public:
    Frame( QWidget* parent = 0 )
      : QFrame( parent, Qt::Popup ),
        slider( new Slider( this ) )
    {
      setAttribute( Qt::WA_DeleteOnClose );
      setFrameStyle( QFrame::Sunken | QFrame::StyledPanel );

      QHBoxLayout* layout = new QHBoxLayout( this );
      layout->setMargin( frameWidth() );
      layout->setSpacing( 0 );
      layout->addWidget( slider );
    }

public:
    Slider* slider;
};

pSpinBox::pSpinBox( QWidget* parent )
    : QWidget( parent )
    , ui( new Ui::pSpinBox )
    , sliderFactor_( 8 )
    , textVisible_( true )
{
    ui->setupUi( this );
    setCheckable( false );

    connect( ui->cbCheck, SIGNAL( pressed() ), this, SIGNAL( pressed() ) );
    connect( ui->cbCheck, SIGNAL( released() ), this, SIGNAL( released() ) );
    connect( ui->cbCheck, SIGNAL( clicked( bool ) ), this, SIGNAL( clicked( bool ) ) );
    connect( ui->cbCheck, SIGNAL( toggled( bool ) ), this, SIGNAL( toggled( bool ) ) );
    connect( ui->sbSpinBox, SIGNAL( editingFinished() ), this, SIGNAL( editingFinished() ) );
    connect( ui->sbSpinBox, SIGNAL( valueChanged( int ) ), this, SIGNAL( valueChanged( int ) ) );
    connect( ui->sbSpinBox, SIGNAL( valueChanged( const QString& ) ), this, SIGNAL( valueChanged( const QString& ) ) );
}

pSpinBox::~pSpinBox()
{
    delete ui;
}

int pSpinBox::sliderFactor() const
{
    return sliderFactor_;
}

void pSpinBox::setSliderFactor( int factor )
{
    sliderFactor_ = factor;
}

bool pSpinBox::isCheckable() const
{
    return ui->cbCheck->isCheckable();
}

void pSpinBox::setCheckable( bool checkable )
{
    ui->cbCheck->setCheckable( checkable );
    ui->cbCheck->setVisible( ui->cbCheck->isCheckable() );
    on_cbCheck_toggled( ui->cbCheck->isChecked() );
}

bool pSpinBox::isChecked() const
{
    return ui->cbCheck->isChecked();
}

void pSpinBox::setChecked( bool checked )
{
    ui->cbCheck->setChecked( checked );
}

bool pSpinBox::isTextVisible() const
{
    return textVisible_;
}

void pSpinBox::setTextVisible( bool visible )
{
    textVisible_ = visible;
    ui->cbCheck->setText( textVisible_ ? text_ : QString::null );
}

QString pSpinBox::text() const
{
    return text_;
}

void pSpinBox::setText( const QString& text )
{
    text_ = text;
    ui->cbCheck->setText( textVisible_ ? text_ : QString::null );
}

int pSpinBox::minimum() const
{
    return ui->sbSpinBox->minimum();
}

void pSpinBox::setMinimum( int value )
{
    ui->sbSpinBox->setMinimum( value );
}

int pSpinBox::maximum() const
{
    return ui->sbSpinBox->maximum();
}

void pSpinBox::setMaximum( int value )
{
    ui->sbSpinBox->setMaximum( value );
}

int pSpinBox::value() const
{
    return ui->sbSpinBox->value();
}

void pSpinBox::setValue( int value )
{
  ui->sbSpinBox->setValue( value );
}

void pSpinBox::setDefaultValue( int value )
{
  const bool locked = blockSignals( true );
  ui->sbSpinBox->setValue( value );
  blockSignals( locked );
}

QCheckBox* pSpinBox::checkBox() const
{
  return ui->cbCheck;
}

QSpinBox* pSpinBox::spinBox() const
{
    return ui->sbSpinBox;
}

QToolButton* pSpinBox::toolButton() const
{
    return ui->tbToolButton;
}

void pSpinBox::sliderAboutToShow( QSlider* slider )
{
    slider->setMinimum( ui->sbSpinBox->minimum() /sliderFactor_ );
    slider->setMaximum( ui->sbSpinBox->maximum() /sliderFactor_ );
    slider->setValue( ui->sbSpinBox->value() );

    connect( ui->sbSpinBox, SIGNAL( valueChanged( int ) ), slider, SLOT( setValue( int ) ) );
    connect( slider, SIGNAL( valueChanged( int ) ), ui->sbSpinBox, SLOT( setValue( int ) ) );
    connect( slider, SIGNAL( sliderMoved( int ) ), ui->sbSpinBox, SLOT( setValue( int ) ) );
}

void pSpinBox::on_tbToolButton_clicked()
{
    Frame* frame = new Frame( this );

    sliderAboutToShow( frame->slider );

    QSize sh = frame->sizeHint();
    sh.rwidth() *= frame->slider->orientation() == Qt::Horizontal ? 2 : 1;
    sh.rheight() *= frame->slider->orientation() == Qt::Vertical ? 2 : 1;
    frame->resize( sh );

    QRect rect = QRect( QPoint(), frame->size() );
    rect.moveCenter( ui->tbToolButton->mapToGlobal( ui->tbToolButton->rect().center() ) );
    rect.moveTop( ui->tbToolButton->mapToGlobal( ui->tbToolButton->rect().bottomLeft() ).y() +2 );

    frame->move( rect.topLeft() );
    frame->show();
}

void pSpinBox::on_cbCheck_toggled( bool checked )
{
    ui->wContainer->setEnabled( !isCheckable() || checked );
}
