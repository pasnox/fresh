/****************************************************************************
**
** 		Created using Monkey Studio IDE v1.8.4.0 (1.8.4.0)
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Fresh Library
** FileName  : pEnvironmentVariablesModel.h
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
#ifndef PENVIRONMENTVARIABLESMODEL_H
#define PENVIRONMENTVARIABLESMODEL_H

/*!
	\file pEnvironmentVariablesModel.h
	\brief A model to handle your process environment variables.
	\author Filipe Azevedo aka Nox P\@sNox <pasnox@gmail.com>
*/

#include "FreshExport.h"

#include <QAbstractItemModel>

/*!
	\ingroup FreshGui
	\class pEnvironmentVariablesModel
	\brief A model to handle your process environment variables.
*/
class FRESH_EXPORT pEnvironmentVariablesModel : public QAbstractItemModel
{
	Q_OBJECT

public:
	/*!
		\ingroup FreshGui
		\struct pEnvironmentVariablesModel::Variable
		\brief This structure represent the state of a variable.
	*/
	struct Variable {
		Variable( const QString& _name = QString::null, const QString& _value = QString::null, bool _enabled = false );

		/*!
			The name.
		*/
		QString name;
		/*!
			The value.
		*/
		QString value;
		/*!
			The enabled state.
			
			A disabled variable should by example not being kept when updating your QProcess environement variables.
			\sa QProcess::setEnvironment
		*/
		bool enabled;
	};

	/*!
		A QHash of pEnvironmentVariablesModel::Variable.
	*/
	typedef QHash<QString, pEnvironmentVariablesModel::Variable> Variables;
	/*!
		A QList of pEnvironmentVariablesModel::Variable pointers.
	*/
	typedef QList<pEnvironmentVariablesModel::Variable*> VariableList;

	/*!
		Create a variable model having \a parent as parent.
	*/
	pEnvironmentVariablesModel( QObject* parent = 0 );

	/*!
		Reimplemented.
	*/
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented.
	*/
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	/*!
		Reimplemented.
	*/
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented.
	*/
	virtual QModelIndex parent( const QModelIndex& index ) const;
	/*!
		Reimplemented.
	*/
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented.
	*/
	virtual QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	/*!
		Reimplemented.
	*/
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	/*!
		Reimplemented.
	*/
	virtual Qt::ItemFlags flags( const QModelIndex& index ) const;
	/*!
		Reimplemented.
	*/
	virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole );
	
	/*!
		Return a QModelIndex for variable \a name at specified \a column.
	*/
	QModelIndex index( const QString& name, int column = 0 ) const;
	/*!
		Return the variable for the given \a index.
	*/
	pEnvironmentVariablesModel::Variable variable( const QModelIndex& index ) const;

	/*!
		Return the variables.
	*/
	pEnvironmentVariablesModel::Variables variables() const;
	/*!
		Return the default variables.
	*/
	pEnvironmentVariablesModel::Variables defaultVariables() const;
	/*!
		Return the variables as a list of pairs of key=value and keeping the disabled one according to \a keepDisabled.
	*/
	QStringList variables( bool keepDisabled ) const;
	/*!
		Return the variable named \a name.
	*/
	pEnvironmentVariablesModel::Variable variable( const QString& name ) const;
	/*!
		Return true if the model contains a variable named \a name else false.
	*/
	bool contains( const QString& name ) const;
	/*!
		Return true if the model is empty else false.
	*/
	bool isEmpty() const;
	
	/*!
		Helper that convert a QStringList to a pEnvironmentVariablesModel::Variables hash.
		\sa QProcessEnvironment::toStringList()
	*/
	static pEnvironmentVariablesModel::Variables stringListToVariables( const QStringList& variables );
	/*!
		Helper that convert a pEnvironmentVariablesModel::Variables hash to a QStringList.
		\sa QProcessEnvironment::toStringList()
	*/
	static QStringList variablesToStringList( const pEnvironmentVariablesModel::Variables& variables, bool keepDisabled );

public slots:
	/*!
		Set the model variables to \a variables making them the default according to \a setDefault.
		\sa setDefaultVariables
	*/
	void setVariables( const pEnvironmentVariablesModel::Variables& variables, bool setDefault );
	/*!
		Set the model default variables to \a variables.
	*/
	void setDefaultVariables( const pEnvironmentVariablesModel::Variables& variables );
	/*!
		Set the model variables to \a variables making them the default according to \a setDefault.
		\sa setDefaultVariables
	*/
	void setVariables( const QStringList& variables, bool setDefault );
	/*!
		Set the model default variables to \a variables.
	*/
	void setDefaultVariables( const QStringList& variables );
	/*!
		Set or update the variable named \a name with the state of \a variable.
	*/
	void setVariable( const QString& name, const pEnvironmentVariablesModel::Variable& variable );
	/*!
		Remove the variable named \a name.
	*/
	void removeVariable( const QString& name );
	/*!
		Clear the model variables.
	*/
	void clearVariables();
	/*!
		Clear the model and set the variables to the default ones.
	*/
	void resetVariablesToDefault();
	/*!
		Clear the model and set the variables to the system ones making them default according to \a setDefault.
	*/
	void resetVariablesToSystem( bool setDefault );

protected:
	int mRowCount;
	pEnvironmentVariablesModel::Variables mDefaultVariables;
	pEnvironmentVariablesModel::Variables mVariables;
	pEnvironmentVariablesModel::VariableList mOrder;

signals:
	/*!
		It's emitted when the default variables have changed.
	*/
	void defaultVariablesChanged();
};

#endif // PENVIRONMENTVARIABLESMODEL_H
