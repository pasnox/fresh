#ifndef PENVIRONMENTVARIABLESMANAGER_H
#define PENVIRONMENTVARIABLESMANAGER_H

/*!
	\file pEnvironmentVariablesManager.h
	\brief Helper manager for pEnvironmentVariablesModel.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "core/FreshExport.h"
#include "pEnvironmentVariablesModel.h"

/*!
	\ingroup Gui
	\class pEnvironmentVariablesManager
	\brief Helper manager for pEnvironmentVariablesModel.

	This helper class allow to read, write, update and merge variables to/from files.
*/
class FRESH_EXPORT pEnvironmentVariablesManager
{
public:
	/*!
		Create a manager for handling pEnvironmentVariablesModel::Variables.
	*/
	pEnvironmentVariablesManager();
	/*!
		Load the variables states from ini file.
	*/
	bool load();
	/*!
		Save the variables states to ini file.
	*/
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
