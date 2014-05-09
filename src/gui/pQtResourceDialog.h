/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pQtResourceDialog.h
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
#ifndef PQTRESOURCEDIALOG_H
#define PQTRESOURCEDIALOG_H

/*!
    \file pQtResourceDialog.h
    \brief A QtResource file dialog mimicing QFileDialog.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QDialog>

class pQtResourceModel;
class QTreeView;
class QLabel;

namespace Ui {
  class pQtResourceDialog;
}

// TODO: Finish documentation

/*!
    \ingroup FreshGui
    \class pSpinBox
    \brief A QtResource file dialog mimicing QFileDialog.
*/
class FRESH_EXPORT pQtResourceDialog : public QDialog
{
  Q_OBJECT

public:
  explicit pQtResourceDialog( QWidget* parent = 0 );
  ~pQtResourceDialog();

  pQtResourceModel* model() const;
  QTreeView* view() const;
  QLabel* label() const;

  QStringList selectedFiles() const;
  void setSelectedFiles( const QStringList& files );

  QString selectedFile() const;
  void setSelectedFile( const QString& file );

  static QString getExistingDirectory( QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString() );
  static QString getOpenFileName( QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QStringList& filters = QStringList() );
  static QStringList getOpenFileNames( QWidget* parent = 0, const QString& caption = QString(), const QString& dir = QString(), const QStringList& filters = QStringList() );

private slots:
  void view_selectionModel_selectionChanged();

private:
  Ui::pQtResourceDialog* ui;
  pQtResourceModel* m_model;
};

#endif // PQTRESOURCEDIALOG_H
