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

#include "../../mathml/mml_units.h"
#include "../../mathml/mml_tag2mml.h"
#include "n_cm_treetemplate.h"
#include "n_rmml_cm.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

int _CM_make_fence_arg( long arg_idx, long precedence, CCM_apply_args& _args );

///////////////////////////////////////////////////////////////////////////////

void _CM_apply__special_pump( std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	long nops = (long) to_read.size(), i;
	if( nops + 1 < (long) _args.l2do.size() && _args.l2do[ nops ] )
	{
		for( i = nops + 1; i < (long) _args.l2do.size(); i++ )
		{
			switch( _args.op_id )
			{
			case TAG2MML_selector:
				_args.l2do[ nops ]->InsertMML_CM_create_markup_rigid_token(
										FBL_TAG_mtext,
										::getComaUsingCurrentLocale() );
				break;
			case TAG2MML_mean:
			case TAG2MML_sdev:
			case TAG2MML_variance:
				_args.l2do[ nops ]->InsertMML_CM_create_markup_rigid_token( 
										FBL_TAG_mo,
										::getComaUsingCurrentLocale() );
				break;
			default:
				break;
			}
			_args.l2do[ nops ]->PumpLine2Line( _args.l2do[ i ] );
		}
	}

	for( i = 0; i < (long) to_read.size() && i < (long) _args.l2do.size() - 1; i++ )
	{
		if( to_read[ i ] && _args.l2do[ i + 1 ] )
		{
			//int ret = ::_CM_apply__special_PumpLine2Line( to_read[ i ], _args.l2do[ i + 1 ] );
			//if( ret )
			to_read[ i ]->PumpLine2Line( _args.l2do[ i + 1 ] );
		}
	}
}

