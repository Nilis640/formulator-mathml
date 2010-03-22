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
#include "../../dictionary/op_autokeyword.h"
#include "../../dictionary/op_map.h"
#include "n_cm_treetemplate.h"
#include "n_rmml_cm.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_apply( 
	enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	CCM_apply_args _args;
	int ret = _args.set( tag_id, xml_node );
	if( ret == -1 )
		return NULL;
	else if( ret == 1 )
		return InsertMML_CM_standalone_transparent( tag_id, xml_node );

	int isSpecialRender = 0;
	if( _args.form == __APPLY_RENDER_SPECIAL )
		isSpecialRender = 1;

	CFormulaNode *_node_apply = isSpecialRender ? InsertMML_CM_apply__special( _args ) : InsertMML_CM_apply__list( _args );
	if( _node_apply )
	{
		_CM_basic_Node_property( _node_apply, xml_node );
		_node_apply->to_mathml_data.precedence = (short) ::mml_getPrecedence( _args.op_id, xml_node->getTags()->getChildNumber() - 1, _args.op_body );
		_node_apply->to_mathml_data.assoc = ::mml_getAssoc( _args.op_id, -1, _args.op_body );
		if( _args.form != __APPLY_RENDER_SPECIAL )
			_node_apply->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_APPLY;

		if( _args.form == __APPLY_RENDER_FORM_NONFENCED_FUN )
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
	}

	return _node_apply;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_apply__list( CCM_apply_args& _args )
{
	MMLContent_TT_mrow setitem2insert;
	CFormulaNode *_node = InsertMMLFormulaNode( setitem2insert.getRoot() );
	if( !_node ) return _node;
	CLineNode *_line1 = _node->getFormulaLineChild( 0 );
	if( !_line1 ) return _node;

	CNode *childNode = _args.l2do[ 0 ]->GetChild( 0 );
	bool bNeedWrap = childNode ? (::_CM_getPrecedence( childNode ) > mmlPrecedence_Atom) : false;
	if( bNeedWrap && childNode && childNode->GetType() == NODE_FORMULA && 
		childNode->to_mathml_data.is_content && ::mml_is_cm_apply( childNode->to_mathml_data.name ) )
	{
		if( !shouldBracket4Apply( ((CFormulaNode*) childNode)->getCalcButtonID(), -1 ) )
			bNeedWrap = false;
	}

	if( _args.form == __APPLY_RENDER_FORM_OP || !_args.l2do[ 0 ] || 
		_args.l2do[ 0 ]->GetChild( 0 ) == 0 )
		_line1->InsertMML_CM_apply__read_op( _args._h_node, _args._h_str, 0, _args.form );
	else if( bNeedWrap )
	{
		CLineNode *headLine = 0;
		if( _args.l2do[ 0 ] )
		{
			CFormulaNode *_f = _line1->InsertMMLFormulaNode( setitem2insert.getRoot() );
			Q_ASSERT( _f );
			if( _f )
			{
				_f->setAutodetectContent();
				_f->to_mathml_data.name = _T("");
				headLine = _f->getFormulaLineChild( 0 );
			}
		}
		if( headLine == 0 ) headLine = _line1;

		headLine->InsertMML_CM_apply__read_op( _args.l2do[ 0 ], _args._h_str, 0, _args.form );
		if( headLine->GetChildCount() == 1 && headLine->GetFirstChild() && headLine->GetFirstChild()->GetType() == NODE_FORMULA )
		{
			CFormulaNode *bracketsFormulaNode = (CFormulaNode*) headLine->GetFirstChild();
			CLineNode *bracketed_line = bracketsFormulaNode ? bracketsFormulaNode->CM_WrapIntoBracketsOnPrecedence( mmlPrecedence_Atom, 0, bNeedWrap, -1 ) : 0;
			if( bracketed_line )
				headLine->InsertMML_CM_remove_outof_brackets( bracketed_line );
		}
	}
	else
		_line1->InsertMML_CM_apply__read_op( _args._h_node, _args._h_str, 0, _args.form );

	CLineNode *_line2 = 0;
	if( _args.form == __APPLY_RENDER_FORM_FENCED_FUN )
	{
		CFormulaNode *_node2 = _line1->InsertMMLFormulaNode( setitem2insert.getRoot() );
		if( !_node2 ) return _node;
		_node2->to_mathml_data.name = _T("");
		_node2->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		_node2->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		_node2->clearIgnoreContentConversion();
		_line1->setReadOnly();
		_line2 = _node2->getFormulaLineChild( 0 );
		if( !_line2 ) return _node;	// internal error
	}
	else
		_line2 = _line1;

	setitem2insert.reset();
	
	_line2->InsertMML_CM_apply__read_op( _args._l_node, _args._l_str, 0, _args.form );

	long k;
	for( k = 1; k < (long) _args.l2do.size(); k++ )
	{
		if( _args.op_id != TAG2MML_factorial || _args.form != __APPLY_RENDER_FORM_OP )
		{
			if( k - 1 || (_args.arity == 1 && _args.form == __APPLY_RENDER_FORM_OP) )
			{
				if( _args.arity < 0 || k - 1 < _args.arity )
				{
					int readonly = (_args.form == __APPLY_RENDER_FORM_OP && k > 2 );
					_line2->InsertMML_CM_apply__read_op( _args._c_node, _args._c_str, readonly, _args.form );
				}
			}
		}
		InsertMML_CM_apply__read_arg( _line2, k, _args );
	}

	if( _args.arity >= 0 && (long) (_args.l2do.size() - 1) < _args.arity )
	{
		CFormulaNode *_row = 0;
		for( ; k < _args.arity; k++ )
		{
			if( k - 1 )
			{
				int readonly = (_args.form == __APPLY_RENDER_FORM_OP && k > 2 );
				_line2->InsertMML_CM_apply__read_op( _args._c_node, _args._c_str, readonly, _args.form );
			}
			_row = _line2->InsertMMLFormulaNode( setitem2insert.getRoot() );
			if( _row )
			{
				_row->to_mathml_data.name = _T("");
				_row->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			}
		}
	}

	if( _args.op_id == TAG2MML_factorial && _args.form == __APPLY_RENDER_FORM_OP )
		_line2->InsertMML_CM_apply__read_op( _args._c_node, _args._c_str, 0, _args.form );

	_line2->InsertMML_CM_apply__read_op( _args._r_node, _args._r_str, 0, _args.form );

	return _node;
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::InsertMML_CM_apply__read_arg( 
	CLineNode *ln, long k, CCM_apply_args& _args )
{
	CLineNode *src = _args.l2do[ k ];
	if( !src ) return;
	CLineNode *line2insert = 0;
	bool shouldInsertAutodetect = false;
	if( _args.form == __APPLY_RENDER_FORM_OP )
	{
		long argPrecedence = mmlPrecedence_Atom;
		short argAssoc = 0;
		if( src->GetChildCount() == 1 && src->GetFirstChild() )
		{
			argPrecedence = ::_CM_getPrecedence( src->GetFirstChild() );
			argAssoc = ::_CM_getAssoc( src->GetFirstChild() );
			if( src->GetFirstChild()->GetType() == NODE_FORMULA && argPrecedence == mmlPrecedence_apply )
			{
				CFormulaNode *apply_n = (CFormulaNode*)(src->GetFirstChild());
				if( apply_n->to_mathml_data.name == FBL_TAG_apply )
				{
					if( _args.op_id == TAG2MML_factorial )
					{
						AST_TXML_Compound *_tags = ((AST_TXML_Tag*)(_args.xml_tags->getChild( k )))->getTags();
						if( _tags && _tags->getChild( 0 ) && 
							_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
						{
							AST_TXML_Tag *_op = (AST_TXML_Tag*)(_tags->getChild( 0 ));
							if( _op->getTagName() == FBL_TAG_divide )
							{
								argPrecedence = mmlPrecedence_mult;
								argAssoc = ::mml_getAssoc( TAG2MML_divide );
							}
						}
					}
					else if( apply_n->getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP ||
								apply_n->getCalcButtonID() == HF_BTNID_SUP )
					{
						shouldInsertAutodetect = true;
					}
				}
			}
		}

		if( (argPrecedence > mmlPrecedence_apply) || shouldInsertAutodetect )
		{
			MMLContent_TT_mrow emptySlot;
			emptySlot.setAutodetect();
			CFormulaNode *emptyNode = ln->InsertMMLFormulaNode( emptySlot.getRoot() );
			CLineNode *emptyLine = 0;
			if( emptyNode && (emptyLine = emptyNode->getFormulaLineChild( 0 )) != 0 )
			{
				emptyNode->setReadOnly();
				ln = emptyLine;
			}
			emptySlot.reset();
		}

		short position = (k == 1) ? (_args.arity == 1 ? 1 : -1) : ((k > 1) ? 1 : 0);
		bool shouldWrapByPrecAssoc = ::mml_shouldWrapByPrecAssoc( _args.nPrecedence, argPrecedence, _args.nAssoc, argAssoc, position );

		if( shouldWrapByPrecAssoc/*argPrecedence > _args.nPrecedence*/ )
		{
			MMLContent_TT_mfenced fence;
			CFormulaNode *_f = ln->InsertMMLFormulaNode_OTHERS_byID( HF_BTNID_DEFAULT_BRACKET_LR, fence.getRoot() );
			if( _f )
			{
				_f->to_mathml_data.name = _T("");
				_f->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				if( _f->GetFirstChild() )
					_f->GetFirstChild()->iterateNodeTree( setIgnoreContent, 0 );
				line2insert = _f->getFormulaLineChild( 0 );
			}
		}

	}
	if( line2insert )
	{
		line2insert->PumpLine2Line( src );
		line2insert->setReadOnly();
		if( line2insert->GetFirstChild() )
			line2insert->GetFirstChild()->setReadOnly();
	}
	else
		ln->PumpLine2Line( src );
}

void CLineNode::InsertMML_CM_apply__read_op( 
	AST_TXML_Tag *_item_node, QString _item_str, int readonly, long form )
{
	if( _item_node )
	{
		CLineNode *lbuf = new CLineNode();
		lbuf->read_mathml( _item_node );
		if( readonly )
			lbuf->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		else if( form == __APPLY_RENDER_FORM_OP )
		{
			CNode *pNode = lbuf->GetFirstChild();
			while( pNode )
			{
				pNode->to_mathml_data.is_content = 
					CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE;
				pNode = pNode->GetNext();
			}
		}
		PumpLine2Line( lbuf );
		delete lbuf;
	}
	else if( _item_str.length() )
		InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _item_str );
}

void CLineNode::InsertMML_CM_apply__read_op( 
	CLineNode *lbuf, QString _item_str, int readonly, long form )
{
	if( lbuf )
	{
		if( readonly )
			lbuf->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		else if( form == __APPLY_RENDER_FORM_OP )
		{
			CNode *pNode = lbuf->GetFirstChild();
			while( pNode )
			{
				pNode->to_mathml_data.is_content = 
					CONTENT_MATHML_CONVERTION2__APPLY_OPERATOR_RECURSIVE_TREE;
				pNode = pNode->GetNext();
			}
		}
		PumpLine2Line( lbuf );
	}
	else if( _item_str.length() )
		InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _item_str );
}

