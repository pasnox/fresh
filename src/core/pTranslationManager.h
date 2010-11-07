#ifndef PTRANSLATIONMANAGER_H
#define PTRANSLATIONMANAGER_H

#include "FreshExport.h"

#include <QObject>
#include <QStringList>
#include <QSet>
#include <QHash>
#include <QLocale>
#include <QFileInfo>

class QTranslator;

typedef QList<QTranslator*> QTranslatorList;

class FRESH_EXPORT pTranslationManager : public QObject
{
	Q_OBJECT
	
public:
	pTranslationManager( QObject* parent = 0 );
	
	QStringList translationsMasks() const;
	QStringList forbiddenTranslationsMasks() const;
	QLocale currentLocale() const;
	QLocale systemLocale() const;
	QStringList availableLocales() const;
	QList<QLocale> availableQLocales() const;
	QStringList translationsPaths() const;
	QStringList systemTranslationsPaths() const;
	bool isFakeCLocaleEnabled() const;

public slots:
	void reloadTranslations();
	void setTranslationsMasks( const QStringList& masks );
	void addTranslationsMask( const QString& mask );
	void removeTranslationsMask( const QString& mask );
	void setForbiddenTranslationsMasks( const QStringList& masks );
	void addForbiddenTranslationsMask( const QString& mask );
	void removeForbiddenTranslationsMask( const QString& mask );
	void setCurrentLocale( const QLocale& locale );
	void setTranslationsPaths( const QStringList& paths );
	void setSystemTranslationsPaths( const QStringList& paths );
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

uint qHash( const QLocale& locale );

#endif // PTRANSLATIONMANAGER_H