CFormulaNode* CLineNode::InsertMML_CM_apply__special( CCM_apply_args& _args )
{
	CFormulaNode *_node_apply = 0;
	std::vector<CLineNode*> to_read;
	int moment_about = 0;
	long i = 0;
	switch( _args.op_id )
	{
/*
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
		_node_apply = createEmptyForm__arcfun( to_read, _args );
		break;
*/
	case TAG2MML_exp:
		_node_apply = createEmptyForm__exp( to_read, _args );
		break;
	case TAG2MML_power:
		_node_apply = createEmptyForm__pow( to_read, _args );
		break;
	case TAG2MML_inverse:
		_node_apply = createEmptyForm__inverse( to_read, _args );
		break;
	case TAG2MML_transpose:
		_node_apply = createEmptyForm__transpose( to_read, _args );
		break;
	case TAG2MML_card:
	case TAG2MML_abs:
		_node_apply = createEmptyForm__abs( to_read, _args );
		break;
	case TAG2MML_conjugate:
		_node_apply = createEmptyForm__conjugate( to_read, _args );
		break;
	case TAG2MML_floor:
		_node_apply = createEmptyForm__floor( to_read, _args );
		break;
	case TAG2MML_ceiling:
		_node_apply = createEmptyForm__ceiling( to_read, _args );
		break;
	case TAG2MML_divide:
		_node_apply = createEmptyForm__divide( to_read, _args );
		break;
	case TAG2MML_quotient:
		_node_apply = createEmptyForm__quotient( to_read, _args );
		break;
	case TAG2MML_mean:
		_node_apply = createEmptyForm__mean( to_read, _args );
		break;
	case TAG2MML_root:
		_node_apply = createEmptyForm__root( to_read, _args );
		break;
	case TAG2MML_log:
		_node_apply = createEmptyForm__log( to_read, _args );
		break;
	case TAG2MML_limit:
		_node_apply = createEmptyForm__sum( to_read, _args, _T("lim"), _T("mi") );
		break;
	case TAG2MML_sum:
		_node_apply = createEmptyForm__sum( to_read, _args, _T("&sum;"), _T("mo") );
		break;
	case TAG2MML_product:
		_node_apply = createEmptyForm__sum( to_read, _args, _T("&prod;"), _T("mo") );
		break;
	case TAG2MML_int:
		_node_apply = createEmptyForm__sum( to_read, _args, _T("&int;"), _T("mo") );
		break;
	case TAG2MML_max:
	case TAG2MML_min:
		_node_apply = createEmptyForm__maxmin( _args );
		break;
	case TAG2MML_moment:
		_node_apply = createEmptyForm__moment( to_read, _args, moment_about );
		break;
	case TAG2MML_selector:
		_node_apply = createEmptyForm__selector( to_read, _args );
		break;
	case TAG2MML_exists:
		_node_apply = createEmptyForm__forall_exists( to_read, _args, _T("&exist;") );
		break;
	case TAG2MML_forall:
		_node_apply = createEmptyForm__forall_exists( to_read, _args, _T("&forall;") );
		break;
	case TAG2MML_diff:
		_node_apply = createEmptyForm__diff( to_read, _args );
		break;
	case TAG2MML_partialdiff:
		_node_apply = createEmptyForm__partialdiff( to_read, _args );
		break;

	case TAG2MML_divergence:
	case TAG2MML_grad:
	case TAG2MML_curl:
	case TAG2MML_laplacian:
		_node_apply = createEmptyForm__calculus( to_read, _args );
		break;

	case TAG2MML_nop:
	case TAG2MML_plus:
	case TAG2MML_times:
	case TAG2MML_gcd:
	case TAG2MML_lcm:
	case TAG2MML_sdev:
	case TAG2MML_variance:
	case TAG2MML_median:
	case TAG2MML_mode:
	case TAG2MML_and:
	case TAG2MML_or:
	case TAG2MML_xor:
	case TAG2MML_union:
	case TAG2MML_intersect:
	case TAG2MML_cartesianproduct:
	case TAG2MML_compose:
	case TAG2MML_eq:
	case TAG2MML_leq:
	case TAG2MML_lt:
	case TAG2MML_geq:
	case TAG2MML_gt:
	case TAG2MML_subset:
	case TAG2MML_prsubset:
		{
			AST_TXML_Tag *xml_node = 0;
			if( _args.xml_n && _args.xml_n->getTags() && 
				_args.xml_n->getTags()->getChild( 0 ) && 
				_args.xml_n->getTags()->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
			{
				xml_node = (AST_TXML_Tag*)(_args.xml_n->getTags()->getChild( 0 ));
			}
			QString tag = ::mml_getCM2PMTag_standalone( _args.op_id, xml_node );
			QString txt = ::mml_getCM2PMText_standalone( _args.op_id, xml_node );
			_node_apply = createEmptyForm__sum( to_read, _args, txt, tag, 1 /* use largeop='true' */ );
		}
		break;

	default:
		for( i = 0; i < (long) _args.l2do.size(); i++ )
		{
			if( _args.l2do[ i ] )
				PumpLine2Line( _args.l2do[ i ] );
		}
	}

	if( _node_apply )
	{
		if( _args.l2do[ 0 ] )
		{
			CLineNode *_l = new CLineNode( _args.l2do[ 0 ] );
			_node_apply->setAddonNode( _l );

			switch( _args.op_id )
			{
			case TAG2MML_moment:
				_node_apply->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_MOMENTABOUT_ADDON_OP;
				break;
			case TAG2MML_root:
				_node_apply->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP__BACKRECURSIVE_TREE;
				break;
			default:
				_node_apply->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP;
			}
		}
		else
		{
			_node_apply->to_mathml_data.is_content = 
				CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		}
		::_CM_apply__special_pump( to_read, _args );

		switch( _args.op_id )
		{
		case TAG2MML_int:
			{
				for( i = 1; i < _args.xml_tags->getChildNumber(); i++ )
				{
					QString tn = ((AST_TXML_Tag*)(_args.xml_tags->getChild( i )))->getTagName();
					if( tn == FBL_TAG_bvar )
					{
						CLineNode *_l = _node_apply->getFormulaLineChild( 0 );
						if( _l )
						{
							_l->InsertMML_CM_standalone_diff_bvar( FBL_TAG_mi, _T("d"),
									(AST_TXML_Tag*)(_args.xml_tags->getChild( i )) );
							CFormulaNode *frmNode = _l->getLastContentFormulaChild();
							if( frmNode )
							{
								frmNode->setReferenced();
								frmNode->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( i );
							}
						}
					}
				}
			}
			break;
		case TAG2MML_divergence:
		case TAG2MML_curl:
			{
				CLineNode *_line = _node_apply->getFormulaLineChild( 0 );
				if( _line )
				{
					CNode *_n = _line->GetFirstChild();
					if( _n && _n->GetType() == NODE_PLANETEXT && _n->getRSpace() == 0 )
					{
						RealFde value = 0.0;
						if( ::mml_convertSpaceName2FloatValue( 0, FBL_VAL_thinmathspace, value, ::getCurrentFormulatorStyle().getUnit2PX() ) != -1 )
							_n->setRSpace( value );
					}
				}
			}
			break;
		case TAG2MML_limit:
			{
				CNode *_n = 0;
				CLineNode *_line = _node_apply->getFormulaLineChild( 0 );
				if( _line && (_n = _line->GetFirstChild()) != 0 && _n->GetType() == NODE_FORMULA )
				{
					CFormulaNode *frmNode = _node_apply->getFormulaDeepFormulaChild( 0, 0 );
					
					if( frmNode && (_n = frmNode->getFormulaDeepChild( 0, 0 )) != 0 &&
						_n->GetType() == NODE_PLANETEXT && _n->getRSpace() == 0 )
					{
						RealFde value = 0.0;
						if( ::mml_convertSpaceName2FloatValue( 0, FBL_VAL_thinmathspace, value, ::getCurrentFormulatorStyle().getUnit2PX() ) != -1 )
							_n->setRSpace( value );
					}
				}
			}
			break;
		default:
			break;
		}
	}

	return _node_apply;
}

///////////////////////////////////////////////////////////////////////////////
// A R C F U N ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
CFormulaNode *CLineNode::createEmptyForm__arcfun( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	return 0;
}
*/
///////////////////////////////////////////////////////////////////////////////
// P O W //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__exp( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( 
		new AST_TXML_Tag( &miV, 0, 
			new AST_TXML_Value( &expV ) ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &msupV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		CNode *frame1st = retnode->GetChild( 0 );
		if( frame1st )
			frame1st->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		to_read[ 0 ] = retnode->getFormulaLineChild( 1 );
	}

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__pow( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	if( _args.l2do.size() > 1 && _args.l2do[ 1 ] )
	{
		long argPrecedence = 
			( _args.l2do[ 1 ]->GetChildCount() == 1 && _args.l2do[ 1 ]->GetFirstChild() ) ?
                                (long) ::_CM_getPrecedence( _args.l2do[ 1 ]->GetFirstChild() ) :
                                (long) mmlPrecedence_Atom;
		if( argPrecedence > mmlPrecedence_Atom )
		{
			CLineNode *newline = new CLineNode();
			CFormulaNode *_n = newline->InsertMMLFormulaNode( MMLContent_TT_mrow().getRoot() );
			if( _n )
			{
				_n->to_mathml_data.name = _T("");
				_n->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				CLineNode *_l = _n->getFormulaLineChild( 0 );
				if( _l )
				{
					_l->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("(") );
					_l->PumpLine2Line( _args.l2do[ 1 ] );
					_l->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T(")") );
				}
			}
			delete _args.l2do[ 1 ];
			_args.l2do[ 1 ] = newline;
		}

	}

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &msupV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );
		to_read[ 1 ] = retnode->getFormulaLineChild( 1 );
	}

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__inverse( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( 
		new AST_TXML_Tag( &mnV, 0, 
			new AST_TXML_Value( &minus1V ) ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &msupV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );
		CNode *frame1st = retnode->GetChild( 1 );
		if( frame1st )
			frame1st->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
	}

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__transpose( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &mathvariantV, &normalV ) );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( new AST_TXML_Tag( &miV, _attr, new AST_TXML_Value( &_tV ) ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &msupV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );
		CNode *frame1st = retnode->GetChild( 1 );
		if( frame1st )
			frame1st->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
	}

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__conjugate( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( 
		new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &macrV ) ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &moverV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////
// A B S //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__abs( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &verbarV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &verbarV ) );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfencedV, _attr, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__floor( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &lfloorV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &rfloorV ) );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfencedV, _attr, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__ceiling( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &lceilV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &rceilV ) );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfencedV, _attr, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__mean( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &langleV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &rangleV ) );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfencedV, _attr, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__divide( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfracV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );
		to_read[ 1 ] = retnode->getFormulaLineChild( 1 );
	}

	delete root;
	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__quotient( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error
        if(_args.arity == -1 || OP_ARITY == _args.arity)
        {
            // to avoid warning
        }

	AST_TXML_Compound *frac__tags = new AST_TXML_Compound();
	frac__tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	frac__tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *frac_root = new AST_TXML_Tag( &mfracV, 0, frac__tags );

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &lfloorV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &rfloorV ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfencedV, _attr, frac_root );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		CFormulaNode *_fn = retnode->getFormulaDeepFormulaChild( 0 );
		if( _fn )
		{
			_fn->to_mathml_data.name = _T("");
			_fn->to_mathml_data.is_content = 
				CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			to_read[ 0 ] = _fn->getFormulaLineChild( 0 );
			to_read[ 1 ] = _fn->getFormulaLineChild( 1 );
		}
	}

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////


