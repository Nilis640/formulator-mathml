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

int CMMLCalc::CreateAST( AST_TXML_Tag* mathRoot )
{
	if( !mathRoot ) return -1;

	dispose();
	m_ast = new CDsrAST_StmtList();
	setGlobalSymbolTable();

	if( mathRoot->getTags() )
	{
		AST_TXML_Node *_xml_child;
		for( long i = 0; i < mathRoot->getTags()->getChildNumber(); i++ )
		{
			if( (_xml_child = mathRoot->getTags()->getChild( i )) != NULL && _xml_child->getNodeType() == ASTT_TXML_Tag )
			{
				CDsrAST_Node *pRes = Create( (AST_TXML_Tag*) _xml_child );
				if( pRes )
				{
					//m_ast->Add( pRes );

					QString nameDummyVar = QString("$%1").arg(i);
					long ln = _xml_child->getLine();
					CDsrAST_CompoundID *pDummyVar = new CDsrAST_CompoundID();
					long idx = getCurrentSymbolTable()->Add( *(new CParseDsrSymbol( nameDummyVar )) );

					CDsrAST_PrimaryID *primaryNode = new CDsrAST_PrimaryID( idx );
					pDummyVar->Add( primaryNode );
					pDummyVar->line = primaryNode->line = ln;

					CDsrAST_Node *pDummyAssign = new CDsrAST_Expr( new CDsrAST_Op( OPERATOR_op_assign, ln ), pDummyVar, pRes );
					pDummyAssign->line = ln;
					m_ast->Add( pDummyAssign );

					m_var2mml.push_back( nameDummyVar );

					//QString aspectStr;
					//_xml_child->getXML( aspectStr );
					//m_var2mml[ nameDummyVar ] = aspectStr;
				}
			}
		}
	}

	return CMMLError_NONE;
}

