/****************************************************************************
**
** Copyright (C) 2010 Andriy Kovalchuk, Vyacheslav Levytskyy,
** Igor Samolyuk, Valentyn Yanchuk (aka "Hermitech Laboratory")
**
** All rights reserved.
** Contact: Hermitech Laboratory (info@mmlsoft.com)
**
** This file is a part of the Formulator MathML Editor project
** (http://www.mmlsoft.com).
**
** Commercial Usage
** Licensees holding valid Formulator Commercial licenses may use this 
** file in accordance with the Formulator Commercial License Agreement 
** provided with the Software or, alternatively, in accordance with the 
** terms contained in a written agreement between you and 
** Hermitech Laboratory.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** Hermitech Laboratory at info@mmlsoft.com.
**
****************************************************************************/

#include "../operator/operator.h"
#include "vm_eval_cmd.h"
#include "vmeval.h"

///////////////////////////////////////////////////////////////////////////////

QString CDSR_VMEval_CommandName[ FUNCTION_STD_LAST ] =
{
	_T("nop"),
	_T("skip if false"),
	_T("skip"),
	_T("push variable address"),
	_T("push variable value"),
	_T("push natural"),
	_T("push integer"),
	_T("push real"),
	_T("push QString"),
	_T("op"),
	_T("return"),
	_T("assign function result"),
	_T("dec run stack ptr")
};

QString vm_Cmd2String( long id )
{
	if( id >= 0 && id < FUNCTION_STD_LAST )
		return CDSR_VMEval_CommandName[ id ];
	return _T("unknown command");
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Cmd& c )
{
	return	QString( "line %1 : " ).arg( c.line ) + vm_Cmd2String( c.cmd ) + 
			QString( _T(", ") ) + c.operand.getTextView( smbtable ) + QString( _T("\n") );
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Line& c )
{
	QString ret = _T("");
	for( long i = 0; i < (long) c.ArrayDim(); i++ )
		ret += getTextView( smbtable, c[ i ] );
	return ret;
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Equation& c )
{
	QString ret = _T("");
	ret += QString( _T("== equation left part :\n") ) + getTextView( smbtable, c.left ) + QString( _T("\n") );
	ret += QString( _T("== equation right part :\n") ) + getTextView( smbtable, c.right ) + QString( _T("\n") );
	return ret;
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_SE& c )
{
	QString ret = _T("==== equation system attr : ");
	if( c.is_koshi ) ret += _T("Koshi form");
	if( c.is_linear ) ret += _T(", linear");
	if( c.is_algebraic ) ret += _T(", algebraical");
	if( c.is_differential ) ret += _T(", differential");
	ret += _T("\n");
	if( c.data_type )
		ret += QString( _T("== equation system type : ") ) + c.data_type->getTypeName() + QString( _T("\n") );
	long i;
	ret += _T("== equation system unknown variables : ( ");
	CParseDsrSymbol *smb;
	for( i = 0; i < (long) c.used_xvar.ArrayDim(); i++ )
	{
		smb = smbtable.getSymbol( c.used_xvar[ i ] );
		if( smb ) ret += smb->name + _T(" ");
	}
	ret += _T(")\n");
	ret += QString( _T("== equation system items :\n") );
	for( i = 0; i < (long) c.ArrayDim(); i++ )
		ret += getTextView( smbtable, c[ i ] );
	return ret;
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Function& c )
{
	QString ret = _T("== function ") + c.name;
	ret += QString("(%1 -> %2):\n").arg(c.n_arg).arg(c.n_res);
	ret += getTextView( smbtable, c.body );
	return ret;
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_FT& c )
{
	QString ret = _T("==== function table:\n");
	for( long i = 1; i < (long) c.ArrayDim(); i++ )
		ret += getTextView( smbtable, c[ i ] );
	return ret;
}

QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval& c )
{
	if( c.isError() )
		return QString( _T("error(s) are found while generating a code for virtual machine\n") );
	QString ret = _T("======== a code for virtual machine:\n");
	ret += QString( _T("==== variables:") ) + QString("%1").arg( c.m_var_table.ArrayDim() - 1 ) + QString( _T("\n") );
	ret += _T("==== main code:\n") + getTextView( smbtable, c.m_cmd_table );
	ret += getTextView( smbtable, c.m_fun_table );
	ret += getTextView( smbtable, c.m_equ_table );
	ret += _T("======== end of a code for virtual machine:\n");
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

void CDSR_VMEval::printOneVariableValue2Text( QString& dest, long var_no, int show_reserved )
{
	UniWord value;
	CParseDsrSymbol *smb;
	dest = _T("");
	smb = getSmbTable().getSymbol( getVariable_getSmbID( var_no ) );
	if( !smb || 
		(smb->param3 && !show_reserved) ||
		getVariable_getValue( var_no, value ) == -1 ) return;
	dest += smb->name + QString( _T(" = ") ) + value.getTextView( getSmbTable() ) + QString( _T("\n") );
}

void CDSR_VMEval::printVariableValue2Text( QString& dest, long var_no, int show_reserved )
{
	if( var_no != -1 )
		printOneVariableValue2Text( dest, var_no, show_reserved );
	else
	{
		QString tmp;
		for( long i = getVariable_Begin(); i < getVariable_End(); i++ )
		{
			printOneVariableValue2Text( tmp, i, show_reserved );
			dest += tmp;
		}
	}
}

void CDSR_VMEval::printOneVariableValue2Formulator( QString& dest, long var_no, int show_reserved )
{
	UniWord value;
	CParseDsrSymbol *smb;
	dest = _T("");
	smb = getSmbTable().getSymbol( getVariable_getSmbID( var_no ) );
	if( !smb || 
		(smb->param3 && !show_reserved) ||
		getVariable_getValue( var_no, value ) == -1 ) return;
	dest += smb->name + QString( _T(" = ") ) + value.getFormulatorView( getSmbTable() ) + QString( _T("\n") );
}

void CDSR_VMEval::printVariableValue2Formulator( QString& dest, long var_no, int show_reserved )
{
	if( var_no != -1 )
		printOneVariableValue2Formulator( dest, var_no, show_reserved );
	else
	{
		QString tmp;
		for( long i = getVariable_Begin(); i < getVariable_End(); i++ )
		{
			printOneVariableValue2Formulator( tmp, i, show_reserved );
			dest += tmp;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