CFormulaNode *CLineNode::createEmptyForm__root( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	long i;
	AST_TXML_Tag *_n = 0;
	for( i = 1; i < _args.xml_tags->getChildNumber(); i++ )
	{
		_n = (AST_TXML_Tag*)(_args.xml_tags->getChild( i ));
		if( _n->getTagName() == FBL_TAG_degree )
			break;
	}

	CLineNode *_newnode = 0;
	CFormulaNode *retnode = 0;
	AST_TXML_Tag *root = 0;
	if( i == _args.xml_tags->getChildNumber() )
	{
		if( _args.l2do.size() > 0 )
		{
			AST_TXML_Compound *_tags = new AST_TXML_Compound();
			_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
			root = new AST_TXML_Tag( &msqrtV, 0, _tags );

			retnode = InsertMMLFormulaNode( root );
			if( retnode )
				to_read[ 0 ] = retnode->getFormulaLineChild( 0 );
		}
	}
	else if( i != 1 )
	{
		AST_TXML_Compound *_tags = new AST_TXML_Compound();
		_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
		_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
		root = new AST_TXML_Tag( &mrootV, 0, _tags );

		retnode = InsertMMLFormulaNode( root );
		if( retnode )
		{
			to_read[ 0 ] = retnode->getFormulaLineChild( 1 );
			to_read[ 1 ] = retnode->getFormulaLineChild( 0 );
		}

		_newnode = _args.l2do[ i ];
		_args.l2do[ i ] = 0;
		_args.l2do.insert( _args.l2do.begin() + 1, _newnode );
		for( i = 1; i < (long) _args.l2do.size(); )
		{
			if( _args.l2do[ i ] == 0 )
				_args.l2do.erase( _args.l2do.begin() + i );
			else
				i++;
		}
	}

	if( retnode == 0 )
	{
		AST_TXML_Compound *_tags = new AST_TXML_Compound();
		_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
		_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
		root = new AST_TXML_Tag( &mrootV, 0, _tags );

		retnode = InsertMMLFormulaNode( root );
		if( retnode )
		{
			to_read[ 0 ] = retnode->getFormulaLineChild( 1 );
			to_read[ 1 ] = retnode->getFormulaLineChild( 0 );
		}
	}

	if( root ) delete root;
	return retnode;
}


