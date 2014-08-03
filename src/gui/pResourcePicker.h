/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pResourcePicker.h
** Date      : 2011-02-20T00:44:25
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
#ifndef PRESOURCEPICKER_H
#define PRESOURCEPICKER_H

/*!
    \file pResourcePicker.h
    \brief A widget that allow to select one or more file from differnts sources (file path, QtResource, Icon theme).
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QWidget>
#include <QFlags>
#include <QFileIconProvider>

namespace Ui {
  class pResourcePicker;
}

// TODO: Finish documentation

/*!
    \ingroup FreshGui
    \class pResourcePicker
    \brief A widget that allow to select one or more file from differnts sources (file path, QtResource, Icon theme).
*/
class FRESH_EXPORT pResourcePicker : public QWidget
{
  Q_OBJECT

public:
  enum Action {
    ActionNone = 0,
    ActionFilePath = 0x1,
    ActionResource = 0x2,
    ActionTheme = 0x4
  };

  Q_DECLARE_FLAGS( Actions, Action )

  explicit pResourcePicker( QWidget* parent = 0 );
  virtual ~pResourcePicker();

  pResourcePicker::Actions enabledActions() const;
  void setEnabledActions( pResourcePicker::Actions actions );

  bool preferUri() const;
  void setPreferUri( bool prefer );

  bool isUpscaleAllowed() const;
  void setUpscaleAllowed( bool upscaleAllowed );

  bool isPreviewEnabled() const;
  void setPreviewEnabled( bool enabled );

  QStringList fileNameWildcards() const;
  void setFileNameWildcards( const QStringList& wildcards );

  QString text() const;
  void setText( const QString& text );

  QString referenceLocalFilePath() const;
  void setReferenceLocalFilePath( const QString& filePath );

  static QString toUri( const QString& filePath );
  static QString fromUri( const QString& uri );
  static QPixmap pixmapFromString( const QString& string, const QSize& size, bool upscaleAllowed = true, Qt::AspectRatioMode aspect = Qt::KeepAspectRatio, Qt::TransformationMode transformation = Qt::SmoothTransformation );

private:
  Ui::pResourcePicker* ui;
  bool preferUri_;
  bool upscaleAllowed_;
  QStringList fileNameWildcards_;
  QString referenceLocalFilePath_;

  static QFileIconProvider iconProvider_;

  QString fileNameFilter() const;
  void updatePreview();

private slots:
  void on_leResource_textChanged( const QString& text );
  void on_leResource_textEdited( const QString& text );
  void on_aFilePath_triggered();
  void on_aResource_triggered();
  void on_aTheme_triggered();

signals:
  void customPreview( const QString& text, const QSize& size, QPixmap& preview );
  void editingFinished();
  void returnPressed();
  void previewUpdated();
  void textChanged( const QString& text );
  void textEdited( const QString& text );
};

Q_DECLARE_OPERATORS_FOR_FLAGS( pResourcePicker::Actions )

#endif // PRESOURCEPICKER_H
