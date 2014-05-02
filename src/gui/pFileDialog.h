/****************************************************************************
**
**      Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe Azevedo aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pFileDialog.h
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
#ifndef PFILEDIALOG_H
#define PFILEDIALOG_H

/*!
    \file pFileDialog.h
    \brief An extended QFileDialog.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QFileDialog>
#include <QMap>
#include <QVariant>

class QGridLayout;
class QLabel;
class QComboBox;
class QCheckBox;

/*!
    \ingroup FreshGui
    \class pFileDialogResult
    \brief Result object of pFileDialog.

    This file dialog add support for some new features like Text Codec choosing, Open in read only mode.
    The results are stocked in a dedicated class that can be read using \a pFileDialog::Property or helpers.
*/
class FRESH_EXPORT pFileDialogResult : public QMap<int, QVariant>
{
public:
    /*!
        Return the selected text codec.
    */
    QString textCodec() const;
    /*!
        Return true if read only has been checked else false.
    */
    bool readOnly() const;
    /*!
        Return the selected directory.
    */
    QString directory() const;
    /*!
        Return the selected file name.
    */
    QString fileName() const;
    /*!
        Return the selected file names.
    */
    QStringList fileNames() const;
    /*!
        Return the selected filter.
    */
    QString selectedFilter() const;
};

/*!
    \ingroup FreshGui
    \class pFileDialog
    \brief An extended QFileDialog.

    This file dialog add support for some new features like Text Codec choosing, Open in read only mode.
    The results are stocked in a dedicated structure that can be read using \a pFileDialog::Property.
*/
class FRESH_EXPORT pFileDialog : public QFileDialog
{
    Q_OBJECT
    
public:
    /*! Thie enumeration represent the possible properties to query. */
    enum Property {
        TextCodec = 0, /*!< The text codec choosed by user (QString). */
        ReadOnly, /*!< The open in read only state (bool). */
        Directory, /*!< The directory choosed by the user (QString). */
        FileName, /*!< The file name choosed by the user (QString) */
        FileNames, /*!< The file names choosed by the user (QStringList). */
        SelectedFilter /*!< The selected filter choosed by the user (QString). */
    };
    
    /*!
        Create an instance of the file dialog having parent \a parent and window title \a caption.
        Depending the QFileDialog::FileMode \a directory will represent the default path or file path.
        The selected filter will be \a filter.
        The text codec choice and read only state will be enabled according to \a textCodecEnabled and \a openReadOnlyEnabled.
    */
    pFileDialog( QWidget* parent = 0, const QString& caption = QString::null, const QString& directory = QString::null, const QString& filter = QString::null, bool textCodecEnabled = true, bool openReadOnlyEnabled = false );
    /*!
        Return the current text codec.
    */
    QString textCodec() const;
    /*!
        Set the current text codec to \a codec.
    */
    void setTextCodec( const QString& codec );
    /*!
        Return true if the text codec choice is enabled, else false.
    */
    bool textCodecEnabled() const;
    /*!
        Enable the text codec choice according to \a enabled.
    */
    void setTextCodecEnabled( bool enabled );
    /*!
        Return true if open in read only is checked, else false.
    */
    bool openReadOnly() const;
    /*!
        Set the open in read only check state according to \a readOnly.
    */
    void setOpenReadOnly( bool readOnly );
    /*!
        Return true if the open in read only mode is enabled, else false.
    */
    bool openReadOnlyEnabled() const;
    /*!
        Return true if the open in read only mode is enabled, else false.
    */
    void setOpenReadOnlyEnabled( bool enabled );
    
