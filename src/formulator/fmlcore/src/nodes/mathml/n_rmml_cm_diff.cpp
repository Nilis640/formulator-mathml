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

#include "HUtils/idfas.h"
#include "n_cm_treetemplate.h"
#include "n_rmml_cm.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

void _CM_fill_diff_bvar( long& bvar_idx, long& degree_idx, long& list_idx, 
	long& arg_idx, long& cb, std::vector<long>& bvar_idx_ar, CCM_apply_args& _args );
int _CM_make_fence_arg( long arg_idx, long precedence, CCM_apply_args& _args );
void _CM_clean_bvar( std::vector<long>& bvar_idx_ar, CCM_apply_args& _args );
void _CM_build_degree( AST_TXML_Tag *&_degree_node, AST_TXML_Tag *&_degree_aspect,
	AST_TXML_Tag *&_degree_addon, long degree_idx, std::vector<long>& bvar_idx_ar, CCM_apply_args& _args );

///////////////////////////////////////////////////////////////////////////////
// C A L C U L U S ////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::createEmptyForm__calculus( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	long i;
	CLineNode *addon = new CLineNode();
	for( i = 1; i < _args.xml_tags->getChildNumber() && 
					 i < (long) _args.l2do.size(); i++ )
	{
		QString tn = ((AST_TXML_Tag*)(_args.xml_tags->getChild( i )))->getTagName();
		if( tn == FBL_TAG_bvar && _args.l2do[ i ] )
		{
			addon->PumpLine2Line( _args.l2do[ i ] );
			delete _args.l2do[ i ];
			_args.l2do[ i ] = 0;
		}
	}
	if( _args.l2do[ 0 ] && _args.l2do[ 0 ]->GetFirstChild() && 
		_args.l2do[ 0 ]->GetFirstChild()->to_mathml_data.is_content == 
			CONTENT_MATHML_CONVERTION2__SELF_CLOSING )
	{
		_args.l2do[ 0 ]->GetFirstChild()->setAddonNode( addon );
	}
	else
		delete addon;
	addon = 0;

	for( i = 1; i < (long) _args.l2do.size(); )
	{
		if( _args.l2do[ i ] == 0 )
			_args.l2do.erase( _args.l2do.begin() + i );
		else
			i++;
	}

	int is_fenced = 0;
	if( !_args.l2do[ 1 ] || _args.l2do[ 1 ]->GetChildCount() != 1 || 
		_args.l2do[ 1 ]->GetChild( 0 ) == 0 ||
		::_CM_getPrecedence( _args.l2do[ 1 ]->GetChild( 0 ) ) > mmlPrecedence_Atom )
			is_fenced = 1;

	MMLContent_TT_mrow setitem2insert;
	CFormulaNode *_node_apply = InsertMMLFormulaNode( setitem2insert.getRoot() );
	setitem2insert.reset();
	if( _node_apply )
	{
		CLineNode *_line = _node_apply->getFormulaLineChild( 0 );
		if( _line )
    	{
			if( !is_fenced )
			{
				for( i = 0; i < (long) _args.l2do.size(); i++ )
					_line->PumpLine2Line( _args.l2do[ i ] );
			}
			else
			{
				_line->PumpLine2Line( _args.l2do[ 0 ] );
				MMLContent_TT_mrow _empty_mrow;
				CFormulaNode *_node = _line->InsertMMLFormulaNode( _empty_mrow.getRoot() );
				if( _node )
				{
					CLineNode *_l = _node->getFormulaLineChild( 0 );
					if( _l )
					{
						_l->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("(") );
						for( i = 1; i < (long) _args.l2do.size(); i++ )
							_l->PumpLine2Line( _args.l2do[ i ] );
						_l->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T(")") );
					}
					_node->to_mathml_data.name = _T("");
					_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				}
			}
		}
	}
	to_read.erase(to_read.begin(), to_read.end());
	for( i = 0; i < (long) _args.l2do.size(); i++ )
	{
		if( _args.l2do[ i ] )
		{
			delete _args.l2do[ i ];
			_args.l2do[ i ] = 0;
		}
	}
	_args.l2do.resize( 1, NULL );

	return _node_apply;
}

