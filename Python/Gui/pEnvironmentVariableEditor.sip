class pEnvironmentVariableEditor : QDialog 
{
%TypeHeaderCode
#include <gui/environmentvariablemanager/pEnvironmentVariableEditor.h>
%End
public:
	pEnvironmentVariableEditor( QWidget* parent = 0, const pEnvironmentVariablesModel::Variable& variable = pEnvironmentVariablesModel::Variable() );
	virtual ~pEnvironmentVariableEditor();
	
	pEnvironmentVariablesModel::Variable variable() const;

};
