#ifndef PTRANSLATIONDIALOG_H
#define PTRANSLATIONDIALOG_H

/*!
	\file pTranslationDialog.h
	\brief A dialog for handling application locales.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QDialog>
#include <QPointer>
#include <QHash>

class pTranslationManager;
class QTreeWidgetItem;

namespace Ui {
	class pTranslationDialog;
};

/*!
	\ingroup Gui
	\class pTranslationDialog
	\brief A dialog for handling application locales.

	This dialog use a pTranslationManager instance to provide to the user the list of available locales/translations.
*/
class FRESH_EXPORT pTranslationDialog : public QDialog
{
	Q_OBJECT

public:
	/*!
		Create an instance of the dialog having \a parent as parent.
	*/
	pTranslationDialog( QWidget* parent = 0 );
	/*!
		Create an instance of the dialog having \a parent as parent and \a translationManager as translation manager.
	*/
	pTranslationDialog( pTranslationManager* translationManager, QWidget* parent = 0 );
	/*!
		Reimplemented.
	*/
	virtual ~pTranslationDialog();
	/*!
		Reimplemented.
	*/
	virtual bool event( QEvent* event );
	/*!
		Return the translation manager this dialog use.
	*/
	pTranslationManager* translationManager() const;
	/*!
		Set the translation manager to use by this dialog.
	*/
	void setTranslationManager( pTranslationManager* translationManager );
	/*!
		Return the selected item locale, if no item is selected it return the translation manager current locale.
	*/
	QString selectedLocale() const;
	/*!
		Convenient static member that allow to quickly present the user a translation manager dialog.
	*/
	static QString getLocale( pTranslationManager* translationManager, QWidget* parent = 0 );

protected:
	Ui::pTranslationDialog* ui;
	QPointer<pTranslationManager> mTranslationManager;
	QString mOriginalLocale;
	QHash<QString, QTreeWidgetItem*> mRootItems;
	
	void init( pTranslationManager* translationManager );
	QTreeWidgetItem* newItem( const QLocale& locale );
	QTreeWidgetItem* rootItem( const QLocale& locale );

protected slots:
	void localeChanged();
	void on_tbLocate_clicked();
	void on_tbReload_clicked();
	void on_twLocales_itemSelectionChanged();
	/*!
		Reimplemented.
	*/
	void reject();
	/*!
		reimplemented.
	*/
	void accept();
};

#endif // PTRANSLATIONDIALOG_H