#if 0
CFormulaNode *CLineNode::createEmptyForm__root( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mrootV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = retnode->getFormulaLineChild( 1 );
		to_read[ 1 ] = retnode->getFormulaLineChild( 0 );
	}

	AST_TXML_Tag *_n = 0;
	for( long i = 1; i < _args.xml_tags->getChildNumber(); i++ )
	{
		_n = (AST_TXML_Tag*)(_args.xml_tags->getChild( i ));
		if( _n->getTagName() == FBL_TAG_degree )
			break;
	}
	CLineNode *_newnode = 0;
	if( i == _args.xml_tags->getChildNumber() )
	{
		if( _args.l2do.size() > 0 )
		{
			_newnode = new CLineNode();
			AST_TXML_Compound *_attr = new AST_TXML_Compound();
			_attr->Add( new AST_TXML_Attribute( &QString( _T("type") ), &QString( _T("integer") ) );
			AST_TXML_Tag *degree_cn = new AST_TXML_Tag( 
				&QString( FBL_TAG_cn ), _attr, 
				new AST_TXML_Value( &QString( _T("2") ) );
			AST_TXML_Tag *degree = new AST_TXML_Tag( 
				&QString( FBL_TAG_degree ), 0, degree_cn );
			_newnode->read_mathml( degree );
			_args.l2do.insert( _args.l2do.begin() + 1, _newnode );
			delete degree;
		}
	}
	else if( i != 1 )
	{
		_newnode = _args.l2do[ i ];
		_args.l2do[ i ] = 0;
		_args.l2do.insert( _args.l2do.begin() + 1, _newnode );
		for( i = 1; i < _args.l2do.size(); )
		{
			if( _args.l2do[ i ] == 0 )
				_args.l2do.erase( _args.l2do.begin() + i );
			else
				i++;
		}
	}

	delete root;
	return retnode;
}
#endif

CFormulaNode *CLineNode::createEmptyForm__log( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	int is_fenced = 0;
	if( _args.l2do.size() > 2 && _args.l2do[ 2 ] != 0 )
		is_fenced = ::_CM_make_fence_arg( 2, mmlPrecedence_Atom, _args );

	AST_TXML_Compound *_log_tags = new AST_TXML_Compound();
	_log_tags->Add( 
		new AST_TXML_Tag( &miV, 0, 
			new AST_TXML_Value( &logV ) ) );
	_log_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *_log_n = new AST_TXML_Tag( &msubV, 0, _log_tags );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( _log_n );
	//_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mrowV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = 0;
		CLineNode *_l = retnode->getFormulaLineChild( 0 );
		if( _l )
		{
			CNode *_n = _l->GetChild( 0 );
			if( _n && _n->GetType() == NODE_FORMULA )
			{
				_n->to_mathml_data.name = _T("");
				_n->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;

				CFormulaNode *fn = (CFormulaNode*)_n;
				if( fn->GetChild( 0 ) )
					fn->GetChild( 0 )->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
				to_read[ 0 ] = fn->getFormulaLineChild( 1 );
			}
		}
		to_read[ 1 ] = retnode->getFormulaLineChild( 0 );
	}

	long i;
	AST_TXML_Tag *_n = 0;
	for( i = 1; i < _args.xml_tags->getChildNumber(); i++ )
	{
		_n = (AST_TXML_Tag*)(_args.xml_tags->getChild( i ));
		if( _n->getTagName() == FBL_TAG_logbase )
			break;
	}
	CLineNode *_newnode = 0;
	if( i == _args.xml_tags->getChildNumber() )
	{
		if( _args.l2do.size() > 0 )
		{
			_newnode = new CLineNode();
			AST_TXML_Compound *_attr = new AST_TXML_Compound();
			_attr->Add( new AST_TXML_Attribute( &typeV, &integerV ) );
			AST_TXML_Tag *degree_cn = new AST_TXML_Tag( 
				&cnV, _attr, new AST_TXML_Value( &val10V ) );
			AST_TXML_Tag *degree = new AST_TXML_Tag( &logbaseV, 0, degree_cn );
			_newnode->read_mathml( degree );
			_args.l2do.insert( _args.l2do.begin() + 1, _newnode );
			delete degree;
		}
	}
	else if( i != 1 )
	{
		_newnode = _args.l2do[ i ];
		_args.l2do[ i ] = 0;
		_args.l2do.insert( _args.l2do.begin() + 1, _newnode );
		for( i = 1; i < (long) _args.l2do.size(); )
		{
			if( _args.l2do[ i ] == 0 )
				_args.l2do.erase( _args.l2do.begin() + i );
			else
				i++;
		}
	}

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////
// S U M //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#define _CM_LIMIT_FORM_NONE		0
#define _CM_LIMIT_FORM_LOW		1
#define _CM_LIMIT_FORM_UP		2
#define _CM_LIMIT_FORM_LOWUP	3
#define _CM_LIMIT_FORM_COND		4
#define _CM_LIMIT_FORM_DOMAIN	5
#define _CM_LIMIT_FORM_INTERVAL	6
int CLineNode::createEmptyForm__sum_parse( CCM_apply_args& _args )
{
	int ret_val = _CM_LIMIT_FORM_NONE;
	CLineNode *bvar = 0, *cond = 0, *ll = 0, *ul = 0, *dom = 0, *il = 0;
	long bvar_idx = -1, cond_idx = -1, ll_idx = -1, ul_idx = -1, dom_idx = -1, arg_idx = -1, il_idx = -1, cb = 0;
	int is_ll = 0, is_ul = 0, is_cond = 0, is_dom = 0, is_il = 0;
	std::vector<long> bvar_idx_ar;
	long i, k;
	int isForceRender = 1;
	AST_TXML_Tag *xmlTagNode = 0; 
	for( i = 1; i < _args.xml_tags->getChildNumber() && i < (long) _args.l2do.size(); i++ )
	{
		xmlTagNode = (AST_TXML_Tag*)(_args.xml_tags->getChild( i ));
		QString tn = xmlTagNode->getTagName();
		if( tn == FBL_TAG_condition )
		{
			cond = _args.l2do[ i ]; cond_idx = i; is_cond = cond != 0;
		}
		else if( tn == FBL_TAG_domainofapplication && _args.op_id != TAG2MML_limit )
		{
			dom = _args.l2do[ i ]; dom_idx = i; is_dom = dom != 0;
		}
		else if( tn == FBL_TAG_lowlimit )
		{
			ll = _args.l2do[ i ]; ll_idx = i; is_ll = ll != 0;
		}
		else if( tn == FBL_TAG_uplimit && _args.op_id != TAG2MML_limit )
		{
			ul = _args.l2do[ i ]; ul_idx = i; is_ul = ul != 0;
		}
		else if( tn == FBL_TAG_interval && _args.op_id != TAG2MML_limit )
		{
			il = _args.l2do[ i ]; il_idx = i; is_il = il != 0;
		}
		else if( tn == FBL_TAG_bvar )
		{
			bvar = _args.l2do[ i ];
			bvar_idx = i;
			bvar_idx_ar.push_back( i );
			if( !::isRenderMode_Force() && !xmlTagNode->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) )
				isForceRender = 0;
		}
		else if( _args.l2do[ i ] )
		{
			arg_idx = i;
			cb++;
		}
	}

	int is_fenced = 0;
	if( cb == 1 && arg_idx != -1 && _args.l2do[ arg_idx ] )
	{
		long argPrecedence = 
			( _args.l2do[ arg_idx ]->GetChildCount() == 1 && 
			  _args.l2do[ arg_idx ]->GetFirstChild() ) ?
                                (long) ::_CM_getPrecedence( _args.l2do[ arg_idx ]->GetFirstChild() ) :
                                (long) mmlPrecedence_Atom;
		if( argPrecedence > mmlPrecedence_apply )
			is_fenced = 1;
	}
	if( is_fenced )
	{
		CLineNode *newline = new CLineNode();
		CFormulaNode *_n = newline->InsertMMLFormulaNode( MMLContent_TT_mrow().getRoot() );
		if( _n )
		{
			_n->to_mathml_data.name = _T("");
			_n->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			CLineNode *_l = _n->getFormulaLineChild( 0 );
			if( _l )
			{
				_l->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("(") );
				_l->PumpLine2Line( _args.l2do[ arg_idx ] );
				_l->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T(")") );
			}
		}
		delete _args.l2do[ arg_idx ];
		_args.l2do[ arg_idx ] = newline;
	}

	if( is_ll && bvar_idx != -1 )
		ret_val = is_ul ? _CM_LIMIT_FORM_LOWUP : _CM_LIMIT_FORM_LOW;
	else if( is_ul )
		ret_val = _CM_LIMIT_FORM_UP;
	else if( is_cond )
		ret_val = _CM_LIMIT_FORM_COND;
	else if( is_dom )
		ret_val = _CM_LIMIT_FORM_DOMAIN;
	else if( is_il )
		ret_val = _CM_LIMIT_FORM_INTERVAL;

	AST_TXML_Tag *_tendsto = 0;
	if( _args.op_id == TAG2MML_limit )
		_tendsto = new AST_TXML_Tag( &tendstoV, 0, (AST_TXML_Tag*)0 );
	else
		_tendsto = new AST_TXML_Tag( &eqV, 0, (AST_TXML_Tag*)0 );
	std::vector<CLineNode*> newline;
	newline.push_back( _args.l2do[ 0 ] );
	_args.l2do[ 0 ] = 0;
	CLineNode *_l1 = 0, *_l2 = 0;
	int is_interval_treated = 0;
	int is_setBvarReference = (_args.op_id == TAG2MML_int && bvar_idx != -1) ? 1 : 0;
	switch( ret_val )
	{
	case _CM_LIMIT_FORM_LOW:
		if( _args.op_id == TAG2MML_int )
			_l1 = new CLineNode();
		else if( bvar_idx != -1 )
		{
			_l1 = new CLineNode( _args.l2do[ bvar_idx ] );
			_l1->read_mathml( _tendsto );
			_l1->setCMIgnore4Children();

			CFormulaNode* _fn = _l1->getContentFormulaChild( 0 );
			if( _fn )
			{
				_fn->setReferenced();
				_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx );
				is_setBvarReference = 1;
			}
		}
		if( _args.l2do[ ll_idx ] )
		{
			_l1->PumpLine2Line( _args.l2do[ ll_idx ] );		// lowlimit
			delete _args.l2do[ ll_idx ];
			_args.l2do[ ll_idx ] = 0;
		}
		newline.push_back( _l1 );
		break;
	case _CM_LIMIT_FORM_UP:
		_l2 = _args.l2do[ ul_idx ];
		_args.l2do[ ul_idx ] = 0;
		newline.push_back( _l2 );
		break;
	case _CM_LIMIT_FORM_LOWUP:
		if( _args.op_id == TAG2MML_int )
			_l1 = new CLineNode();
		else if( bvar_idx != -1 )
		{
			_l1 = new CLineNode( _args.l2do[ bvar_idx ] );
			_l1->read_mathml( _tendsto );
			_l1->setCMIgnore4Children();

			CFormulaNode* _fn = _l1->getContentFormulaChild( 0 );
			if( _fn )
			{
				_fn->setReferenced();
				_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx );
				is_setBvarReference = 1;
			}
		}
		if( _args.l2do[ ll_idx ] )
		{
			_l1->PumpLine2Line( _args.l2do[ ll_idx ] );		// lowlimit
			delete _args.l2do[ ll_idx ];
			_args.l2do[ ll_idx ] = 0;
		}
		_l2 = _args.l2do[ ul_idx ];
		_args.l2do[ ul_idx ] = 0;
		newline.push_back( _l1 );
		newline.push_back( _l2 );
		break;
	case _CM_LIMIT_FORM_DOMAIN:
	case _CM_LIMIT_FORM_COND:
		{
			long argsIdx = (ret_val == _CM_LIMIT_FORM_DOMAIN) ? dom_idx : cond_idx;
			if( bvar_idx_ar.size() && isForceRender )
			{
				CLineNode *lineNode = new CLineNode();
				for( k = 0; k < (long) bvar_idx_ar.size(); k++ )
				{
					if( k )
					{
						lineNode->InsertMML_CM_create_markup_rigid_token(
										FBL_TAG_mo,
										::getComaUsingCurrentLocale() );
					}
					CLineNode *_l = new CLineNode( _args.l2do[ bvar_idx_ar[ k ] ] );
					CFormulaNode* _fn = _l->getContentFormulaChild( 0 );
					if( _fn )
					{
						_fn->setReferenced();
						_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx_ar[ k ] );
						is_setBvarReference = 1;
					}
					_l->setCMIgnore4Children();
					lineNode->PumpLine2Line( _l );
					delete _l;
				}
				lineNode->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, QString("|") );
				lineNode->PumpLine2Line( _args.l2do[ argsIdx ] );
				delete _args.l2do[ argsIdx ];
				_args.l2do[ argsIdx ] = lineNode;
			}
			newline.push_back( _args.l2do[ argsIdx ] );
			_args.l2do[ argsIdx ] = 0;
		}
		break;
	case _CM_LIMIT_FORM_INTERVAL:
		{
			AST_TXML_Tag *_xml_tmp = (AST_TXML_Tag*)(_args.xml_tags->getChild( il_idx ));
			AST_TXML_Compound *_tags_il = _xml_tmp->getTags();
			if( _tags_il && bvar_idx != -1 )
			{
				for( long i = 0; i < _tags_il->getChildNumber(); )
				{
					if( _tags_il->getChild( i ) == 0 || 
						_tags_il->getChild( i )->getNodeType() != ASTT_TXML_Tag )
						_tags_il->Erase( i );
					else
						i++;
				}
				if( _tags_il->getChildNumber() == 2 && _args.l2do[ il_idx ] )
				{
					ret_val = _CM_LIMIT_FORM_LOWUP;
					is_interval_treated = 1;
		    
					if( _args.op_id == TAG2MML_int )
						_l1 = new CLineNode();
					else
					{
						_l1 = new CLineNode( _args.l2do[ bvar_idx ] );
						_l1->read_mathml( _tendsto );

						CFormulaNode* _fn = _l1->getContentFormulaChild( 0 );
						if( _fn )
						{
							_fn->setReferenced();
							_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx );
							is_setBvarReference = 1;
						}
					}

					_l2 = new CLineNode();

					CLineNode *srcLine = 0;
					CFormulaNode *frmNode = _args.l2do[ il_idx ]->getContentFormulaChild( 0 );
					if( frmNode && (srcLine = frmNode->getFormulaLineChild(0, 0)) != 0 )
					{
						int i = 0, cb = 0;
						CNode *pCurNode = srcLine->GetFirstChild();
						while( pCurNode && cb < 2 )
						{
							if( !pCurNode->isIgnoreContentConversion() )
							{
								QVector<CNode*> children;
								srcLine->RemoveOrCopyChildren( children, i, i + 1, 0 );
								(cb ? _l2 : _l1)->InsertChildren( children, -1 );
								cb++;
							}
							pCurNode = pCurNode->GetNext();
							i++;
						}
						
						_l1->setCMIgnore4Children();
						_l2->setCMIgnore4Children();

						CFormulaNode* _fn = 0;
						_fn = _l1->getContentFormulaChild( ( _args.op_id == TAG2MML_int ) ? 0 : 2 );
						if( _fn )
						{
							_fn->setReferenced();
							_fn->setIgnoreContentConversion();
							_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_interval( 0 );
						}
						_fn = _l2->getContentFormulaChild( 0 );
						if( _fn )
						{
							_fn->setReferenced();
							_fn->setIgnoreContentConversion();
							_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_interval( 1 );
						}
					}

					newline.push_back( _l1 );
					newline.push_back( _l2 );
				}
			}
		    
			if( !is_interval_treated )
			{
				ret_val = _CM_LIMIT_FORM_COND;
				newline.push_back( _args.l2do[ il_idx ] );
				_args.l2do[ il_idx ] = 0;
			}
		}
		break;
	default:
		break;
	}

	if( newline[ 0 ] )
	{
		for( i = 0; i < (long) bvar_idx_ar.size(); i++ )
		{
			CLineNode *bvarLine = _args.l2do[ bvar_idx_ar[ i ] ];
			if( bvarLine )
			{
				if( /*bvar_idx == bvar_idx_ar[ i ] && */is_setBvarReference )
				{
					CFormulaNode* _fn = bvarLine->getContentFormulaChild( 0 );
					if( _fn )
					{
						_fn->setContentReference();
						_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx_ar[ i ] );
					}
				}
				newline[ 0 ]->PumpLine2Line( bvarLine );
				delete _args.l2do[ bvar_idx_ar[ i ] ];
				_args.l2do[ bvar_idx_ar[ i ] ] = 0;
			}
		}
	    
		if( is_interval_treated && _args.l2do[ il_idx ] )
		{
			CFormulaNode* _fn = _args.l2do[ il_idx ]->getContentFormulaChild( 0 );
			if( _fn )
			{
				_fn->setContentReference();
				_fn->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__INTERVAL_REFERENCE;
			}
			newline[ 0 ]->PumpLine2Line( _args.l2do[ il_idx ] );

			delete _args.l2do[ il_idx ];
			_args.l2do[ il_idx ] = 0;
		}
	}

	for( i = 0; i < (long) _args.l2do.size(); i++ )
		if( _args.l2do[ i ] )
			newline.push_back( _args.l2do[ i ] );
	_args.l2do.erase(_args.l2do.begin(), _args.l2do.end());
	for( i = 0; i < (long) newline.size(); i++ )
		_args.l2do.push_back( newline[ i ] );

	return ret_val;
}

