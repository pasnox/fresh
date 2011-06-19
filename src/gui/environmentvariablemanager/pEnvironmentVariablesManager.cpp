/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariablesManager.cpp
** Date      : 2011-02-20T00:41:09
** License   : LGPL v3
** Home Page : https://github.com/pasnox/fresh
** Comment   : Fresh Library is a Qt 4 extension library providing set of new core & gui classes.
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU Leser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This package is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this program. If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "pEnvironmentVariablesManager.h"
#include "pSettings.h"

#include <QProcessEnvironment>

const QString pEnvironmentVariablesManager::mSettingsKey( "EnvironmentVariables" );

pEnvironmentVariablesManager::pEnvironmentVariablesManager()
{
}

pEnvironmentVariablesManager::~pEnvironmentVariablesManager()
{
}

bool pEnvironmentVariablesManager::load()
{
	return readVariables( mVariables );
}

bool pEnvironmentVariablesManager::save()
{
	return writeVariables( mVariables );
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::variables() const
{
	pEnvironmentVariablesModel::Variables variables = mVariables;
	
	if ( variables.isEmpty() ) {
		variables = pEnvironmentVariablesModel::stringListToVariables( QProcessEnvironment::systemEnvironment().toStringList() );
	}
	else {
		mergeNewVariables( variables );
	}
	
	return variables;
}

void pEnvironmentVariablesManager::setVariables( const pEnvironmentVariablesModel::Variables& variables )
{
	mVariables = removeUnmodifiedVariables( variables );
}

bool pEnvironmentVariablesManager::mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
	const pEnvironmentVariablesModel::Variables newVariables = pEnvironmentVariablesModel::stringListToVariables( QProcessEnvironment::systemEnvironment().toStringList() );
	bool modified = false;
	
	foreach ( const QString& name, newVariables.keys() ) {
		if ( variables.contains( name ) ) {
			continue;
		}
		
		modified = true;
		variables[ name ] = newVariables[ name ];
	}
	
	return modified;
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const
{
	pEnvironmentVariablesModel::Variables items = variables;
	mergeNewVariables( items );
	return items;
}

bool pEnvironmentVariablesManager::removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
	const pEnvironmentVariablesModel::Variables sysVariables = pEnvironmentVariablesModel::stringListToVariables( QProcessEnvironment::systemEnvironment().toStringList() );
	bool modified = false;
	
	foreach ( const pEnvironmentVariablesModel::Variable& variable, variables ) {
		if ( !variable.enabled ) {
			continue;
		}
		
		if ( sysVariables.contains( variable.name ) && variable.value == sysVariables[ variable.name ].value ) {
			variables.remove( variable.name );
			modified = true;
		}
	}
	
	return modified;
}

pEnvironmentVariablesModel::Variables pEnvironmentVariablesManager::removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const
{
	pEnvironmentVariablesModel::Variables items = variables;
	removeUnmodifiedVariables( items );
	return items;
}

QStringList pEnvironmentVariablesManager::variables( bool keepDisabled ) const
{
	return pEnvironmentVariablesModel::variablesToStringList( variables(), keepDisabled );
}

bool pEnvironmentVariablesManager::writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const
{
	pSettings settings( 0, mSettingsKey, "1.0.0" );
	
	settings.remove( mSettingsKey );
	
	settings.beginWriteArray( mSettingsKey );
	
	for ( int i = 0; i < variables.count(); i++ ) {
		settings.setArrayIndex( i );
		const pEnvironmentVariablesModel::Variable& variable = (variables.begin() +i).value();
		
		settings.setValue( "Name", variable.name );
		settings.setValue( "Value", variable.value );
		settings.setValue( "Enabled", variable.enabled );
	}
	
	settings.endArray();
	
	return true;
}

bool pEnvironmentVariablesManager::readVariables( pEnvironmentVariablesModel::Variables& variables ) const
{
	pSettings settings( 0, mSettingsKey, "1.0.0" );
	
	const int count = settings.beginReadArray( mSettingsKey );
	
	for ( int i = 0; i < count; i++ ) {
		settings.setArrayIndex( i );
		
		variables[ settings.value( "Name" ).toString() ] =
			pEnvironmentVariablesModel::Variable( settings.value( "Name" ).toString(),
			settings.value( "Value" ).toString(), settings.value( "Enabled" ).toBool() );
	}
	
	settings.endArray();
	
	return true;
}
