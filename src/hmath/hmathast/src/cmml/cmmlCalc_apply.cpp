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

CDsrAST_Node* CMMLCalc::Create_apply( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection )
{
	if( !m_dsrSymbolTable ) return NULL;
	if( !applyNode || !applyNode->getTags() || nodeCollection.size() < 2 )
	{
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_ArgNumberMismatch), QString(m_taskName), applyNode->getLine(), applyNode->getTagName() );
		return NULL;
	}

	CDsrAST_Node *pRes = NULL;
	int iRes = CMMLError_NONE;

	QString opName = nodeCollection[ 0 ]->getTagName();
	int opID = mapMMLTag2DsrOp( opName );

	if( opID >= FUNCTION_STD_NONE && opID < FUNCTION_STD_LAST )
		pRes = Create_apply_DsrOp( applyNode, nodeCollection, opName, opID, nodeCollection[ 0 ]->getLine(), iRes );
	else
		pRes = Create_apply_GeneralCall( applyNode, nodeCollection, opName, opID, nodeCollection[ 0 ]->getLine(), iRes );

	if( iRes != CMMLError_NONE )
		m_dmm.AddLine( m_dmmPage, getErrorMsg(iRes), QString(m_taskName), applyNode->getLine(), opName );
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////

CDsrAST_Node* CMMLCalc::Create_apply_DsrOp( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection,
										    QString opName, int opID, long line, int& iRes )
{
	CDsrAST_Node *pRes = NULL;
	switch( opID )
	{
	//case OPERATOR_op_equation_e:
	//	break;

	case OPERATOR_op_assign:
	// TODO: check "assign" out
	case OPERATOR_square_brackets:
	//
	case OPERATOR_op_add:			
	case OPERATOR_op_mul:			
	case OPERATOR_op_bool_and:		
	case OPERATOR_op_bool_or:		
	case OPERATOR_op_compare_e:		
	case OPERATOR_op_rel_g:			
	case OPERATOR_op_rel_l:			
	case OPERATOR_op_rel_ge:			
	case OPERATOR_op_rel_le:			
	case OPERATOR_op_setmul:			
	case OPERATOR_op_setadd:
		{
			CDsrAST_Node *arg1 = (nodeCollection.size() > 1) ? Create( nodeCollection[ 1 ] ) : NULL;
			if( arg1 ) arg1->line = nodeCollection[ 1 ]->getLine();
			if( nodeCollection.size() <= 2 )
			{
				pRes = new CDsrAST_Expr( new CDsrAST_Op( opID, line ), arg1, NULL );
				pRes->line = applyNode->getLine();
			}
			else
			{
				CDsrAST_Node *arg2 = NULL;
				for( unsigned long i = 2; i < nodeCollection.size(); i++ )
				{
					if( (arg2 = Create( nodeCollection[ i ] )) != NULL )
						arg2->line = nodeCollection[ i ]->getLine();
					arg1 = new CDsrAST_Expr( new CDsrAST_Op( opID, line ), arg1, arg2 );
					arg1->line = applyNode->getLine();
				}
				pRes = arg1;
			}
		}
		break;

	case OPERATOR_op_sub:			
	case OPERATOR_op_div:			
	case OPERATOR_op_pow:			
	case OPERATOR_op_mod:			
	case OPERATOR_op_bool_not:		
	case OPERATOR_op_compare_ne:		
	case OPERATOR_op_rel_in:			
	case OPERATOR_op_rel_not_in:		
	case OPERATOR_op_rel_set_in_eq:	
	case OPERATOR_op_rel_set_in:		
	case OPERATOR_op_rel_set_not_in:	
		{
			// check arguments number
			if( nodeCollection.size() > 3 )
				iRes = CMMLError_IgnoreExcessArgs;

			CDsrAST_Node *arg1 = (nodeCollection.size() > 1) ? Create( nodeCollection[ 1 ] ) : NULL;
			if( arg1 ) arg1->line = nodeCollection[ 1 ]->getLine();
			CDsrAST_Node *arg2 = (nodeCollection.size() > 2) ? Create( nodeCollection[ 2 ] ) : NULL;
			if( arg2 ) arg2->line = nodeCollection[ 2 ]->getLine();
			pRes = new CDsrAST_Expr( new CDsrAST_Op( opID, line ), arg1, arg2 );
			pRes->line = applyNode->getLine();
		}
		break;

	case FUNCTION_std_exp:			
	case FUNCTION_std_root:			
	case FUNCTION_std_gcd:			
	case FUNCTION_std_abs:			
	case FUNCTION_std_real:			
	case FUNCTION_std_imag:			
	case FUNCTION_std_lcm:			
	case FUNCTION_std_floor:			
	case FUNCTION_std_ceil:			
	case FUNCTION_std_ln:			
	case FUNCTION_std_log:			
	case FUNCTION_std_sin:			
	case FUNCTION_std_cos:			
	case FUNCTION_std_tg:			
	case FUNCTION_std_sec:			
	case FUNCTION_std_cosec:			
	case FUNCTION_std_ctg:			
	case FUNCTION_std_sh:			
	case FUNCTION_std_ch:			
	case FUNCTION_std_th:			
	case FUNCTION_std_sech:			
	case FUNCTION_std_cosech:		
	case FUNCTION_std_cth:			
	case FUNCTION_std_arcsin:		
	case FUNCTION_std_arccos:		
	case FUNCTION_std_arctg:			
	case FUNCTION_std_arch:			
	case FUNCTION_std_arccosec:		
	case FUNCTION_std_arcsec:		
	case FUNCTION_std_arsh:			
	case FUNCTION_std_arcth:			
	case FUNCTION_std_det:			
	case FUNCTION_std_primes:		
		pRes = constructFunctionCall( applyNode, nodeCollection, m_dsrSymbolTable, ::ml_Op2String( opID ), line, iRes );
		break;

	case FORMULA_transpose:
	case FORMULA_integers:
	case FORMULA_reals:				
	case FORMULA_rationals:			
	case FORMULA_naturalnumbers:
	case FORMULA_complexes:			
	case FORMULA_exponentiale:
	case FORMULA_eulergamma:
	case FORMULA_imaginaryi:			
	case FORMULA_set_empty:			
	case FORMULA_pi:
		pRes = constructFunctionCall( applyNode, nodeCollection, m_dsrSymbolTable, ::ml_Op2String( opID ), line, iRes );
		break;

	case CONST_true:
	case CONST_false:
		iRes = CMMLError_NotSupportedCurrently;
		break;

	case FUNCTION_STD_NONE:
		iRes = CMMLError_UnexpectedApplyOp;
		break;

	default:
		pRes = constructFunctionCall( applyNode, nodeCollection, opName, line, iRes );
	}
	return pRes;
}