CFormulaNode *CLineNode::createEmptyForm__sum( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args, QString optext, QString optag, int isLargeOpAttr )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	int form = createEmptyForm__sum_parse( _args );
	QString _underover = _T("");

	AST_TXML_Compound *_attr = 0;
	if( isLargeOpAttr )
	{
		_attr = new AST_TXML_Compound();
		_attr->Add( new AST_TXML_Attribute( &largeopV, &trueV ) );
	}
	QString valV( optext );
	AST_TXML_Tag *_n = new AST_TXML_Tag( &optag, _attr, new AST_TXML_Value( &valV ) );

	int actual_args = 1;
	switch( form )
	{
	case _CM_LIMIT_FORM_DOMAIN:
	case _CM_LIMIT_FORM_COND:
	case _CM_LIMIT_FORM_LOW:
		_underover = FBL_TAG_munder;
		actual_args = 2;
		break;
	case _CM_LIMIT_FORM_UP:
		_underover = FBL_TAG_mover;
		actual_args = 2;
		break;
	case _CM_LIMIT_FORM_LOWUP:
		_underover = FBL_TAG_munderover;
		actual_args = 3;
		break;
	default:
		break;
	}

	AST_TXML_Tag *_sum_n = 0;
	if( _underover.length() )
	{
		AST_TXML_Compound *_sum_tags = new AST_TXML_Compound();
		_sum_tags->Add( _n );
		_sum_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
		if( form == _CM_LIMIT_FORM_LOWUP )
			_sum_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
		QString underoverV( _underover );
		_sum_n = new AST_TXML_Tag( &underoverV, 0, _sum_tags );
	}
	else
		_sum_n = new AST_TXML_Tag( &mrowV, 0, _n );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( _sum_n );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mrowV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		to_read[ 0 ] = 0;
		CLineNode *_l = retnode->getFormulaLineChild( 0 );
		if( _l )
		{
			if( actual_args > 1 )
			{
				CNode *_n = _l->GetChild( 0 );
				if( _n && _n->GetType() == NODE_FORMULA )
				{
					_n->to_mathml_data.name = _T("");
					_n->to_mathml_data.is_content = 
						CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
					CFormulaNode *fn = (CFormulaNode*)_n;
					if( fn->GetChild( 0 ) )
						fn->GetChild( 0 )->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
					to_read[ 0 ] = fn->getFormulaLineChild( 1 );
					if( actual_args > 2 )
						to_read.push_back( fn->getFormulaLineChild( 2 ) );
				}
				to_read.push_back( _l );
			}
			else
			{
				if( _l->GetChild( 0 ) )
				{
					_l->GetChild( 0 )->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
				}
				to_read[ 0 ] = _l;
			}
		}
	}

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__maxmin( CCM_apply_args& _args )
{
	AST_TXML_Compound *_set_tags = new AST_TXML_Compound();
	for( long i = 1; i < _args.xml_tags->getChildNumber(); i++ )
		_set_tags->Add( _args.xml_tags->getChild( i ) );
	AST_TXML_Tag *_set = new AST_TXML_Tag( &setV, 0, _set_tags );

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( _args.xml_tags->getChild( 0 ) );
	_tags->Add( _set );

	MMLContent_TT_mrow _row;
	_row.set( _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( _row.getRoot() );

	_row.reset();
	_tags->detachChildren();
	_set->getTags()->detachChildren();
	delete _set;

	if( retnode )
	{
		if( _args.l2do[ 0 ] )
		{
			delete _args.l2do[ 0 ];
			_args.l2do[ 0 ] = 0;
		}
		CFormulaNode *fn = retnode->getFormulaDeepFormulaChild( 0, 1 );
		if( fn /*&& 0*/ )
		{
			fn->to_mathml_data.name = _T("");
			/*fn->to_mathml_data.is_content =
				CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY_ADDON_OP;*/
		}
/*		CNode *frame1st = retnode->GetChild( 1 );
		if( frame1st )
		{
			frame1st->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
			frame1st->to_mathml_data.name = _T("");
			frame1st->to_mathml_data.is_content =
				CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		}*/
	}

	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__moment( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args, int& moment_about )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	//=== parse <momentabout> and <degree> items
	moment_about = 0;
	long i, d_idx = -1, m_idx = -1, n_idx = -1;
	CLineNode *degree_node = 0;
	std::vector<CLineNode*> newline;
	AST_TXML_Tag *xmlTagNode = 0;
	for( i = 1; i < _args.xml_tags->getChildNumber(); i++ )
	{
		if( _args.l2do[ i ] == 0 ) continue;
		xmlTagNode = (AST_TXML_Tag*)(_args.xml_tags->getChild( i ));
		if( xmlTagNode->getTagName() == FBL_TAG_momentabout )
		{
			if( moment_about == 0 &&
				_args.l2do[ 0 ] && _args.l2do[ 0 ]->GetType() == NODE_LINE &&
				((CLineNode*)(_args.l2do[ 0 ]))->GetChildCount() > 0 )
			{
				if( !::isRenderMode_Force() && !xmlTagNode->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) )
				{
					if( _args.l2do[ 0 ] ) _args.l2do[ 0 ]->PumpLine2Line( _args.l2do[ i ] );
					delete _args.l2do[ i ];
					_args.l2do[ i ] = 0;
				}
				else
				{
					m_idx = i;
				}
				moment_about = 1;
			}
		}
		else if( ((AST_TXML_Tag*)(_args.xml_tags->getChild( i )))->getTagName() == FBL_TAG_degree )
		{
			degree_node = _args.l2do[ i ];
			_args.l2do[ i ] = 0;
			d_idx = i;
		}
		else if( n_idx == -1 && _args.l2do[ i ] )
			n_idx = i;
	}

	// if( d_idx == -1 ) // error: degree is mandatory in <moment>
	newline.push_back( _args.l2do[ 0 ] );
	_args.l2do[ 0 ] = 0;

	if( n_idx != -1 && m_idx != -1 )
	{
		CLineNode *lineNode = new CLineNode( _args.l2do[ m_idx ] );
		delete _args.l2do[ m_idx ];
		_args.l2do[ m_idx ] = 0;
/*		CFormulaNode* _fn = lineNode->getContentFormulaChild( 0 );
		if( _fn )
		{
			_fn->setReferenced();
			_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_momentabout();
		}
		lineNode->setCMIgnore4Children();*/
		lineNode->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, QString( _T("|") ) );
		lineNode->PumpLine2Line( _args.l2do[ n_idx ] );
		delete _args.l2do[ n_idx ];
		_args.l2do[ n_idx ] = lineNode;
	}

	if( !degree_node )
	{
		degree_node = new CLineNode();
		degree_node->InsertMMLFormulaNode( MMLContent_TT_mrow().getRoot() );
		CFormulaNode *_slot = degree_node->getFormulaChild( 0 );
		if( _slot )
		{
			_slot->to_mathml_data.reserved = _slot->to_mathml_data.name;
			_slot->to_mathml_data.name = FBL_TAG_degree;
			_slot->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		}
	}

	newline.push_back( degree_node );
	for( i = 0; i < (long) _args.l2do.size(); i++ )
		if( _args.l2do[ i ] )
			newline.push_back( _args.l2do[ i ] );

	_args.l2do.erase(_args.l2do.begin(), _args.l2do.end());
	for( i = 0; i < (long) newline.size(); i++ )
		_args.l2do.push_back( newline[ i ] );

	//=== create form
	AST_TXML_Compound *frac__tags = new AST_TXML_Compound();
	frac__tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	frac__tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *frac_root = new AST_TXML_Tag( &msupV, 0, frac__tags );

	AST_TXML_Compound *_attr = new AST_TXML_Compound();
	_attr->Add( new AST_TXML_Attribute( &openV, &langleV ) );
	_attr->Add( new AST_TXML_Attribute( &closeV, &rangleV ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfencedV, _attr, frac_root );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		CFormulaNode *_fn = retnode->getFormulaDeepFormulaChild( 0 );
		if( _fn )
		{
			_fn->to_mathml_data.name = _T("");
			_fn->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			to_read[ 0 ] = _fn->getFormulaLineChild( 1 );
			to_read[ 1 ] = _fn->getFormulaLineChild( 0 );
		}
	}

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__selector( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &msubV, 0, _tags );

	int is_fenced = 0;
	CFormulaNode *retnode = 0;
	if( _args.l2do[ 1 ] && 
		::_CM_getPrecedence( _args.l2do[ 1 ] ) > mmlPrecedence_Atom )
	{
		root = new AST_TXML_Tag( &mfencedV, 0, root );
		is_fenced = 1;
	}

	retnode = InsertMMLFormulaNode( root );

	if( retnode )
	{
		CFormulaNode *_fn = 
			is_fenced ? retnode->getFormulaDeepFormulaChild( 0 ) : retnode;
		if( _fn )
		{
			to_read[ 0 ] = _fn->getFormulaLineChild( 0 );
			to_read[ 1 ] = _fn->getFormulaLineChild( 1 );
		}
	}

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

