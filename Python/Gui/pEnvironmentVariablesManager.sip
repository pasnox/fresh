class pEnvironmentVariablesManager : QObject
{
%TypeHeaderCode
#include <gui/environmentvariablemanager/pEnvironmentVariablesManager.h>
%End

public:
	pEnvironmentVariablesManager( QObject* parent = 0 );
	
	bool load();
	bool save();
	
	pEnvironmentVariablesModel::Variables variables() const;
	void setVariables( const pEnvironmentVariablesModel::Variables& variables );
	
	bool mergeNewVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	//FIXME pEnvironmentVariablesModel::Variables mergeNewVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	bool removeUnmodifiedVariables( pEnvironmentVariablesModel::Variables& variables ) const;
	//FIXME pEnvironmentVariablesModel::Variables removeUnmodifiedVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	QStringList variables( bool keepDisabled ) const;

protected:
	virtual bool writeVariables( const pEnvironmentVariablesModel::Variables& variables ) const;
	virtual bool readVariables( pEnvironmentVariablesModel::Variables& variables ) const;
};
