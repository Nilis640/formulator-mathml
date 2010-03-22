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
#include "../ast/astdsr.h"
#include "../vm/vmeval.h"
#include "../symbol/symbol_table_dsr.h"

///////////////////////////////////////////////////////////////////////////////

//int CMMLCalc::CreateVM( enum CDsrMathModel taskID, long subtaskID )
//{
//	int iRes = CMMLError_NONE;
//	if( !m_dsrSymbolTable || !m_ast )
//		iRes = CMMLError_NoAST;
//	else
//	{
//		// Create Virtual PC
//		m_dsrSymbolTable->setMessanger( &m_dmm, m_dmmPage, QString( m_taskName ) );
//		m_dsrSymbolTable->setTaskID( taskID, subtaskID );
//		m_dsrSymbolTable->allocateVariables();
//		m_vm = new CDSR_VMEval( m_dsrSymbolTable );
//	    
//		// Calculate types
//		m_ast->setObjectContextThroughTree();
//		m_ast->placeDataType( *m_dsrSymbolTable );
//	    
//		if( !(m_ast->getDataType() && m_ast->getDataType()->getType() != DSRDATA_TYPE_ERROR) )
//			iRes = CMMLError_Semantic;
//
//		m_ast->setLevelThroughTree();
//		m_vm->getInstruction( m_ast );
//		if( iRes != CMMLError_Semantic && m_vm->isError() )
//			iRes = CMMLError_Semantic;
//	}
//
//	m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName) );
//	return iRes;
//}

int CMMLCalc::CreateVM( enum CDsrMathModel taskID, long subtaskID )
{
	int iRes = CMMLError_NONE;
	if( !m_dsrSymbolTable || !m_ast )
		iRes = CMMLError_NoAST;
	else
	{
		// Create Virtual PC
		m_dsrSymbolTable->setMessanger( &m_dmm, m_dmmPage, QString( m_taskName ) );
		m_dsrSymbolTable->setTaskID( taskID, subtaskID );
		m_dsrSymbolTable->allocateVariables();
		m_vm = new CDSR_VMEval( m_dsrSymbolTable );
	    
		// Calculate types
		m_ast->setObjectContextThroughTree();
		m_ast->placeDataType( *m_dsrSymbolTable, true );
	    
		if( m_ast->getDataType() && m_ast->getDataType()->getType() != DSRDATA_TYPE_ERROR )
		{
			m_ast->setLevelThroughTree();
			m_vm->getInstruction( m_ast );
			if( m_vm->isError() )
				iRes = CMMLError_Semantic;
		}
		else
			iRes = CMMLError_Semantic;
	}

	m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName) );
	return iRes;
}

int CMMLCalc::execMain()
{
	if( !m_vm )
	{
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_NoVM), QString(m_taskName) );
		return CMMLError_NoVM;
	}

	return m_vm->execCommand();
}

///////////////////////////////////////////////////////////////////////////////