int CLineNode::createEmptyForm__forall_exists_parse( CCM_apply_args& _args )
{
	long i;
	int is_app = 0;
	CLineNode *condLine = new CLineNode(), *bvarLine = new CLineNode(), *appLine = new CLineNode();
	AST_TXML_Tag *tagNode = 0;
	int isForceRender = 1;
	for( i = 1; i < _args.xml_tags->getChildNumber() && i < (long) _args.l2do.size(); i++ )
	{
		tagNode = (AST_TXML_Tag*)(_args.xml_tags->getChild( i ));
		QString tn = tagNode->getTagName();
		if( tn == FBL_TAG_condition )
		{
			condLine->PumpLine2Line( _args.l2do[ i ] );
		}
		else if( tn == FBL_TAG_bvar )
		{
			if( bvarLine->GetChildCount() > 0 )
			{
				bvarLine->InsertMML_CM_create_markup_rigid_token(
							FBL_TAG_mo,
							::getComaUsingCurrentLocale() );
			}
			bvarLine->PumpLine2Line( _args.l2do[ i ] );
			if( isForceRender && !::isRenderMode_Force() && !tagNode->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) )
				isForceRender = 0;
		}
		else
		{
			if( (tn == FBL_TAG_apply || tn == FBL_TAG_reln) && _args.l2do[ i ] != 0 ) is_app = 1;
			appLine->PumpLine2Line( _args.l2do[ i ] );
		}
	}

	if( bvarLine->GetChildCount() == 0 )
	{
		isForceRender = 0;
		AST_TXML_Tag *_bvar_root = new AST_TXML_Tag( &bvarV, 0, (AST_TXML_Tag*)0 );
		bvarLine->read_mathml( _bvar_root );
		delete _bvar_root;
	}
	if( condLine->GetChildCount() > 0 )
	{
		if( isForceRender )
		{
			bvarLine->InsertMML_CM_create_markup_rigid_token(
						FBL_TAG_mo,
						::getComaUsingCurrentLocale() );
		}
		else
		{
			if( _args.l2do[ 0 ] ) _args.l2do[ 0 ]->PumpLine2Line( bvarLine );
		}
		bvarLine->PumpLine2Line( condLine );
		delete condLine;
	}

	for( i = 1; i < (long) _args.l2do.size(); i++ )
	{
		if( _args.l2do[ i ] )
			delete _args.l2do[ i ];
	}
	_args.l2do.resize( 1 );
	_args.l2do.push_back( bvarLine );
	if( appLine->GetChildCount() > 0 ) _args.l2do.push_back( appLine );

	return _CM_LIMIT_FORM_NONE;
}

