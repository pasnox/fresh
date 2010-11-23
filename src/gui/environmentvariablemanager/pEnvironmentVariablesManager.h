#ifndef PENVIRONMENTVARIABLESMANAGER_H
#define PENVIRONMENTVARIABLESMANAGER_H

#include "core/FreshExport.h"
#include "pEnvironmentVariablesModel.h"

class FRESH_EXPORT pEnvironmentVariablesManager : public QObject
{
	Q_OBJECT
	
public:
	pEnvironmentVariablesManager( QObject* parent = 0 );
	
	bool load();
	bool save();
	
	pEnvironmentVariablesModel::Variables variables() const;
	void setVariables( const pEnvironmentVariablesModel::Variables& variables );
	
	bool mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	pEnvironmentVariablesModel::Variables mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	bool removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	pEnvironmentVariablesModel::Variables removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	QStringList variables( bool keepDisabled ) const;

protected:
	static const QString mSettingsKey;
	bool mInitialized;
	mutable pEnvironmentVariablesModel::Variables mVariables;
	
	virtual bool writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	virtual bool readVariables( pEnvironmentVariablesModel::Variables& variables ) const;
};

#endif // PENVIRONMENTVARIABLESMANAGER_H
