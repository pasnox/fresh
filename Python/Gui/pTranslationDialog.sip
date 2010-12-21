namespace Ui {
	class pTranslationDialog;
};

class pTranslationDialog : QDialog
{
%TypeHeaderCode
#include <gui/pTranslationDialog.h>
%End

public:
	pTranslationDialog( pTranslationManager* translationManager, QWidget* parent = 0 );
	virtual ~pTranslationDialog();
	
	virtual bool event( QEvent* event );
	
	QString selectedLocale() const;
	
	static QString getLocale( pTranslationManager* translationManager, QWidget* parent = 0 );

protected:
	QTreeWidgetItem* newItem( const QLocale& locale );
	QTreeWidgetItem* rootItem( const QLocale& locale );

protected slots:
	void localeChanged();
	void on_tbLocate_clicked();
	void on_tbReload_clicked();
	void on_twLocales_itemSelectionChanged();
	void reject();
	void accept();
};
