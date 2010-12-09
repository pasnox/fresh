#ifndef PTRANSLATIONDIALOG_H
#define PTRANSLATIONDIALOG_H

#include "core/FreshExport.h"

#include <QDialog>
#include <QHash>

class pTranslationManager;
class QTreeWidgetItem;

namespace Ui {
	class pTranslationDialog;
};

class FRESH_EXPORT pTranslationDialog : public QDialog
{
	Q_OBJECT

public:
	pTranslationDialog( pTranslationManager* translationManager, QWidget* parent = 0 );
	virtual ~pTranslationDialog();
	
	virtual bool event( QEvent* event );
	
	QString selectedLocale() const;
	
	static QString getLocale( pTranslationManager* translationManager, QWidget* parent = 0 );

protected:
	Ui::pTranslationDialog* ui;
	pTranslationManager* mTranslationManager;
	QString mOriginalLocale;
	QHash<QString, QTreeWidgetItem*> mRootItems;
	
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

#endif // PTRANSLATIONDIALOG_H