#define __DIFF_DXDT_PREFIX	_T("d")

CFormulaNode *CLineNode::createEmptyForm__diff_dxdt( 
	QString d_tag, QString d_text, std::vector<long>& bvar_idx_ar, 
	AST_TXML_Tag *degree, AST_TXML_Tag *degree_aspect,
        std::vector<CLineNode*>& to_read, CCM_apply_args& _args, int /*is_fenced*/ )
{
	CFormulaNode *retnode = 0;
	QString diffAspect( _T("&prime;") );
	long degreeValue = 1;
	AST_TXML_Tag *bvarTag = 0;
	if( bvar_idx_ar.size() > 0 )
	{
		diffAspect = _T("");
		degreeValue = -1;
		if( ::isAutographDiff() && bvar_idx_ar.size() == 1 )
		{
			bvarTag = (AST_TXML_Tag*)(_args.xml_tags->getChild( bvar_idx_ar[ 0 ] ));
			AST_TXML_Tag *dgrTag = bvarTag ? bvarTag->getTagChildByName( FBL_TAG_degree ) : NULL;
			AST_TXML_Tag *valTag = dgrTag ? dgrTag->getTagChildByName( FBL_TAG_cn ) : NULL;
			if( valTag )
			{
				LSDigit ldigit;
				if( ldigit.Analisys( valTag->getText().midRef(0) ) == LSDigit::Integer )
					degreeValue = ldigit.mnt_int;
			}
			switch( degreeValue )
			{
			case 1:
				diffAspect = _T("&prime;");
				break;
			case 2:
				diffAspect = _T("&Prime;");
				break;
			}
		}
	}
	if( diffAspect.length() && d_text == __DIFF_DXDT_PREFIX )
	{
		MMLContent_TT_mrow dxdt;
		retnode = InsertMMLFormulaNode( dxdt.getRoot() );
		CLineNode *retnodeLine = retnode ? retnode->getFormulaLineChild( 0 ) : NULL;
		if( retnodeLine )
		{
			CFormulaNode *frmNode = retnodeLine->InsertMMLFormulaNode( dxdt.getRoot() );
			if( frmNode )
			{
				to_read[ 0 ] = frmNode->getFormulaLineChild( 0 );
				frmNode->to_mathml_data.name = _T("");
				frmNode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				frmNode->setAutodetectContent();
			}
			else
			{
				Q_ASSERT( 0 );
				to_read[ 0 ] = 0;
			}
			retnodeLine->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, diffAspect );

			if( bvar_idx_ar.size() == 1 && bvarTag /*&& degreeValue > 1*/ )
			{
				retnodeLine->InsertMML_CM_standalone_diff_bvar( d_tag, d_text, bvarTag );
				CFormulaNode *linkFormula = retnodeLine->getLastContentFormulaChild();
				if( linkFormula )
				{
					linkFormula->setReferenced();
					linkFormula->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx_ar[ 0 ] );
					linkFormula->setHiddenNDisabled();
					linkFormula->iterateNodeTree( setRecursiveReadOnly, 0 );
				}
			}
		}
		dxdt.reset();
		return retnode;
	}

	Q_ASSERT( bvar_idx_ar.size() > 0 );
	if( bvar_idx_ar.size() == 0 ) return retnode;

	MMLContent_TT_mrow _row, _row2;
	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
	AST_TXML_Tag *root = new AST_TXML_Tag( &mfracV, 0, _tags );
	_row.set( root );
	retnode = InsertMMLFormulaNode( _row.getRoot() );
	_row.reset();
	delete root;

	CFormulaNode *frmNode = 0, *_n = 0, *numContent = 0;
	if( !retnode || !(_n = retnode->getFormulaDeepFormulaChild( 0 )) || !retnode->GetFirstChild() )
		return retnode;

	CLineNode *num = _n->getFormulaLineChild( 0 ), *denum = _n->getFormulaLineChild( 1 );
	if( !num || !denum ) return retnode;

	if( ::isAutographEditing_dxdt() )
		_n->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
	else
		retnode->GetFirstChild()->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );

	if( !degree && !degree_aspect )
		degree = (AST_TXML_Tag*)(_args.xml_tags->getChild( bvar_idx_ar[ 0 ] ));
	num->InsertMML_CM_standalone_diff_bvar_degree( d_tag, d_text, degree, degree_aspect );

	if( ::isAutographEditing_dxdt() )
		numContent = num->InsertMMLFormulaNode( _row2.getRoot() );

	AST_TXML_Tag *xml_node = 0;
	for( long i = 0; i < (long) bvar_idx_ar.size(); i++ )
	{
		xml_node = (AST_TXML_Tag*)(_args.xml_tags->getChild( bvar_idx_ar[ i ] ));
		denum->InsertMML_CM_standalone_diff_bvar( d_tag, d_text, xml_node );
		if( (frmNode = denum->getLastContentFormulaChild()) != 0 )
		{
			frmNode->setReferenced();
			frmNode->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx_ar[ i ] );
		}
	}

	if( ::isAutographEditing_dxdt() && numContent )
	{
		to_read[ 0 ] = numContent->getFormulaLineChild( 0 );

		_n->to_mathml_data.name = _T("");
		_n->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		_n->clearReadOnly();
		_n->clearIgnoreContentConversion();
		_n->setAutodetectContent();

		numContent->to_mathml_data.name = _T("");
		numContent->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		numContent->setAutodetectContent();
	}
	else
		to_read[ 0 ] = retnode->getFormulaLineChild( 0 );

	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__diff( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	long bvar_idx = -1, degree_idx = -1, list_idx = -1, arg_idx = -1, cb = 0;
	std::vector<long> bvar_idx_ar;

	::_CM_fill_diff_bvar( bvar_idx, degree_idx, list_idx, arg_idx, cb, bvar_idx_ar, _args );
	int is_fenced = 0;
	if( cb == 1 && arg_idx != -1 )
		is_fenced = ::_CM_make_fence_arg( arg_idx, mmlPrecedence_apply, _args );

	std::vector<long> bvar2insert;
	if( bvar_idx != -1 ) bvar2insert.push_back( bvar_idx );
	CFormulaNode *retnode = createEmptyForm__diff_dxdt( FBL_TAG_mi, __DIFF_DXDT_PREFIX, bvar2insert, NULL, NULL, to_read, _args, is_fenced );
	::_CM_clean_bvar( bvar_idx_ar, _args );

	return retnode;
}