CDsrAST_Node* CMMLCalc::Create_apply_GeneralCall(   AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection,
													QString opName, int opID, long line, int& iRes )
{
	CDsrAST_Node *pRes = NULL;
	switch( opID )
	{
	case _PROCESS_FUNCTION_INVERSE:
		iRes = CMMLError_NotSupportedCurrently;
		break;

	case _NOTYET_SYMBOLIC:
	case _NOTYET_STAT:
		iRes = CMMLError_NotSupportedCurrently;
		break;

	case _TODO_FUNCTION_QUOTIENT:
	case _TODO_FUNCTION_FACTORIAL:
	case _TODO_FUNCTION_MAX:
	case _TODO_FUNCTION_MIN:
	case _TODO_FUNCTION_XOR:

	case _TODO_FUNCTION_set_not_in_eq:
	case _TODO_FUNCTION_setdiff:
	case _TODO_FUNCTION_set_card:
	case _TODO_FUNCTION_set_cartesianproduct:

	case _TODO_FUNCTION_arccot:
	case _TODO_FUNCTION_arccoth:
	case _TODO_FUNCTION_arcsech:
	case _TODO_FUNCTION_arccosech:

	case _TODO_FUNCTION_COMPLEX_CONJUGATE:
	case _TODO_FUNCTION_COMPLEX_ARG:

	case _TODO_FUNCTION_LA_vectorproduct:
	case _TODO_FUNCTION_LA_scalarproduct:
	case _TODO_FUNCTION_LA_outerproduct:

	case _TODO_FUNCTION_INTEGRAL:
	case _TODO_FUNCTION_PARTDIFF:
	case _TODO_FUNCTION_DIVERGENCE:
	case _TODO_FUNCTION_GRAD:
	case _TODO_FUNCTION_CURL:
	case _TODO_FUNCTION_LAPLASIAN:
		iRes = CMMLError_NotSupportedCurrently;
		break;

	case _TODO_FUNCTION_SUM:
	case _TODO_FUNCTION_PRODUCT:
	case _TODO_FUNCTION_DIFF:
		iRes = CMMLError_NotSupportedCurrently;
		break;

	case _CONSTRUCT_FUNCTION_CN:
	case _CONSTRUCT_FUNCTION_CI:
		pRes = constructFunctionCall( applyNode, nodeCollection, nodeCollection[ 0 ]->getText(), line, iRes );
		break;
	case _CONSTRUCT_FUNCTION_CSYMBOL:
		{
			QString nodeCollectionText = nodeCollection[ 0 ]->getText();
			enum TAG2MML internalTagID = ::mml_getTag2MML( nodeCollectionText );
			int openmathOp = FUNCTION_STD_NONE;
			if( internalTagID != TAG2MML_NONE &&
				(openmathOp = ::mapMMLTag2DsrOp( internalTagID )) >= FUNCTION_STD_NONE && openmathOp < FUNCTION_STD_LAST )
			{
				pRes = Create_apply_DsrOp( applyNode, nodeCollection, nodeCollection[ 0 ]->getText(), openmathOp, line, iRes );
			}
			else if( ::mml_isAssignOpBody( nodeCollectionText ) )
			{
				pRes = Create_apply_DsrOp( applyNode, nodeCollection, nodeCollection[ 0 ]->getText(), OPERATOR_op_assign, line, iRes );
			}
			else
			{
				pRes = constructFunctionCall( applyNode, nodeCollection, nodeCollection[ 0 ]->getText(), line, iRes );
			}
		}
		break;

	case _CONSTRUCT_FUNCTION_APPLY:
		{
			QString newFunName = ::generateInternalVarName();
			CParseDsrSymbol *smb = new CParseDsrSymbol( newFunName, SMBTABLE_TYPE_VARIABLE, NULL );
			long idx = addDsrSymbol2Table( getCurrentSymbolTable(), *smb );

			CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
			CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
			primaryNode->line = line;
			compoundNode->Add( primaryNode );
			compoundNode->line = line;

			CDsrAST_Node *expr = Create( nodeCollection[ 0 ] );
			expr = new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_assign, line ), compoundNode, expr );
			expr->line = line;

			pRes = constructFunctionCall( applyNode, nodeCollection, getCurrentSymbolTable(), newFunName, line, iRes );
		}
		break;

	case _CONSTRUCT_FUNCTION_COMPOSE:
		iRes = CMMLError_NotSupportedCurrently;
		break;

	case _CONSTRUCT_FUNCTION_LAMBDA:
		{
			std::vector<AST_TXML_Tag*> lambdaCollection;
			nodeCollection[ 0 ]->getAllTags2Vector( lambdaCollection );
			QString newRecName = _T("");
			CDsrAST_Node *newRec = Create_lambda( nodeCollection[ 0 ], lambdaCollection, newRecName );
			if( newRec )
			{
				manualAddAstNode( newRec );
				pRes = constructFunctionCall( applyNode, nodeCollection, getCurrentSymbolTable(), newRecName, line, iRes );
			}
			else
				iRes = CMMLError_NotAFunction;
		}
		break;

	default:
		pRes = constructFunctionCall( applyNode, nodeCollection, opName, line, iRes );
	}
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////

