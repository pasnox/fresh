/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pSpinBox.h
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
#ifndef PSPINBOX_H
#define PSPINBOX_H

/*!
    \file pSpinBox.h
    \brief A mixed QSpinBox/QSlider widget.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QWidget>

namespace Ui {
  class pSpinBox;
}

class QCheckBox;
class QLabel;
class QSpinBox;
class QToolButton;
class QSlider;

// TODO: Finish documentation
// TODO: Refactor the class so it inherits QSpinBox directly.

/*!
    \ingroup FreshGui
    \class pSpinBox
    \brief A mixed QSpinBox/QSlider widget.

    This widget is a special QSpinBox like widget.
    It has the possibility to be checkable, to have a label and change its value using a QSlider.
    When the pSpinBox is checkable, the QSpinBox/QSlider will be enabled according to the checked state.
*/
class FRESH_EXPORT pSpinBox : public QWidget
{
    Q_OBJECT

public:
    pSpinBox( QWidget* parent = 0 );
    virtual ~pSpinBox();

    int sliderFactor() const;
    void setSliderFactor( int factor );

    bool isCheckable() const;
    void setCheckable( bool checkable );

    bool isChecked() const;
    void setChecked( bool checked );

    bool isTextVisible() const;
    void setTextVisible( bool visible );

    QString text() const;
    void setText( const QString& text );

    int minimum() const;
    void setMinimum( int value );

    int maximum() const;
    void setMaximum( int value );

    int value() const;
    void setValue( int value );
    void setDefaultValue( int value );

protected:
    QCheckBox* checkBox() const;
    QLabel* label() const;
    QSpinBox* spinBox() const;
    QToolButton* toolButton() const;

    virtual void sliderAboutToShow( QSlider* slider );

private slots:
    void on_tbToolButton_clicked();
    void on_cbCheck_toggled( bool checked) ;

private:
    Ui::pSpinBox* ui;
    int sliderFactor_;

signals:
    void editingFinished();
    void valueChanged( int i );
    void valueChanged( const QString& text );
    void pressed();
    void released();
    void clicked( bool checked = false );
    void toggled( bool checked );
};

#endif // PSPINBOX_H
