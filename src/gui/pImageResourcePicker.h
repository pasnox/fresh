/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pImageResourcePicker.h
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
#ifndef PIMAGERESOURCEPICKER_H
#define PIMAGERESOURCEPICKER_H

/*!
    \file pImageResourcePicker.h
    \brief A widget that allow to select one or more images from differnts sources (file path, QtResource, Icon theme).
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include "pResourcePicker.h"

// TODO: Finish documentation

/*!
    \ingroup FreshGui
    \class pImageResourcePicker
    \brief A widget that allow to select one or more images from differnts sources (file path, QtResource, Icon theme).
*/

class FRESH_EXPORT pImageResourcePicker : public pResourcePicker
{
  Q_OBJECT

public:
  explicit pImageResourcePicker( QWidget* parent = 0 );
};

#endif // PIMAGERESOURCEPICKER_H