//CDsrAST_Node* CMMLCalc::constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, AST_TXML_Tag* opTag, int& iRes )
//{
//	CParseDsrSymbol *smb = NULL;
//	long idx = lookupSymbol( opTag, smb );
//	return constructFunctionCall( applyNode, nodeCollection, idx, smb, iRes );
//}

CDsrAST_Node* CMMLCalc::constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, QString opName, long line, int& iRes )
{
	CParseDsrSymbol *smb = NULL;
	long idx = lookupSymbol( opName, smb );
	return constructFunctionCall( applyNode, nodeCollection, idx, smb, line, iRes );
}

CDsrAST_Node* CMMLCalc::constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, CParseDsrSymbolTable* dsrSymbolTable, QString opName, long line, int& iRes )
{
	long *nameID = dsrSymbolTable->is_find( opName ), idx = -1;
	CParseDsrSymbol *smb = NULL;
	if( nameID )
	{
		idx = *nameID;
		smb = dsrSymbolTable->getSymbol( idx );
	}
	return constructFunctionCall( applyNode, nodeCollection, idx, smb, line, iRes );
}

CDsrAST_Node* CMMLCalc::constructFunctionCall( AST_TXML_Tag* applyNode, std::vector<AST_TXML_Tag*>& nodeCollection, long idx, CParseDsrSymbol *smb, long line, int& iRes )
{
	CDsrAST_Node *pRes = NULL;
	if( idx != -1 && smb != NULL )
	{
		CDsrAST_Node *node = NULL;
		CDsrAST_ArgList *argsNode = NULL;
		if(nodeCollection.size() > 1)
		{
			argsNode = new CDsrAST_ArgList();
			argsNode->line = nodeCollection[ 1 ]->getLine();
		}
		for( unsigned long i = 1; i < nodeCollection.size(); i++ )
		{
			if( (node = Create( nodeCollection[ i ] )) != NULL )
				argsNode->Add( node );
		}
		CDsrDataType_Node *typeRec = smb->getDataType();
		if( smb->hasSpecialName() )
		{
			CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
			primaryNode->line = line;
			pRes = new CDsrAST_Special( primaryNode, NULL, NULL, argsNode );
			pRes->line = applyNode->getLine();

			if( smb->getArgNumber() != (long) nodeCollection.size() - 1 )
				iRes = CMMLError_ArgNumberMismatch;
		}
		else if( smb->isFunction() || (typeRec && ::isType_Var_and_Function( typeRec->getType() )) )
		{
			CDsrAST_CompoundID *compoundNode = new CDsrAST_CompoundID();
			CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
			primaryNode->line = line;
			compoundNode->Add( primaryNode );
			compoundNode->line = line;
			pRes = new CDsrAST_Function( compoundNode, NULL, NULL, argsNode );
			pRes->line = applyNode->getLine();

			if( smb->getArgNumber() != (long) nodeCollection.size() - 1 )
				iRes = CMMLError_ArgNumberMismatch;
		}
		else
		{
			iRes = CMMLError_NotAFunction;
		}
	}
	else
	{
		iRes = CMMLError_UnknownSymbol;
	}
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////
