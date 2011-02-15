#ifndef PTRANSLATIONMANAGER_H
#define PTRANSLATIONMANAGER_H

/*!
    \file pTranslationManager.h
    \brief Application translations maanager.
    \author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

// for qHash( const QLocale& locale )
#include "pCoreUtils.h" 
using namespace pCoreUtils;

#include <QObject>
#include <QStringList>
#include <QSet>
#include <QHash>
#include <QLocale>
#include <QFileInfo>

class QTranslator;

/*!< A QList of QTranslator */ 
typedef QList<QTranslator*> QTranslatorList;

/*!
	\ingroup Core
    \class pTranslationManager
    \brief Application translations maanager.

    With this class you can easily set the patterns of your translations and possible paths to locate them.
	The manager can then reload the translations and list availables locales availables for your translations.
	Changing your application language was never so easy !
*/
class FRESH_EXPORT pTranslationManager : public QObject
{
	Q_OBJECT
	
public:
	/*!
		Returns the global instance of the pTranslationManager.
	*/
	static pTranslationManager* instance();
	/*!
		Construct a pTranslationManager having parent \a parent.
	*/
	pTranslationManager( QObject* parent = 0 );
	/*!
		Return the translations patterns defined.
		\sa forbiddenTranslationsMasks().
	*/
	QStringList translationsMasks() const;
	/*!
		Return the forbidden translations patterns defined.
		\sa translationsMasks().
	*/
	QStringList forbiddenTranslationsMasks() const;
	/*!
		Return the current locale of your application (that mean the current language too).
	*/
	QLocale currentLocale() const;
	/*!
		Return the system locale.
	*/
	QLocale systemLocale() const;
	/*!
		Return all the found locales (languages) available for your application.
	*/
	QStringList availableLocales() const;
	/*!
		Return all the found QLocale (languages) available for your application.
	*/
	QList<QLocale> availableQLocales() const;
	/*!
		Return the paths to look for translations files.
	*/
	QStringList translationsPaths() const;
	/*!
		Return the paths to look for system translations files.
	*/
	QStringList systemTranslationsPaths() const;
	/*!
		Return true if pTranslationManager should handle C locale as UnitedStates locale, else false.
	*/
	bool isFakeCLocaleEnabled() const;

public slots:
	/*!
		Looks for translations files to (re)load.
	*/
	void reloadTranslations();
	/*!
		Set the translations patterns to \a masks.
		\note The masks can be filters (ie: toto*.qm).
	*/
	void setTranslationsMasks( const QStringList& masks );
	/*!
		Add the translation pattern \a mask.
	*/
	void addTranslationsMask( const QString& mask );
	/*!
		Remove the pattern \a mask from the patterns list.
	*/
	void removeTranslationsMask( const QString& mask );
	/*!
		Set the forbidden translations patterns to \a masks.
		\note The masks can be filters (ie: toto*.qm).
	*/
	void setForbiddenTranslationsMasks( const QStringList& masks );
	/*!
		Add the forbidden translation pattern \a mask.
	*/
	void addForbiddenTranslationsMask( const QString& mask );
	/*!
		Remove the forbidden pattern \a mask from the forbidden patterns list.
	*/
	void removeForbiddenTranslationsMask( const QString& mask );
	/*!
		Set the current QLocale (language) of the application to \a locale.
	*/
	void setCurrentLocale( const QLocale& locale );
	/*!
		Set the paths to be check when looking for translations to \a paths.
	*/
	void setTranslationsPaths( const QStringList& paths );
	/*!
		Set the system paths to be check when looking for translations to \a paths.
	*/
	void setSystemTranslationsPaths( const QStringList& paths );
	/*!
		Set true if C locale should be handled as UnitedStates locale, else false.
	*/
	void setFakeCLocaleEnabled( bool enabled );
	
protected:
	QLocale mCurrentLocale;
	QSet<QString> mTranslationsMasks;
	QSet<QString> mForbiddenTranslationsMasks;
	QStringList mTranslationsPaths;
	QStringList mSystemTranslationsPaths;
	bool mFakeCLocaleEnabled;
	QSet<QString> mAvailableLocales;
	QHash<QLocale, QTranslatorList> mTranslators;
	
	bool addTranslator( const QString& filePath, const QLocale& locale );
	void clearTranslators();
};

#endif // PTRANSLATIONMANAGER_H