CDsrAST_Node* CMMLCalc::Create( AST_TXML_Tag* xmlNode )
{
	if( !xmlNode ) return NULL;
	QString emptyStr;

	std::vector<AST_TXML_Tag*> nodeCollection;
	xmlNode->getAllTags2Vector( nodeCollection );

	CDsrAST_Node *pRes = NULL;
	enum TAG2MML tagID = ::mml_getTag2MML( xmlNode->getTagName() );
	switch( tagID )
	{
	// === work
	case TAG2MML_apply:
	case TAG2MML_reln:
		pRes = Create_apply( xmlNode, nodeCollection );
		break;
	case TAG2MML_csymbol:
	case TAG2MML_ci:
		pRes = Create_ci( xmlNode );
		break;
	case TAG2MML_cn:
		pRes = Create_cn( xmlNode );
		break;

	// === transparent
	case TAG2MML_condition:
		pRes = Create_condition( xmlNode );
		break;

	// === "Constant and Symbol Elements"
	case TAG2MML_integers:
	case TAG2MML_reals:
	case TAG2MML_rationals:
	case TAG2MML_naturalnumbers:
	case TAG2MML_complexes:
	case TAG2MML_exponentiale:
	case TAG2MML_imaginaryi:
	case TAG2MML_pi:
	case TAG2MML_eulergamma:
		pRes = Create_constant( QString(_T("@")) + xmlNode->getTagName(), xmlNode->getLine() );
		break;
	case TAG2MML_emptyset:
		pRes = Create_constant( ::ml_Op2String( FORMULA_set_empty ), xmlNode->getLine() );
		break;
	case TAG2MML_true:
	case TAG2MML_false:
		pRes = Create_constant( xmlNode->getTagName(), xmlNode->getLine() );
		break;

	// === Constructors
	case TAG2MML_set:
	case TAG2MML_list:
		pRes = Create_set( xmlNode, nodeCollection );
		break;
	case TAG2MML_vector:
		pRes = Create_vector( xmlNode, nodeCollection );
		break;
	case TAG2MML_matrix:
		pRes = Create_matrix( xmlNode, nodeCollection );
		break;

	// === Declarations
	case TAG2MML_lambda:
		pRes = Create_lambda( xmlNode, nodeCollection, emptyStr );
		break;
	case TAG2MML_declare:
		pRes = Create_declare( xmlNode, nodeCollection );
		break;

	// === deprecated (absent is MathML 3.0)
	case TAG2MML_fn:
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_IgnoringNode_Deprecated), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName() );
		break;

	case TAG2MML_semantics:
		// nothing to do: just ignore the whole node
		break;

	// === to do
	case TAG2MML_piecewise:
		pRes = Create_piecewise( xmlNode, nodeCollection );
		break;
	
	// === ignored (can't be on this level or this is an error)
	case TAG2MML_inverse:				// only inside <apply>
	case TAG2MML_compose:				// only inside <apply>
	case TAG2MML_domain:				// only inside <apply>
	case TAG2MML_codomain:				// only inside <apply>
	case TAG2MML_image:					// only inside <apply>
	case TAG2MML_domainofapplication:	// only inside <apply>
	case TAG2MML_sep:					// only inside <cn>
	case TAG2MML_piece:					// only inside <piecewise>
	case TAG2MML_otherwise:				// only inside <piecewise>
	case TAG2MML_quotient:				// only inside <apply>
	case TAG2MML_factorial:				// only inside <apply>
	case TAG2MML_divide:				// only inside <apply>
	case TAG2MML_max:					// only inside <apply>
	case TAG2MML_min:					// only inside <apply>
	case TAG2MML_minus:					// only inside <apply>
	case TAG2MML_plus:					// only inside <apply>
	case TAG2MML_power:					// only inside <apply>
	case TAG2MML_times:					// only inside <apply>
	case TAG2MML_root:					// only inside <apply>
	case TAG2MML_gcd:					// only inside <apply>
	case TAG2MML_and:					// only inside <apply>
	case TAG2MML_or:					// only inside <apply>
	case TAG2MML_xor:					// only inside <apply>
	case TAG2MML_not:					// only inside <apply>
	case TAG2MML_implies:				// only inside <apply>
	case TAG2MML_forall:				// only inside <apply>
	case TAG2MML_exists:				// only inside <apply>
	case TAG2MML_abs:					// only inside <apply>
	case TAG2MML_conjugate:				// only inside <apply>
	case TAG2MML_arg:					// only inside <apply>
	case TAG2MML_real:					// only inside <apply>
	case TAG2MML_imaginary:				// only inside <apply>
	case TAG2MML_lcm:					// only inside <apply>
	case TAG2MML_floor:					// only inside <apply>
	case TAG2MML_ceiling:				// only inside <apply>
	case TAG2MML_eq:					// only inside <apply>
	case TAG2MML_neq:					// only inside <apply>
	case TAG2MML_gt:					// only inside <apply>
	case TAG2MML_lt:					// only inside <apply>
	case TAG2MML_geq:					// only inside <apply>
	case TAG2MML_leq:					// only inside <apply>
	case TAG2MML_equivalent:			// only inside <apply>
	case TAG2MML_approx:				// only inside <apply>
	case TAG2MML_factorof:				// only inside <apply>
	case TAG2MML_int:					// only inside <apply>
	case TAG2MML_diff:					// only inside <apply>
	case TAG2MML_partialdiff:			// only inside <apply>
	case TAG2MML_lowlimit:				// only inside some container
	case TAG2MML_uplimit:				// only inside <apply>
	case TAG2MML_bvar:					// only inside <apply>
	case TAG2MML_degree:				// only inside <apply>
	case TAG2MML_divergence:			// only inside <apply>
	case TAG2MML_grad:					// only inside <apply>
	case TAG2MML_curl:					// only inside <apply>
	case TAG2MML_laplacian:				// only inside <apply>
	case TAG2MML_union:					// only inside <apply>
	case TAG2MML_intersect:				// only inside <apply>
	case TAG2MML_in:					// only inside <apply>
	case TAG2MML_notin:					// only inside <apply>
	case TAG2MML_subset:				// only inside <apply>
	case TAG2MML_prsubset:				// only inside <apply>
	case TAG2MML_notsubset:				// only inside <apply>
	case TAG2MML_notprsubset:			// only inside <apply>
	case TAG2MML_setdiff:				// only inside <apply>
	case TAG2MML_card:					// only inside <apply>
	case TAG2MML_cartesianproduct:		// only inside <apply>
	case TAG2MML_sum:					// only inside <apply>
	case TAG2MML_product:				// only inside <apply>
	case TAG2MML_limit:					// only inside <apply>
	case TAG2MML_tendsto:				// only inside <apply>
	case TAG2MML_exp:					// only inside <apply>
	case TAG2MML_ln:					// only inside <apply>
	case TAG2MML_log:					// only inside <apply>
	case TAG2MML_sin:					// only inside <apply>
	case TAG2MML_cos:					// only inside <apply>
	case TAG2MML_tan:					// only inside <apply>
	case TAG2MML_sec:					// only inside <apply>
	case TAG2MML_csc:					// only inside <apply>
	case TAG2MML_cot:					// only inside <apply>
	case TAG2MML_sinh:					// only inside <apply>
	case TAG2MML_cosh:					// only inside <apply>
	case TAG2MML_tanh:					// only inside <apply>
	case TAG2MML_sech:					// only inside <apply>
	case TAG2MML_csch:					// only inside <apply>
	case TAG2MML_coth:					// only inside <apply>
	case TAG2MML_arcsin:				// only inside <apply>
	case TAG2MML_arccos:				// only inside <apply>
	case TAG2MML_arctan:				// only inside <apply>
	case TAG2MML_arccosh:				// only inside <apply>
	case TAG2MML_arccot:				// only inside <apply>
	case TAG2MML_arccoth:				// only inside <apply>
	case TAG2MML_arccsc:				// only inside <apply>
	case TAG2MML_arccsch:				// only inside <apply>
	case TAG2MML_arcsec:				// only inside <apply>
	case TAG2MML_arcsech:				// only inside <apply>
	case TAG2MML_arcsinh:				// only inside <apply>
	case TAG2MML_arctanh:				// only inside <apply>
	case TAG2MML_mean:					// only inside <apply>
	case TAG2MML_sdev:					// only inside <apply>
	case TAG2MML_variance:				// only inside <apply>
	case TAG2MML_median:				// only inside <apply>
	case TAG2MML_mode:					// only inside <apply>
	case TAG2MML_moment:				// only inside <apply>
	case TAG2MML_momentabout:			// only inside <apply>
	case TAG2MML_matrixrow:				// only inside <matrix>
	case TAG2MML_determinant:			// only inside <apply>
	case TAG2MML_transpose:				// only inside <apply>
	case TAG2MML_selector:				// only inside <apply>
	case TAG2MML_vectorproduct:			// only inside <apply>
	case TAG2MML_scalarproduct:			// only inside <apply>
	case TAG2MML_outerproduct:			// only inside <apply>
	case TAG2MML_annotation:			// only inside <semantics>
	case TAG2MML_annotation_xml:		// only inside <apply>
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_IgnoringNode_WrongContainer), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName() );
		break;

	// === to do "Constant and Symbol Elements"
	case TAG2MML_primes:
	case TAG2MML_notanumber:
	case TAG2MML_infinity:
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_NotSupportedCurrently), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName() );
		break;

	// === not yet supported
	case TAG2MML_interval:
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_NotSupportedCurrently), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName() );
		break;

	// === questionable
	case TAG2MML_ident:
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_NotSupportedCurrently), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName() );
		break;

	default:
		m_dmm.AddLine( m_dmmPage, getErrorMsg(CMMLError_UnknownXmlTag), QString(m_taskName), xmlNode->getLine(), xmlNode->getTagName() );
	}
	return pRes;
}

///////////////////////////////////////////////////////////////////////////////
