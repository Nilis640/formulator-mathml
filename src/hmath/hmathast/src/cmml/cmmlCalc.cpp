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

#include "cmmlCalc.h"

#include "HMathAST/iast.h"
#include "HMathAST/ivm.h"
#include "HMathAST/ismbtbl.h"

#include "../ast/astdsr.h"
#include "../vm/vmeval.h"
#include "../symbol/symbol_table_dsr.h"

///////////////////////////////////////////////////////////////////////////////

CMMLCalc::CMMLCalc( DRMessageManager& dmm, long dmmPage, QString taskName ) :
	m_dmm( dmm )
{
	m_dmmPage = dmmPage;
	m_taskName = taskName;
	m_ast = NULL;
	m_dsrSymbolTable = NULL;
	m_localSymbolTable = NULL;
	m_vm = NULL;
}

CMMLCalc::~CMMLCalc()
{
	dispose();
}

void CMMLCalc::dispose()
{
	if( m_ast )
	{
		delete m_ast;
		m_ast = NULL;
	}
	if( m_dsrSymbolTable )
	{
		delete m_dsrSymbolTable;
		m_dsrSymbolTable = NULL;
	}
	if( m_vm )
	{
		delete m_vm;
		m_vm = NULL;
	}
	m_var2mml.clear();
}

//const QMap<QString,QString>& CMMLCalc::getInternalVars()
//{
//	return m_var2mml;
//}

const QVector<QString>& CMMLCalc::getInternalVars()
{
	return m_var2mml;
}

QString CMMLCalc::getErrorMsg( int errCode )
{
	static QString errorMsg[ CMMLError_LAST ] =
	{
		_T("OK"),

		_T("Abstract syntax tree is empty, there is no data to init Virtual Machine"),
		_T("Virtual Machine is empty"),
		_T("Operation \'%1\' is not supported in the current version"),
		_T("Ignoring \'%1\' node (deprecated construction)"),
		_T("Ignoring \'%1\' node (construction is found outside of its proper container)"),
		_T("Unexpected apply operation \'%1\'"),
		_T("Ignore excess args of \'%1\' node"),
		_T("Unknown function name \'%1\'"),
		_T("Wrong number of arguments in function \'%1\'"),
		_T("Wrong number of arguments in \'%1\' element (%2 is expected)"),
		_T("Unknown symbol \'%1\' (function name or operation is expected)"),
		_T("Unknown symbol \'%1\'"),
		_T("Syntax error in the token element \'%1\'"),
		_T("Unknown constant name in the token element \'%1\'"),
		_T("Syntax error in the token element \'%1\' (unknown type)"),
		_T("Token element \'%1\' is empty"),
		_T("Constructor \'%1\' is empty"),
		_T("Matrix constructor \'%1\' is empty"),
		_T("Syntax error in matrix constructor \'%1\'"),
		_T("Internal error: Record for \'%1\' element is missing in the symbol table"),
		_T("Function body is expected for \'%1\' element"),
		_T("Error(s) while initializing the declared variable by the second child of \'%1\' element"),
		_T("Syntax error in the token element \'%1\' (unknown tag)"),
		_T("Syntax error in the token element \'%1\' (each \'piece\' element must contain exactly two children)"),
		_T("Syntax error in the token element \'%1\' (the second children of the \'piece\' element should be \'apply\' expression)"),
		_T("Semantic error(s)"),

	};
	return errorMsg[ (errCode >= 0 && errCode < CMMLError_LAST) ? errCode : 0 ];
}

///////////////////////////////////////////////////////////////////////////////

CDSR_VMEval* CMMLCalc::getVM()
{
	return m_vm;
}

void CMMLCalc::debugPrint( QString& dest )
{
	m_ast->debugPrint( dest, m_dsrSymbolTable, 0 );
}

void CMMLCalc::manualAddAstNode( CDsrAST_Node *newRec )
{
	if( m_ast && newRec )
	{
		m_ast->Add( newRec );
	}
}

void CMMLCalc::setLocalSymbolTable()
{
	if( m_dsrSymbolTable )
		m_localSymbolTable = m_dsrSymbolTable->getNewLocalSmbTable();
}

void CMMLCalc::setGlobalSymbolTable()
{
	m_localSymbolTable = NULL;
	if( !m_dsrSymbolTable )
		m_dsrSymbolTable = new CParseDsrSymbolTable();
}

CParseDsrSymbolTable* CMMLCalc::getCurrentSymbolTable()
{
	return m_localSymbolTable ? m_localSymbolTable : m_dsrSymbolTable;
}

///////////////////////////////////////////////////////////////////////////////
