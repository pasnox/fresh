typedef QMap<int, QVariant> pFileDialogResult;

class pFileDialog : QFileDialog
{
%TypeHeaderCode
#include <gui/pFileDialog.h>
%End

public:
	enum Property {
		TextCodec = 0,
		OpenReadOnly,
		Directory,
		FileName,
		FileNames,
		SelectedFilter
	};
	
	pFileDialog( QWidget* parent = 0, const QString& caption = QString::null, const QString& directory = QString::null, const QString& filter = QString::null, bool textCodecEnabled = true, bool openReadOnlyEnabled = false );
	
	QString textCodec() const;
	void setTextCodec( const QString& codec );
	
	bool textCodecEnabled() const;
	void setTextCodecEnabled( bool enabled );
	
	bool openReadOnly() const;
	void setOpenReadOnly( bool readOnly );
	
	bool openReadOnlyEnabled() const;
	void setOpenReadOnlyEnabled( bool enabled );
	
	static pFileDialogResult getExistingDirectory( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, QFileDialog::Options options = QFileDialog::ShowDirsOnly );
	static pFileDialogResult getOpenFileName( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
	static pFileDialogResult getOpenFileNames( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, bool enabledOpenReadOnly = true, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
	static pFileDialogResult getSaveFileName( QWidget* parent = 0, const QString& caption = QString::null, const QString& dir = QString::null, const QString& filter = QString::null, bool enabledTextCodec = true, const QString& selectedFilter = QString::null, QFileDialog::Options options = 0 );
protected:
	static void setDialog( pFileDialog* dlg, const QString& caption, const QString& dir, const QString& filter, bool enabledTextCodec, bool enabledOpenReadOnly, const QString& selectedFilter, QFileDialog::FileMode mode, QFileDialog::Options options );
};
