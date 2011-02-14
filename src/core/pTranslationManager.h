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
		\brief Returns the global instance of the pTranslationManager class.
		\return A pTranslationManager.
	*/
	static pTranslationManager* instance();
	/*!
		\brief Construct a pTranslationManager having parent \a parent.
		\param parent The parent of this object.
	*/
	pTranslationManager( QObject* parent = 0 );
	/*!
		\brief Return the translations patterns defined.
		\return A QStringList containing the patterns.
		\sa forbiddenTranslationsMasks()
	*/
	QStringList translationsMasks() const;
	/*!
		\brief Return the forbidden translations patterns defined.
		\return A QStringList containing the forbidden patterns.
		\sa translationsMasks()
	*/
	QStringList forbiddenTranslationsMasks() const;
	/*!
		\brief Return the current locale of your application (that mean the current language too).
		\return A QLocale object.
	*/
	QLocale currentLocale() const;
	/*!
		\brief Return the system locale.
		\return A QLocale object.
	*/
	QLocale systemLocale() const;
	/*!
		\brief Return all the found locales (languages) available for your application.
		\return A QStringList of QLocale names.
	*/
	QStringList availableLocales() const;
	/*!
		\brief Return all the found locales (languages) available for your application.
		\return A QList of QLocale.
	*/
	QList<QLocale> availableQLocales() const;
	/*!
		\brief Return the paths to look for translations files.
		\return A QStringList of paths.
	*/
	QStringList translationsPaths() const;
	/*!
		\brief Return the paths to look for system translations files.
		\return A QStringList of system paths.
	*/
	QStringList systemTranslationsPaths() const;
	/*!
		\brief Return if pTranslationManager should handle C locale as UnitedStates locale.
		\return A bool telling if C locale is handled as UnitedStates locale.
	*/
	bool isFakeCLocaleEnabled() const;

public slots:
	/*!
		\brief Looks for translations files.
	*/
	void reloadTranslations();
	/*!
		\brief Set the translations patterns.
		\param masks The patterns used as wildcard when looking for translations files.
	*/
	void setTranslationsMasks( const QStringList& masks );
	/*!
		\brief Add the translation pattern \a mask.
		\param mask The pattern to add to the patterns list.
	*/
	void addTranslationsMask( const QString& mask );
	/*!
		\brief Remove the pattern \a mask from the patterns list.
		\param mask The pattern to remove.
	*/
	void removeTranslationsMask( const QString& mask );
	/*!
		\brief Set the forbidden translations patterns.
		\param masks The forbidden patterns used as wildcard when looking for translations files.
	*/
	void setForbiddenTranslationsMasks( const QStringList& masks );
	/*!
		\brief Add the forbidden translation pattern \a mask.
		\param mask The forbidden pattern to add to the forbidden patterns list.
	*/
	void addForbiddenTranslationsMask( const QString& mask );
	/*!
		\brief Remove the forbidden pattern \a mask from the forbidden patterns list.
		\param mask The forbidden pattern to remove.
	*/
	void removeForbiddenTranslationsMask( const QString& mask );
	/*!
		\brief Set the current locale (language) of the application to \a locale.
		\param locale The locale to be used.
	*/
	void setCurrentLocale( const QLocale& locale );
	/*!
		\brief Set the paths to be check when looking for translations.
		\param paths The paths to be looked.
	*/
	void setTranslationsPaths( const QStringList& paths );
	/*!
		\brief Set the system paths to be check when looking for translations.
		\param paths The system paths to be looked.
	*/
	void setSystemTranslationsPaths( const QStringList& paths );
	/*!
		\brief Set if C locale should be handled as UnitedStates locale.
		\param enabled True if C emulate UnitedStates locale, else false.
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
	
	/*!
		\brief Internally add a QTranslator in the available translators.
		\param filePath The file path of the translator to add.
		\param locale The locale of the translator.
		\return A bool setted to true if adding was ok else false.
		\sa QCoreApplication::installTranslator()
	*/
	bool addTranslator( const QString& filePath, const QLocale& locale );
	/*!
		\brief Delete and clear the internal hash of QTranslator.
	*/
	void clearTranslators();
};

#endif // PTRANSLATIONMANAGER_H