CFormulaNode *CLineNode::createEmptyForm__partialdiff( 
	std::vector<CLineNode*>& to_read, CCM_apply_args& _args )
{
	const int OP_ARITY = 1;
	to_read.erase(to_read.begin(), to_read.end());
	to_read.resize( OP_ARITY, 0 );
	Q_ASSERT( _args.arity == -1 || OP_ARITY == _args.arity );// internal error

	long bvar_idx = -1, degree_idx = -1, list_idx = -1, arg_idx = -1, cb = 0, i;
	std::vector<long> bvar_idx_ar;

	CFormulaNode *retnode = 0;

	::_CM_fill_diff_bvar( bvar_idx, degree_idx, list_idx, arg_idx, cb, bvar_idx_ar, _args );
	int is_fenced = 0;
	if( cb == 1 && arg_idx != -1 )
		is_fenced = ::_CM_make_fence_arg( arg_idx, mmlPrecedence_apply, _args );

	if( list_idx == -1 && bvar_idx != -1 && bvar_idx_ar.size() > 0 )
	{
		AST_TXML_Tag *_degree_node = 0, *_degree_aspect = 0, *_degree_addon = 0;
		::_CM_build_degree( _degree_node, _degree_aspect, _degree_addon, 
							degree_idx, bvar_idx_ar, _args );

		retnode = createEmptyForm__diff_dxdt( FBL_TAG_mi, _T("&PartialD;"), bvar_idx_ar, _degree_node, _degree_aspect, to_read, _args, is_fenced );

		if( _degree_aspect )
		{
			/*AST_TXML_Compound *_tags = _degree_aspect->getTags();
			if( _tags )
			{ _tags->Erase( 0 ); _tags->detachChildren(); }*/
			delete _degree_aspect;
			_degree_aspect = 0;
		}
		/*if( _degree_addon )
		{
			delete _degree_addon;
			_degree_addon = 0;
		}*/
		::_CM_clean_bvar( bvar_idx_ar, _args );
	}
	else
	{
		if( list_idx != -1 )
		{
			CNode *_node = _args.l2do[ list_idx ]->GetFirstChild();
			if( _node && _node->GetType() == NODE_FORMULA )
			{
				CFormulaNode *_fnode = (CFormulaNode*)_node;
				CLineNode *_interior = _fnode->getFormulaLineChild( 0 );
				if( _interior )
				{
					_node = _interior->GetFirstChild();
					if( _node && _node->GetType() == NODE_PLANETEXT && 
						_node->isReadOnly() && _node->isIgnoreContentConversion() )
					{
						_interior->DeleteChild( _node );
					}
					_node = _interior->GetLastChild();
					if( _node && _node->GetType() == NODE_PLANETEXT && 
						_node->isReadOnly() && _node->isIgnoreContentConversion() )
					{
						_interior->DeleteChild( _node );
					}
				}
			}
		}

		CLineNode *_line = 0;
		MMLContent_TT_mrow _row;
		retnode = InsertMMLFormulaNode( _row.getRoot() );
		_row.reset();
		if( retnode ) _line = retnode->getFormulaLineChild( 0 );
		if( _line )
    	{
			AST_TXML_Compound *_D_tags = new AST_TXML_Compound();
			QString sV("D");
			_D_tags->Add( new AST_TXML_Tag( &miV, 0, new AST_TXML_Value( &sV ) ) );
			_D_tags->Add( new AST_TXML_Tag( &mrowV, 0, (AST_TXML_Tag*)0 ) );
			AST_TXML_Tag *_D_n = new AST_TXML_Tag( &msubV, 0, _D_tags );
			CFormulaNode *fnode = _line->InsertMMLFormulaNode( _D_n );
			delete _D_n;

			if( fnode )
			{
				if( fnode->GetFirstChild() )
					fnode->GetFirstChild()->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
				fnode->to_mathml_data.name = _T("");
				fnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				CLineNode *_interior = fnode->getFormulaLineChild( 1 );
				if( _interior && list_idx != -1 )
					_interior->PumpLine2Line( _args.l2do[ list_idx ] );
			}

			_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("(") );
			for( i = 1; i < (long) _args.l2do.size(); i++ )
				_line->PumpLine2Line( _args.l2do[ i ] );
			_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T(")") );
		}
	}

	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_standalone_diff_bvar( 
	QString tagname, QString optext, AST_TXML_Tag *xml_node )
{
	AST_TXML_Tag *degree = 0;
	AST_TXML_Compound *_tags = xml_node->getTags();
	AST_TXML_Compound body;
	long i;
	if( _tags )
	{
		for( i = 0; i < _tags->getChildNumber(); i++ )
		{
			if( _tags->getChild( i ) && 
				_tags->getChild( i )->getNodeType() == ASTT_TXML_Tag )
			{
				AST_TXML_Tag *_n = (AST_TXML_Tag*)_tags->getChild( i );
				if( _n->getTagName() == FBL_TAG_degree )
					degree = _n;
				else
					body.Add( _n );
			}
		}
	}

	MMLContent_TT_Token _dd;
	_dd.set( tagname, optext );
	body.InsertChild( 0, _dd.getRoot() );

	_tags = new AST_TXML_Compound();
	AST_TXML_Tag *root = 0, *_rt_base = 0, *_rt_parent = 0;
	MMLContent_TT_mrow _row;
	CFormulaNode *newnode = 0;
	if( degree )
	{
		_rt_base = degree;
		if( body.getChildNumber() == 1 )
			_rt_parent = (AST_TXML_Tag*)body.getChild( 0 );
		else
		{
			_row.set( &body );
			_rt_parent = _row.getRoot();
		}

		_tags->Add( _rt_parent );
		_tags->Add( _rt_base );
		root = new AST_TXML_Tag( &msupV, 0, _tags );
		newnode = InsertMMLFormulaNode( root );
	}
	else
	{
		root = new AST_TXML_Tag( &mrowV, 0, &body );
		newnode = InsertMMLFormulaNode( root );
		root->setTags( 0 );
	}

	_dd.reset();
	_row.reset();
	body.detachChildren();
	if( root->getTags() )
		root->getTags()->detachChildren();
	delete root;

	if( newnode )
	{
		_CM_tune_Node_property( newnode, xml_node );
		newnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;

		CLineNode *lineNode = 0;
		CFormulaNode *frmNode = 0;
		if( degree )
		{
			newnode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
			newnode->clearReadOnly();

			frmNode = newnode->getFormulaDeepFormulaChild( 0, 0 );
			if( frmNode )
			{
				//frmNode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
				//frmNode->clearReadOnly();
				frmNode->clearIgnoreContentConversion();
				frmNode->to_mathml_data.name = _T("");
				frmNode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;

				lineNode = frmNode->getFormulaLineChild( 0, 0 );
				if( lineNode )
				{
					frmNode = lineNode->getLastContentFormulaChild();
					if( frmNode )
					{
						frmNode->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
					}
				}
			}
			frmNode = newnode->getFormulaDeepFormulaChild( 1, 0 );
			if( frmNode )
			{
				frmNode->clearReadOnly();
				frmNode->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
				//frmNode->clearIgnoreContentConversion();
				//frmNode->to_mathml_data.name = _T("");
				//frmNode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			}
		}
		else
		{
			newnode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
			newnode->clearReadOnly();

			lineNode = newnode->getFormulaLineChild( 0, 0 );
			if( lineNode )
			{
				frmNode = lineNode->getLastContentFormulaChild();
				if( frmNode ) frmNode->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
			}
		}
	}

	return newnode;
}

void CLineNode::InsertMML_CM_standalone_diff_bvar_degree( 
	QString tagname, QString optext, AST_TXML_Tag *xml_node,
	AST_TXML_Tag *degree_aspect )
{
	MMLContent_TT_Token _degree_token;
	AST_TXML_Tag *degree = 0;
	AST_TXML_Compound *_tags = 0;
	if( xml_node )
		_tags = xml_node->getTags();
	long i;
	if( degree_aspect )
		degree = degree_aspect;
	if( _tags )
	{
		for( i = 0; i < _tags->getChildNumber(); i++ )
		{
			if( _tags->getChild( i ) && 
				_tags->getChild( i )->getNodeType() == ASTT_TXML_Tag )
			{
				AST_TXML_Tag *_n = (AST_TXML_Tag*)_tags->getChild( i );
				if( _n->getTagName() == FBL_TAG_degree )
				{
					degree = _n;
					break;
				}
			}
		}
	}

	MMLContent_TT_Token _dd;
	_dd.set( tagname, optext );
	if( degree )
	{
		AST_TXML_Tag *root = 0/*, *_rt_base = 0, *_rt_parent = 0*/;
		AST_TXML_Compound *_tags = new AST_TXML_Compound();
		_tags->Add( _dd.getRoot() );
		_tags->Add( degree );
		root = new AST_TXML_Tag( &msupV, 0, _tags );
		InsertMMLFormulaNode( root );
		root->getTags()->detachChildren();
		delete root;

		CFormulaNode *frmNode = getLastFormulaChild();
		if( frmNode )
		{
			frmNode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
			if( !degree_aspect )
			{
				CFormulaNode *powerNode = frmNode->getFormulaDeepFormulaChild( 1, 0 );
				if( powerNode )
				{
					powerNode->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
					powerNode->setReferenced();
					powerNode->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_degree( 0 );
				}
			}
/*			frmNode->clearReadOnly();
			CLineNode *lineNode = newnode->getFormulaLineChild( 0, 0 );
			if( lineNode )
			{
				CFormulaNode *frmNode = lineNode->getLastContentFormulaChild();
				if( frmNode ) frmNode->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
			}

			frmNode->setReferenced();
			frmNode->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx );*/
		}
	
	
	}
	else
	{
		read_mathml( _dd.getRoot() );
		CNode *_n = GetLastChild();
		if( _n )
		{
			_n->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
			//_n->setReadOnly(); // lev
		}
	}
	_dd.reset();
	_degree_token.reset();
}

///////////////////////////////////////////////////////////////////////////////

void _CM_fill_diff_bvar( 
	long& bvar_idx, long& degree_idx, long& list_idx, long& arg_idx, long& cb,
	std::vector<long>& bvar_idx_ar, CCM_apply_args& _args )
{
	for( long i = 1; i < _args.xml_tags->getChildNumber() && i < (long) _args.l2do.size(); i++ )
	{
		QString tn = ((AST_TXML_Tag*)(_args.xml_tags->getChild( i )))->getTagName();
		if( _args.l2do[ i ] )
		{
			if( tn == FBL_TAG_bvar )
			{
				bvar_idx = i;
				bvar_idx_ar.push_back( i );
			}
			else if( tn == FBL_TAG_degree )
				degree_idx = i;
			else if( tn == FBL_TAG_list )
				list_idx = i;
			else
			{
				arg_idx = i;
				cb++;
			}
		}
	}
}

int _CM_make_fence_arg( long arg_idx, long precedence, CCM_apply_args& _args )
{
	long argPrecedence = 
		( _args.l2do[ arg_idx ]->GetChildCount() == 1 && 
		  _args.l2do[ arg_idx ]->GetFirstChild() ) ?
                        (long) ::_CM_getPrecedence( _args.l2do[ arg_idx ]->GetFirstChild() ) :
                        (long) mmlPrecedence_Atom;
	if( argPrecedence > precedence )
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
		return 1;
	}
	return 0;
}