///////////////////////////////////////////////////////////////////////////////

CCM_apply_args::CCM_apply_args( void )
{
	_h_str = _l_str = _r_str = _c_str = _T("");
	_h_node = _l_node = _r_node = _c_node = NULL;	
	xml_n = 0;
	xml_tags = 0;
}

int CCM_apply_args::set( enum TAG2MML /*tag_id*/, AST_TXML_Tag*& xml_node )
{
	if( !xml_node ) return -1;
	if( !xml_node->getTags() || !xml_node->isExistTagChild() )
		return -1;

	long i;
	xml_n = xml_node;
	AST_TXML_Compound *_tags = xml_node->getTags();
	for( i = 0; i < _tags->getChildNumber(); )
	{
		if( _tags->getChild( i ) == 0 || 
			_tags->getChild( i )->getNodeType() != ASTT_TXML_Tag )
			_tags->Erase( i );
		else
			i++;
	}
    
	if( _tags->getChildNumber() == 0 || ::_CM_isDeclareChild( _tags, 0 ) )
		return -1;
    
	int wasUniversalQualifier = 0, wasBVar = 0;
	for( i = 1; i < _tags->getChildNumber(); )
	{
		if( ::_CM_isUniversalQualifierChild( _tags, i ) )
			wasUniversalQualifier = 1;
		if( ::_CM_isBVarChild( _tags, i ) )
			wasBVar = 1;
		if( ::_CM_isDeclareChild( _tags, i ) )
			_tags->Erase( i );
		else
			i++;
	}
    
	if( _tags->getChildNumber() < 2 ) return 1;

	op_id = ::mml_getTag2MML( ((AST_TXML_Tag*) _tags->getChild( 0 ))->getTagName() );
	op_body = ((AST_TXML_Tag*) _tags->getChild( 0 ))->getText();
	nSize = _tags->getChildNumber();
	arity = ::mml_getCM2Argc( op_id );
	nPrecedence = ::mml_getPrecedence( op_id, nSize - 1, op_body );
	nAssoc = ::mml_getAssoc( op_id, -1, op_body );
	if( op_id == TAG2MML_minus )
		arity = nSize == 2 ? 1 : 2;
	else if( (op_id == TAG2MML_plus || op_id == TAG2MML_nop) && nSize == 2 )
		arity = 1;

	if( op_id == TAG2MML_max || op_id == TAG2MML_min )
	{    
		l2do.resize( 1, NULL );
		l2do[ 0 ] = new CLineNode();
		l2do[ 0 ]->read_mathml( (AST_TXML_Tag*)(_tags->getChild( 0 )) );
	}
	else
	{    
		l2do.resize( nSize, NULL );
		l2do[ 0 ] = new CLineNode();
		l2do[ 0 ]->setHidden();
		l2do[ 0 ]->read_mathml( (AST_TXML_Tag*)(_tags->getChild( 0 )) );
		l2do[ 0 ]->clearHidden();
		for( i = 1; i < nSize; i++ )
		{
			l2do[ i ] = new CLineNode();
			l2do[ i ]->read_mathml( (AST_TXML_Tag*)(_tags->getChild( i )) );
		}
	}

	form = ::mml_getApplyForm( op_id );

	if( op_id == TAG2MML_csymbol )
	{
		AST_TXML_Tag *opinfo_node = (AST_TXML_Tag*)(_tags->getChild( 0 ));
		HMathOpAttr *opinfo = opinfo_node ? ::getOperatorEntryByMML( opinfo_node->getText() ) : 0;
		if( opinfo && opinfo->form == HMathOpAttr_FORM_INFIX )
		{
			form = __APPLY_RENDER_FORM_OP;
			nPrecedence = opinfo->precedence;
			nAssoc = 0;
			if( nSize == 2 )
				arity = 1;
		}
	}

	if( wasUniversalQualifier && wasBVar && ::mml_isNAryOperator( op_id ) )
		form = __APPLY_RENDER_SPECIAL;

	switch( form )
	{
	case __APPLY_RENDER_FORM_FENCED_FUN:
		_h_node = (AST_TXML_Tag*)(_tags->getChild( 0 )); // l2do[ 0 ];
		_l_str = _T("(");
		_r_str = _T(")");
		_c_str = ::getComaUsingCurrentLocale();
		break;
	case __APPLY_RENDER_FORM_NONFENCED_FUN:
		Q_ASSERT( ::mml_getCM2Argc( op_id ) == 1 );
		_h_node = (AST_TXML_Tag*)(_tags->getChild( 0 )); // l2do[ 0 ];
		_c_str = ::getComaUsingCurrentLocale();	// _T(",");
		if( (l2do[ 1 ] && l2do[ 1 ]->GetChildCount() > 0 && l2do[ 1 ]->GetChild( 0 ) != 0) &&
			::_CM_getPrecedence( l2do[ 1 ]->GetChild( 0 ) ) > mmlPrecedence_Atom )
		{
			QString buttonID = _T("");
			if( l2do[ 1 ]->GetChild( 0 )->GetType() == NODE_FORMULA )
				buttonID = ((CFormulaNode*) l2do[ 1 ]->GetChild( 0 ))->getCalcButtonID();
			if( shouldBracket4Apply( buttonID, -1 /*don't need child position, only button matters*/ ) )
			{
				_l_str = _T("(");
				_r_str = _T(")");
			}
		}
		break;
	case __APPLY_RENDER_FORM_OP:
		_c_node = (AST_TXML_Tag*)(_tags->getChild( 0 )); // l2do[ 0 ];
		break;
	case __APPLY_RENDER_SPECIAL:
		break;
	default:
		break;
	}

	xml_tags = xml_node->getTags();

	return 0;
}

CCM_apply_args::~CCM_apply_args()
{
	for( unsigned long i = 0; i < l2do.size(); i++ )
		if( l2do[ i ] ) delete l2do[ i ];
}

///////////////////////////////////////////////////////////////////////////////

