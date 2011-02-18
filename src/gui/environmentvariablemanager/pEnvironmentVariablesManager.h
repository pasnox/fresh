#ifndef PENVIRONMENTVARIABLESMANAGER_H
#define PENVIRONMENTVARIABLESMANAGER_H

/*!
	\file pEnvironmentVariablesManager.h
	\brief Helper manager for pEnvironmentVariablesModel.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"
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
	/*!
		Return the variables of this manager.
	*/
	pEnvironmentVariablesModel::Variables variables() const;
	/*!
		Set the variables of this manager.
	*/
	void setVariables( const pEnvironmentVariablesModel::Variables& variables );
	/*!
		Check for new variables defined in QProcessEnvironment::systemEnvironment() and merge them in \a variables.
		
		Returns true if changes occurs else false.
	*/
	bool mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	/*!
		Check for new variables defined in QProcessEnvironment::systemEnvironment() and merge them with \a variables.
		
		Returns the merged version of \a variables.
	*/
	pEnvironmentVariablesModel::Variables mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	/*!
		Remove from \a variables all unmodified variables.
		
		Unmododified variables are the ones that are enabled and content is same as defined in QProcessEnvironment::systemEnvironment().
		
		Returns true if changes occurs else false.
	*/
	bool removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	/*!
		Remove from \a variables all unmodified variables.
		
		Unmododified variables are the ones that are enabled and content is same as defined in QProcessEnvironment::systemEnvironment().
		
		Returns the merged version of \a variables.
	*/
	pEnvironmentVariablesModel::Variables removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	/*!
		Return a list of pairs of key=value variables.
		
		The disabled one are kept according to \a keepDisabled.
	*/
	QStringList variables( bool keepDisabled ) const;

protected:
	static const QString mSettingsKey;
	bool mInitialized;
	mutable pEnvironmentVariablesModel::Variables mVariables;
	
	/*!
		This is the low level function that write \a variables to an ini file.
		
		The defauld implementation use pSettings for that.
	*/
	virtual bool writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	/*!
		This is the low level function that read \a variables from an ini file.
		
		The defauld implementation use pSettings for that.
	*/
	virtual bool readVariables( pEnvironmentVariablesModel::Variables& variables ) const;
};

#endif // PENVIRONMENTVARIABLESMANAGER_H