void _CM_clean_bvar( std::vector<long>& bvar_idx_ar, CCM_apply_args& _args )
{
	long i;
	for( i = 0; i < (long) bvar_idx_ar.size(); i++ )
	{
		CLineNode *bvarLine = _args.l2do[ bvar_idx_ar[ i ] ];
		if( bvarLine )
		{
			if( _args.l2do[ 0 ] )
			{
				CFormulaNode* _fn = bvarLine->getContentFormulaChild( 0 );
				if( _fn )
				{
					_fn->setContentReference();
					_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_bvar( bvar_idx_ar[ i ] );
				}
				_args.l2do[ 0 ]->PumpLine2Line( bvarLine );
			}
			delete _args.l2do[ bvar_idx_ar[ i ] ];
			_args.l2do[ bvar_idx_ar[ i ] ] = 0;
		}
	}

	for( i = 1; i < (long) _args.l2do.size(); )
	{
		if( _args.l2do[ i ] == 0 )
			_args.l2do.erase( _args.l2do.begin() + i );
		else
			i++;
	}
}

void _CM_build_degree( 
	AST_TXML_Tag *&_degree_node, AST_TXML_Tag *&_degree_aspect, 
        AST_TXML_Tag *& /*_degree_addon*/, long degree_idx, std::vector<long>& bvar_idx_ar, CCM_apply_args& _args )
{
	if( degree_idx != -1 )
	{
		_degree_node = _args.xml_n;
		if( _args.l2do[ degree_idx ] )
		{
			if( _args.l2do[ 0 ] )
			{
				CFormulaNode* _fn = _args.l2do[ degree_idx ]->getContentFormulaChild( 0 );
				if( _fn )
				{
					_fn->setContentReference();
					_fn->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_degree( 0 );
				}
				_args.l2do[ 0 ]->PumpLine2Line( _args.l2do[ degree_idx ] );
			}
			delete _args.l2do[ degree_idx ];
			_args.l2do[ degree_idx ] = 0;
		}

		return;
	}

	long degree_counter = 0, i, j, f = 0;
	AST_TXML_Tag *_tmp = 0, *_tmp1 = 0, *_tmp2 = 0;

	AST_TXML_Compound *_tags = new AST_TXML_Compound();
	_tags->Add( new AST_TXML_Tag( &plusTagV, 0, (AST_TXML_Tag*)0 ) );

	for( i = 0; i < (long) bvar_idx_ar.size(); i++ )
	{
		_tmp = (AST_TXML_Tag*)(_args.xml_tags->getChild( bvar_idx_ar[ i ] ));
		f = 0;
		if( _tmp && _tmp->getTags() )
		{
			for( j = 0; j < _tmp->getTags()->getChildNumber(); j++ )
			{
				if( !_tmp->getTags()->getChild( j ) || 
					_tmp->getTags()->getChild( j )->getNodeType() != ASTT_TXML_Tag )
					continue;
				_tmp1 = (AST_TXML_Tag*)(_tmp->getTags()->getChild( j ));
				if( _tmp1->getTagName() != FBL_TAG_degree )
					continue;
				if( _tmp1->getTags() )
				{
					AST_TXML_Compound *_cn_tags = _tmp1->getTags();
					if( _cn_tags->getChildNumber() == 1 && 
						_cn_tags->getChild( 0 ) &&
						_cn_tags->getChild( 0 )->getNodeType() == ASTT_TXML_Tag )
					{
						_tmp2 = (AST_TXML_Tag*)(_cn_tags->getChild( 0 ));
						QString str;
						if( _tmp2->getTagName() == FBL_TAG_cn &&
							(str = _tmp2->getDeepChildValueText()).length() )
						{
							LDigit ld;
							if( ld.Analisys( str.midRef(0) ) == LDigit::Integer )
							{
								degree_counter += ld.mnt_int;
								_tmp = 0;
								f = 1;
								break;
							}
						}
					}
				}
				_tmp = _tmp1; f = 1; break; 
			}
		}
		if( f == 0 )
			degree_counter++;
		else if( _tmp )
		{
			_tags->Add( new AST_TXML_Tag( *_tmp ) );
		}
	}
	QString sV = QString("%1").arg(degree_counter);
	if( degree_counter > 1 )
		_tags->Add( new AST_TXML_Tag( &cnV, 0, new AST_TXML_Value( &sV ) ) );
	switch( _tags->getChildNumber() )
	{
	case 1:
		_degree_aspect = 0;
		delete _tags;
		break;
	case 2:
		_degree_aspect = (AST_TXML_Tag*)(_tags->getChild( 1 ));
		_tags->ReplaceChild( 1, NULL );
		delete _tags;
		break;
	default:
		_degree_aspect = new AST_TXML_Tag( &applyV, 0, _tags );
	}
}

///////////////////////////////////////////////////////////////////////////////
