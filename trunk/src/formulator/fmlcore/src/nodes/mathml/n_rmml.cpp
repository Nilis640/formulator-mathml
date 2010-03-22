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

#include "n_cm_treetemplate.h"
#include "../../dictionary/op_map.h"
#include "../node.h"

#define FBL_VAL_yes						_T("yes")
#define FBL_VAL_no						_T("no")
#define FBL_VAL_on						_T("on")
#define FBL_VAL_off						_T("off")

///////////////////////////////////////////////////////////////////////////////

void CFrameNode::read_mathml( AST_TXML_Compound *_math_line )
{
	if ( !_math_line || GetParent() == 0 ) return;
	CLineNode *pLineNode = new CLineNode();
	AddChild( pLineNode );
	pLineNode->read_mathml( _math_line, CLineNode_read_mathml_WITHOUT_EMPTY_NESTING_FRAMES );
	setPrecedenceAsFirstChild();
}

void CFrameNode::read_mathml( AST_TXML_Tag *_math )
{
	if( !_math ) return;

	CLineNode *pLineNode = 0;
	long i;

	int islinebreaktable = 0;
	int isnoparent = GetParent() == 0;

	if( !isnoparent && ::mml_is_table_tag( _math->getTagName() ) && _math->getTags() != NULL )
	{
		AST_TXML_Attribute *_attr = _math->getAttribute( FBL_ATTR_linebreak );
		if( _attr && _attr->getStringValue() == FBL_VAL_true )
			islinebreaktable = 1;
	}

	if( isnoparent || islinebreaktable )
	{
		if( isnoparent )
		{
			if( !::mml_is_root_tag( _math->getTagName() ) || !_math->getTags() )
				return;
			_math->formatBlank();

			if( !_math->getTags() )
			{
				pLineNode = new CLineNode();
				AddChild( pLineNode );
				return;
			}
		}

		std::vector<long> line_length;
		AST_TXML_Compound *_math_tags = _math->getTags();
		AST_TXML_Tag *mtr, *mtable = NULL;

		int istablecorrect = 0;
		if( isnoparent )
			istablecorrect = mml_is_table_tag_correct( _math_tags, line_length );
		else
		{
			istablecorrect = mml_is_table_tag_correct( _math, line_length );
			mtable = _math;
		}

		if( istablecorrect )
		{
			for( i = 0; i < (long) line_length.size(); i++ )
				if( line_length[ i ] != 1 ) break;
			if( i == (long) line_length.size() )
			{
				if( mtable == NULL )
					mtable = (AST_TXML_Tag*)_math_tags->getChild( 0 );

				int isMultiline = 0;
				if( ::mml_is_table_tag( mtable->getTagName() ) )
				{
					AST_TXML_Attribute *_attr = mtable->getAttribute( FBL_ATTR_linebreak );
					if( _attr && _attr->getStringValue() == FBL_VAL_true )
						isMultiline = 1;
				}

				if( isMultiline )
				{
					for( i = 0; 
						 i < mtable->getTags()->getChildNumber() && 
						 mtable->getTags()->getChild( i )->getNodeType() == ASTT_TXML_Tag; 
						 i++ )
					{
						pLineNode = new CLineNode();
						AddChild( pLineNode );
						mtr = (AST_TXML_Tag*)mtable->getTags()->getChild( i );
						mtr = (AST_TXML_Tag*)mtr->getTags()->getChild( 0 );
						pLineNode->read_mathml( mtr->getTags(), CLineNode_read_mathml_WITHOUT_EMPTY_NESTING_FRAMES );
					}
					return;
				}
			}
		}

		if( isnoparent )
		{
			pLineNode = new CLineNode();
			pLineNode->SetLevel( GetLevel() );
			AddChild( pLineNode );
			pLineNode->read_mathml( _math_tags, CLineNode_read_mathml_WITHOUT_EMPTY_NESTING_FRAMES );
			setPrecedenceAsFirstChild();
			return;
		}
	}

	pLineNode = new CLineNode();
	pLineNode->SetLevel( GetLevel() );
	AddChild( pLineNode );
	pLineNode->read_mathml( _math );
	setPrecedenceAsFirstChild();
}