CFormulaNode *CLineNode::createEmptyForm__forall_exists( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args, QString optext )
{
	const int OP_ARITY = 2;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	createEmptyForm__forall_exists_parse( _args );

	QString sV(":");
	QString s2V( optext );
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &s2V ) ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0, OLIMP_MATHML_FLAG__MROW_KEEP1CHILD ) );
	_tags->Add( new AST_TXML_Tag( &moV, 0, new AST_TXML_Value( &sV ) ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0, OLIMP_MATHML_FLAG__MROW_KEEP1CHILD ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mrowV, 0, _tags );

	CFormulaNode *retnode = InsertMMLFormulaNode( root );
	if( retnode )
	{
		CLineNode *_l = retnode->getFormulaLineChild( 0 );
		if( _l )
		{
			if( _l->GetChild( 0 ) )
				_l->GetChild( 0 )->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
			if( _l->GetChild( 2 ) )
				_l->GetChild( 2 )->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		}

		CFormulaNode *_fn = 0;
		_fn = retnode->getFormulaDeepFormulaChild( 0, 1 );
		if( _fn )
		{
			_fn->to_mathml_data.name = _T("");
			_fn->to_mathml_data.is_content =
				CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			to_read[ 0 ] = _fn->getFormulaLineChild( 0 );
		}
		_fn = retnode->getFormulaDeepFormulaChild( 0, 3 );
		if( _fn )
		{
			_fn->to_mathml_data.name = _T("");
			_fn->to_mathml_data.is_content =
				CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			to_read[ 1 ] = _fn->getFormulaLineChild( 0 );
		}
	}

	delete root;
	return retnode;
}

///////////////////////////////////////////////////////////////////////////////
