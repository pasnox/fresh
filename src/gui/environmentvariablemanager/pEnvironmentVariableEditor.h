#ifndef PENVIRONMENTVARIABLEEDITOR_H
#define PENVIRONMENTVARIABLEEDITOR_H

#include "core/FreshExport.h"
#include "pEnvironmentVariablesModel.h"

#include <QDialog>

class Ui_pEnvironmentVariableEditor;

class FRESH_EXPORT pEnvironmentVariableEditor : public QDialog 
{
	Q_OBJECT

public:
	pEnvironmentVariableEditor( QWidget* parent = 0, const QString& name = QString::null, const QString& value = QString::null );
	virtual ~pEnvironmentVariableEditor();
	
	pEnvironmentVariablesModel::Variable variable() const;

protected:
	Ui_pEnvironmentVariableEditor* ui;
};

#endif // PENVIRONMENTVARIABLEEDITOR_H