void CLineNode::read_mathml( AST_TXML_Tag *node )
{
	if( !node ) return;

	CNode *nodeLinebreak = 0;
	if( ::mml_is_row_tag( node->getTagName() ) && !node->getTags() &&
		!node->getFlags().isSetFlag(OLIMP_MATHML_FLAG__MROW_CONTENT_AUTODETECT) )
		return;
	else
	{
		nodeLinebreak = read_mathml_mrow2empty_frame( node );
		setPrecedenceAsFirstChild();
	}
}

void CLineNode::read_mathml( AST_TXML_Compound *_node_lst, int mode )
{
	if( !_node_lst ) return;

	CNode *nodeLinebreak = 0;

	long i;
	AST_TXML_Tag *node;

	AST_TXML_Compound *_tags = 0;
	int isEraseRow = 0;
	if( GetChildCount() == 0 && _node_lst->getChildNumber() == 1 && 
		_node_lst->getChild( 0 ) && _node_lst->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
	{
		node = (AST_TXML_Tag*)(_node_lst->getChild( 0 ));
		if( ::mml_is_row_tag( node->getTagName() ) && 
			!node->getFlags().isSetFlag(OLIMP_MATHML_FLAG__MROW_KEEP1CHILD) && 
			!node->getFlags().isSetFlag(OLIMP_MATHML_FLAG__MROW_CONTENT_AUTODETECT) )
		{
			if( (_tags = node->getTags()) == 0 ) return;
			isEraseRow = 1;
		}
	}
	if( _tags && isEraseRow )
	{
		read_mathml( _tags, mode );
		setPrecedenceAsFirstChild();
		return;
	}

	for( i = 0; i < _node_lst->getChildNumber(); i++ )
	{
		if( !_node_lst->getChild( i ) || 
			_node_lst->getChild( i )->getNodeType() != ASTT_TXML_Tag )
			continue;
		node = (AST_TXML_Tag*)(_node_lst->getChild( i ));
		nodeLinebreak = read_mathml_mrow2empty_frame( node );
	}
	setPrecedenceAsFirstChild();
}

CNode* CLineNode::read_mathml_mrow2empty_frame( AST_TXML_Tag *node )
{
	if( !node ) return 0;

	CNode *nodeLinebreak = 0;

	if( node->getFlags().isSetFlag( FL_XML_PROCESSING_INSTRUCTION ) )
	{
		InsertMML_CM_standalone_transparent( TAG2MML_mrow, node );
		return nodeLinebreak;
	}

	enum TAG2MML tag_id = ::mml_getTag2MML( node->getTagName() );
	CNode* newNode = 0;

	switch( tag_id )
	{
	case TAG2MML_mrow:
		{
			if( !node->getFlags().isSetFlag(OLIMP_MATHML_FLAG__MROW_KEEP1CHILD) &&
                                ((node->getTags() && node->getTags()->getChildNumber() == 1) ||
				 node->getFlags().isSetFlag(OLIMP_MATHML_FLAG__MROW_ARTIFICIAL )))
				read_mathml( node->getTags() );
			else
				newNode = InsertMMLFormulaNode( node );
		}
		break;

	case TAG2MML_mn:
	case TAG2MML_mo:
	case TAG2MML_mi:
	case TAG2MML_mtext:
	case TAG2MML_ms:
	case TAG2MML_mglyph:
		newNode = InsertMMLTokenNode( node );
		break;

	case TAG2MML_mspace:
		nodeLinebreak = InsertMMLTokenNode( node );
		newNode = nodeLinebreak;
		break;

	case TAG2MML_malignmark:
	case TAG2MML_maligngroup:
		break;

	case TAG2MML_mtable:
	case TAG2MML_mlabeledtr:
	case TAG2MML_mtr:
	case TAG2MML_mtd:
	case TAG2MML_mfenced:
	case TAG2MML_mfrac:
	case TAG2MML_mstyle:
	case TAG2MML_msqrt:
	case TAG2MML_merror:
	case TAG2MML_menclose:
	case TAG2MML_mpadded:
	case TAG2MML_mphantom:
	case TAG2MML_maction:
	case TAG2MML_munder:
	case TAG2MML_mover:
	case TAG2MML_munderover:
	case TAG2MML_msup:
	case TAG2MML_msub:
	case TAG2MML_mmultiscripts:
		newNode = InsertMMLFormulaNode( node );
		break;

	case TAG2MML_semantics:
		newNode = InsertMML_CM_semantics( tag_id, node );
		break;
	case TAG2MML_annotation:
		newNode = InsertMML_CM_annotation( tag_id, node );
		break;
	case TAG2MML_annotation_xml:
		newNode = InsertMML_CM_annotation_xml( tag_id, node );
		break;

	case TAG2MML_piecewise:
		newNode = InsertMML_CM_piecewise( tag_id, node );
		break;
	case TAG2MML_piece:
	case TAG2MML_otherwise:
		newNode = InsertMML_CM_standalone_piece( tag_id, node );
		break;

	case TAG2MML_apply:
	case TAG2MML_reln:
	case TAG2MML_condition:
		newNode = InsertMML_CM_apply( tag_id, node );
		break;

	case TAG2MML_sep:
		break;

	case TAG2MML_declare:
		newNode = InsertMML_CM_construct_declare( tag_id, node );
		break;

	case TAG2MML_selector:
	case TAG2MML_inverse:
	case TAG2MML_transpose:
		newNode = InsertMML_CM_standalone_opaque( tag_id, node );
		break;

	case TAG2MML_momentabout:
	case TAG2MML_matrixrow:
	case TAG2MML_lowlimit:
	case TAG2MML_uplimit:
	case TAG2MML_fn:
	case TAG2MML_bvar:
	case TAG2MML_degree:
	case TAG2MML_logbase:
	case TAG2MML_domainofapplication:

	case TAG2MML_math:
		newNode = InsertMML_CM_standalone_transparent( tag_id, node );
		break;

	case TAG2MML_interval:
		newNode = InsertMML_CM_construct_interval( tag_id, node );
		break;
	case TAG2MML_lambda:
		newNode = InsertMML_CM_construct_lamda( tag_id, node );
		break;
	case TAG2MML_set:
	case TAG2MML_list:
		newNode = InsertMML_CM_construct_set( tag_id, node );
		break;
	case TAG2MML_vector:
		newNode = InsertMML_CM_construct_vector( tag_id, _T(""), node );
		break;
	case TAG2MML_matrix:
		newNode = InsertMML_CM_construct_vector( tag_id, FBL_TAG_matrixrow, node );
		break;

	case TAG2MML_cn:
		newNode = InsertMMLContent_cn( node );
		break;
	case TAG2MML_ci:
		newNode = InsertMMLContent_ci( node );
		break;
	case TAG2MML_csymbol:
		newNode = InsertMMLContent_csymbol( node );
		break;

	case TAG2MML_determinant:
	//
	case TAG2MML_limit:
	//
	case TAG2MML_ident:
	case TAG2MML_domain:
	case TAG2MML_codomain:
	case TAG2MML_image:
	//
	case TAG2MML_divergence:
	case TAG2MML_curl:
	//
	case TAG2MML_median:
	case TAG2MML_mode:
	//
	case TAG2MML_max:
	case TAG2MML_min:
	case TAG2MML_rem:
	case TAG2MML_gcd:

	case TAG2MML_card:
	case TAG2MML_mean:
	//
	case TAG2MML_abs:
	case TAG2MML_conjugate:
	case TAG2MML_divide:
	case TAG2MML_power:
	case TAG2MML_root:
	case TAG2MML_floor:
	case TAG2MML_ceiling:
	case TAG2MML_quotient:

	//
	case TAG2MML_arg:
	case TAG2MML_lcm:
	//
	case TAG2MML_ln:
	case TAG2MML_log:
	case TAG2MML_sin:
	case TAG2MML_cos:
	case TAG2MML_tan:
	case TAG2MML_sec:
	case TAG2MML_csc:
	case TAG2MML_cot:
	case TAG2MML_sinh:
	case TAG2MML_cosh:
	case TAG2MML_tanh:
	case TAG2MML_sech:
	case TAG2MML_csch:
	case TAG2MML_coth:
		newNode = InsertMML_CM_standalone_operator( tag_id, node, FSTYLE_FUNCTION );
		break;

	case TAG2MML_arcsin:
	case TAG2MML_arccos:
	case TAG2MML_arctan:
	case TAG2MML_arccosh:
	case TAG2MML_arccot:
	case TAG2MML_arccoth:
	case TAG2MML_arccsc:
	case TAG2MML_arccsch:
	case TAG2MML_arcsec:
	case TAG2MML_arcsech:
	case TAG2MML_arcsinh:
	case TAG2MML_arctanh:
		if( ::isRenderArcFunAsPower() )
			newNode = InsertMML_CM_standalone_operator_msup( tag_id, node );
		else
			newNode = InsertMML_CM_standalone_operator( tag_id, node, FSTYLE_FUNCTION );
		break;

	case TAG2MML_laplacian:
	case TAG2MML_variance:
		newNode = InsertMML_CM_standalone_operator_msup( tag_id, node );
		break;

	case TAG2MML_exp:
	//
	case TAG2MML_moment:
	//
	case TAG2MML_diff:
	case TAG2MML_partialdiff:
	//
	case TAG2MML_grad:
	case TAG2MML_union:
	case TAG2MML_intersect:
	case TAG2MML_in:
	case TAG2MML_notin:
	case TAG2MML_subset:
	case TAG2MML_prsubset:
	case TAG2MML_notsubset:
	case TAG2MML_notprsubset:
	case TAG2MML_setdiff:
	case TAG2MML_cartesianproduct:
	//
	case TAG2MML_vectorproduct:
	case TAG2MML_scalarproduct:
	case TAG2MML_outerproduct:
	//
	case TAG2MML_sum:
	case TAG2MML_product:
	case TAG2MML_tendsto:
	//
	case TAG2MML_compose:
	//
	case TAG2MML_int:
	//
	case TAG2MML_eq:
	case TAG2MML_neq:
	case TAG2MML_gt:
	case TAG2MML_lt:
	case TAG2MML_geq:
	case TAG2MML_leq:
	case TAG2MML_equivalent:
	case TAG2MML_approx:
	case TAG2MML_factorof:
	//
	case TAG2MML_factorial:
	case TAG2MML_minus:
	case TAG2MML_plus:
	case TAG2MML_times:
	case TAG2MML_and:
	case TAG2MML_or:
	case TAG2MML_xor:
	case TAG2MML_not:
	case TAG2MML_implies:
	case TAG2MML_forall:
	case TAG2MML_exists:
	case TAG2MML_real:
	case TAG2MML_imaginary:
	//
	case TAG2MML_sdev:

	case TAG2MML_integers:
	case TAG2MML_reals:
	case TAG2MML_rationals:
	case TAG2MML_naturalnumbers:
	case TAG2MML_complexes:
	case TAG2MML_primes:
	case TAG2MML_exponentiale:
	case TAG2MML_imaginaryi:
	case TAG2MML_notanumber:
	case TAG2MML_true:
	case TAG2MML_false:
	case TAG2MML_emptyset:
	case TAG2MML_pi:
	case TAG2MML_eulergamma:
	case TAG2MML_infinity:

	case TAG2MML_nop:
		newNode = InsertMML_CM_standalone_operator( tag_id, node );
		break;

	default:
		newNode = InsertMMLFormulaNode( node );
	}


	if( newNode )
	{
		int isCannotEdit = 0, isImmovable = 0;
		QString attrValue;

		if( newNode->getMathMLAttr().getUnfilteredAttr( FBL_ATTR_formulator_readonly, attrValue ) == -1 && ::isAutographEditing() )
			newNode->getMathMLAttr().getUnfilteredAttr( FBL_ATTR_autograph_readonly, attrValue );
		if( attrValue == FBL_VAL_yes || attrValue == FBL_VAL_on || attrValue == FBL_VAL_true )
			isCannotEdit = 1;

		attrValue = _T("");
		if( newNode->getMathMLAttr().getUnfilteredAttr( FBL_ATTR_formulator_immovable, attrValue ) == -1 && ::isAutographEditing() )
			newNode->getMathMLAttr().getUnfilteredAttr( FBL_ATTR_autograph_immovable, attrValue );
		if( attrValue == FBL_VAL_yes || attrValue == FBL_VAL_on || attrValue == FBL_VAL_true )
			isImmovable = 1;

		if( isCannotEdit )
		{
			if( isImmovable )
				newNode->iterateNodeTree( setRecursiveReadOnly, 0 );
			else
				newNode->iterateNodeTree( setRecursiveCannotEdit, 0 );
		}
		else if( isImmovable )
			newNode->iterateNodeTree( setRecursiveImmovable, 0 );
	}

	return nodeLinebreak;
}

///////////////////////////////////////////////////////////////////////////////
