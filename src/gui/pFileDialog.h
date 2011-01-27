#ifndef PFILEDIALOG_H
#define PFILEDIALOG_H

/*!
	\file pFileDialog.h
	\brief An extended QFileDialog.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"

#include <QFileDialog>
#include <QMap>
#include <QVariant>

class QGridLayout;
class QLabel;
class QComboBox;
class QCheckBox;

typedef QMap<int, QVariant> pFileDialogResult;

/*!
	\ingroup Gui
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
		OpenReadOnly, /*!< The open in read only state (bool). */ 
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
	
	static pFileDialogResult getExistingDirectory( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, QFileDialog::Options options = QFileDialog::ShowDirsOnly );
	static pFileDialogResult getOpenFileName( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
	static pFileDialogResult getOpenFileNames( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
	static pFileDialogResult getSaveFileName( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );

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