    /*!
        This is a convenience static function that will return an existing directory selected by the user.
        
        \code
        QString dir = pFileDialog::getExistingDirectory( this, tr( "Open Directory" ),
                                                    "/home",
                                                    false,
                                                    false,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
        \endcode
        
        This function creates a modal file dialog with the given \a parent widget.
        If parent is not 0, the dialog will be shown centered over the parent widget.
        
        The dialog's working directory is set to \a dir, and the caption is set to \a caption.
        Either of these may be an empty string in which case the current directory and a default caption will be used respectively.
        
        The options argument holds various options about how to run the dialog, see the QFileDialog::Option enum for more information on the flags you can pass.
        To ensure a native file dialog, ShowDirsOnly must be set.
        
        \note This use a QFileDialog so the native dialog will never be shown even when asking the correct flag for that.
        On Windows if \a parent is not 0 then it will position the dialog just below the parent's title bar.
        On Unix/X11, the normal behavior of the file dialog is to resolve and follow symlinks.
        For example, if /usr/tmp is a symlink to /var/tmp, the file dialog will change to /var/tmp after entering /usr/tmp.
        If options includes DontResolveSymlinks, the file dialog will treat symlinks as regular directories.
        
        \sa getOpenFileName(), getOpenFileNames(), and getSaveFileName().
    */
    static pFileDialogResult getExistingDirectory( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, QFileDialog::Options options = QFileDialog::ShowDirsOnly );
    /*!
        This is a convenience static function that returns an existing file selected by the user.
        
        \code
        QString fileName = pFileDialog::getOpenFileName( this, tr( "Open File" ),
                                                    "/home",
                                                    tr( "Images (*.png *.xpm *.jpg)" ),
                                                    true, true, QString::null,
                                                    QString::null, 0 );
        \endcode
        
        The function creates a modal file dialog with the given \a parent widget.
        If \a parent is not 0, the dialog will be shown centered over the parent widget.
        
        The file dialog's working directory will be set to \a dir.
        If \a dir includes a file name, the file will be selected.
        Only files that match the given \a filter are shown.
        The filter selected is set to \a selectedFilter.
        The parameters \a dir, \a selectedFilter, and \a filter may be empty strings.
        If you want multiple filters, separate them with ';;', for example:
        
        \code
        "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"
        \endcode
        
        The \a options argument holds various options about how to run the dialog, see the QFileDialog::Option enum for more information on the flags you can pass.
        The dialog's caption is set to \a caption. If \a caption is not specified then a default caption will be used.
        
        \note This use a QFileDialog so the native dialog will never be shown even when asking the correct flag for that.
        On Windows if \a parent is not 0 then it will position the dialog just below the parent's title bar.
        On Unix/X11, the normal behavior of the file dialog is to resolve and follow symlinks.
        For example, if /usr/tmp is a symlink to /var/tmp, the file dialog will change to /var/tmp after entering /usr/tmp.
        If options includes DontResolveSymlinks, the file dialog will treat symlinks as regular directories.
        
        \sa getOpenFileNames(), getSaveFileName(), and getExistingDirectory().
    */
    static pFileDialogResult getOpenFileName( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, const QString& textCodec = QString::null, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
    /*!
        This is a convenience static function that will return one or more existing files selected by the user.
        
        \code
        QStringList files = pFileDialog::getOpenFileNames( this, tr( "Select one or more files to open" ),
                                                    "/home", tr( "Images (*.png *.xpm *.jpg)" ),
                                                    true, true, QString::null,
                                                    QString::null, 0 );
        \endcode
        
        This function creates a modal file dialog with the given \a parent widget.
        If \a parent is not 0, the dialog will be shown centered over the parent widget.
        
        The file dialog's working directory will be set to \a dir. If \a dir includes a file name, the file will be selected.
        The filter is set to \a filter so that only those files which match the filter are shown.
        The filter selected is set to \a selectedFilter.
        
        The parameters \a dir, \a selectedFilter and \a filter may be empty strings.
        If you need multiple filters, separate them with ';;', for instance:
        
        \code
        "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"
        \endcode
        
        The dialog's caption is set to \a caption.
        If \a caption is not specified then a default caption will be used.
        
        \note This use a QFileDialog so the native dialog will never be shown even when asking the correct flag for that.
        On Windows if \a parent is not 0 then it will position the dialog just below the parent's title bar.
        On Unix/X11, the normal behavior of the file dialog is to resolve and follow symlinks.
        For example, if /usr/tmp is a symlink to /var/tmp, the file dialog will change to /var/tmp after entering /usr/tmp.
        The options argument holds various options about how to run the dialog, see the QFileDialog::Option enum for more information on the flags you can pass.
        
        \sa getOpenFileName(), getSaveFileName(), and getExistingDirectory().
    */
    static pFileDialogResult getOpenFileNames( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, const QString& textCodec = QString::null, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
    /*!
        This is a convenience static function that will return a file name selected by the user.
        The file does not have to exist.
        
        It creates a modal file dialog with the given \a parent widget.
        If \a parent is not 0, the dialog will be shown centered over the parent widget.
        
        \code
        QString fileName = pFileDialog::getSaveFileName( this, tr( "Save File" ),
                                                "/home/jana/untitled.png",
                                                tr( "Images (*.png *.xpm *.jpg)" ),
                                                true, QString::null, QString::null, 0 );
        \endcode
        
        The file dialog's working directory will be set to \a dir.
        If \a dir includes a file name, the file will be selected.
        Only files that match the \a filter are shown.
        The filter selected is set to \a selectedFilter.
        The parameters \a dir, \a selectedFilter, and \a filter may be empty strings.
        Multiple filters are separated with ';;'. For instance:
        
        \code
        "Images (*.png *.xpm *.jpg);;Text files (*.txt);;XML files (*.xml)"
        \endcode
        
        The \æ options argument holds various options about how to run the dialog, see the QFileDialog::Option enum for more information on the flags you can pass.
        The default filter can be chosen by setting \a selectedFilter to the desired value.
        The dialog's caption is set to \a caption.
        If \a caption is not specified, a default caption will be used.
        
        \note This use a QFileDialog so the native dialog will never be shown even when asking the correct flag for that.
        On Windows if parent is not 0 then it will position the dialog just below the parent's title bar.
        On Unix/X11, the normal behavior of the file dialog is to resolve and follow symlinks.
        For example, if /usr/tmp is a symlink to /var/tmp, the file dialog will change to /var/tmp after entering /usr/tmp.
        If \a options includes DontResolveSymlinks the file dialog will treat symlinks as regular directories.
        
        \sa getOpenFileName(), getOpenFileNames(), and getExistingDirectory().
    */
    static pFileDialogResult getSaveFileName( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, const QString& textCodec = QString::null, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );

protected:
    QGridLayout* glDialog;
    bool mTextCodecEnabled;
    QLabel* lCodec;
    QComboBox* cbCodec;
    bool mOpenReadOnlyEnabled;
    QCheckBox* cbOpenReadOnly;
    
    static void setDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, const QString& selectedFilter, QFileDialog::FileMode mode, QFileDialog::Options options );
};

#endif // PFILEDIALOG_H
