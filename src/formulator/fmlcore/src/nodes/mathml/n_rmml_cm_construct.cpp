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
#include "../../mathml/mml_tag2mml.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_construct_empty_vector( AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_mfenced vector_frame;
	/*static*/ MMLContent_TT_mrow _empty_mrow;
	vector_frame.set( _empty_mrow.getRoot() );
	CFormulaNode *_node = InsertMMLFormulaNode( vector_frame.getRoot() );
	vector_frame.reset();
	if( !_node ) return _node;

	//=== interior mrow must not be converted as "mrow", but as a content filler
	CFormulaNode *_slot = _node->getFormulaDeepFormulaChild( 0 );
	if( _slot )
	{
		_slot->to_mathml_data.name = _T("");
		_slot->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	}

	_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	_CM_basic_Node_property( _node, xml_node );

	return _node;
}

CFormulaNode* CLineNode::InsertMML_CM_construct_vector( 
        enum TAG2MML /*tag_id*/, QString rows, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_Vector vec2insert;

	if( !xml_node->getTags() )
		return InsertMML_CM_construct_empty_vector( xml_node );

	std::vector<AST_TXML_Tag*> matrixrow_nodes;
	AST_TXML_Node *_xml_child;
	AST_TXML_Tag *_child;

	for( long i = 0; i < xml_node->getTags()->getChildNumber(); i++ )
	{
		if( (_xml_child = xml_node->getTags()->getChild( i )) != NULL &&
			 _xml_child->getNodeType() == ASTT_TXML_Tag )
		{
			_child = (AST_TXML_Tag*)_xml_child;
			if( rows.length() && 
				xml_strcmp( _child->getTagName(), rows ) )
				break;
			matrixrow_nodes.push_back( _child );
		}
	}

	if( matrixrow_nodes.size() == 0 )
		return InsertMML_CM_construct_empty_vector( xml_node );

	/*static*/ MMLContent_TT_mrow _empty_row;
	CFormulaNode *_node = InsertMMLFormulaNode( _empty_row.getRoot() );
	_empty_row.reset();
	if( !_node ) return InsertMML_CM_construct_empty_vector( xml_node );
	CLineNode *_line = _node->getFormulaLineChild( 0 );
	if( !_line ) return InsertMML_CM_construct_empty_vector( xml_node );

	_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("(") );
	vec2insert.set( matrixrow_nodes, (int) rows.length() );
	CFormulaNode *_row = _line->InsertMMLFormulaNode( vec2insert.getRoot() );
	vec2insert.reset();
	if( _row )
	{
		_row->to_mathml_data.name = _T("");
		_row->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	}
	_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T(")") );

	_CM_basic_Node_property( _node, xml_node );
	if( rows.length() )
		_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_MATRIX;
	else
		_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;

	return _node;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_construct_interval( 
	enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_mfenced_tuned set2insert;
	QString _l = _T("["), _r = _T("]"), _coma = FBL_VAL_DEFAULT_mfenced_separator;

	AST_TXML_Attribute *_attr = xml_node->getAttribute( FBL_ATTR_closure );
	if( _attr )
	{
		if( !::xml_strcmp( _attr->getStringValue(), FBL_VAL_open ) )
		{			_l = _T("("); _r = _T(")");		}
		else if( !::xml_strcmp( _attr->getStringValue(), FBL_VAL_open_closed ) )
		{			_l = _T("("); _r = _T("]");		}
		else if( !::xml_strcmp( _attr->getStringValue(), FBL_VAL_closed_open ) )
		{			_l = _T("["); _r = _T(")");		}
	}
	int is_empty = !xml_node->getTags() || !xml_node->isExistTagChild();

	// all non-tag children are ignored
	xml_node->deleteValueChildren();

	AST_TXML_Compound _tags;
	CFormulaNode *_node = InsertMML_CM_construct_list__constructive( 
		tag_id, is_empty ? &_tags : xml_node->getTags(), _l, _r, _coma, QString( _T("") ) );

	if( _node )
	{
		_CM_basic_Node_property( _node, xml_node );
		_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	}

	return _node;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_construct_declare( 
	enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_mfenced_tuned set2insert;
	QString _head = ::mml_getCM2PMText_standalone( tag_id, xml_node );
	QString _l = _T(""), _r = _T(""), _coma = _T("&larr;");
	int is_empty = !xml_node->getTags() || !xml_node->isExistTagChild();
	// all non-tag children are ignored
	xml_node->deleteValueChildren();
	AST_TXML_Compound _tags;
	CFormulaNode *_node = InsertMML_CM_construct_list__constructive( 
							tag_id, is_empty ? &_tags : xml_node->getTags(), 
							_l, _r, _coma, _head );
	if( _node )
	{
		_CM_basic_Node_property( _node, xml_node );
		_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
//		InsertMMLContent__read_attributes( _node, xml_node, xml_node->getTagName(), CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE );

		if( !::isRenderMode_Force() && !xml_node->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) )
		{
			_node->setHiddenNDisabled();
			_node->iterateNodeTree( setRecursiveReadOnly, 0 );
		}
	}

	return _node;
}

CFormulaNode* CLineNode::InsertMML_CM_construct_lamda( 
	enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_mfenced_tuned set2insert;
	QString _head = ::mml_getCM2PMText_standalone( tag_id, xml_node );
	QString _l = _T("("), _r = _T(")"), _coma = FBL_VAL_DEFAULT_mfenced_separator;
	int is_empty = !xml_node->getTags() || !xml_node->isExistTagChild();
	// all non-tag children are ignored
	xml_node->deleteValueChildren();
	AST_TXML_Compound _tags;

	std::vector<QString> comaList;
	QString *comaArr = 0;
	long comaLen = -1;
	int wasDomain = -1;
	if( xml_node->getTags() && xml_node->getTags()->getChildNumber() > 0 )
	{
		AST_TXML_Node *da = 0;
		comaList.resize( xml_node->getTags()->getChildNumber(), _coma );
		for( long i = 0; i < (long) comaList.size(); i++ )
		{
			da = xml_node->getTags()->getChild( i );
			if( da && da->getNodeType() == ASTT_TXML_Tag )
			{
				if( ((AST_TXML_Tag*)da)->getTagName() == FBL_TAG_domainofapplication )
				{
					comaList[ i ] = _T(":");
					if( wasDomain == -1 ) wasDomain = i;
				}
			}
		}
		comaArr = &(comaList[ 0 ]);
		comaLen = (long) comaList.size();
	}

	CFormulaNode *_node = 0;
	if( wasDomain == 0 && xml_node->getTags() && xml_node->getTags()->getChildNumber() == 2 )
	{
		_node = InsertMML_CM_construct_lambda__domainofapplication( tag_id, xml_node->getTags(), _head );
		
		if( _node )
		{
			_CM_basic_Node_property( _node, xml_node );
			_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__BACKRECURSIVE_TREE;
		}
	}
	else
	{
		_node = InsertMML_CM_construct_list__constructive( tag_id, is_empty ? &_tags : xml_node->getTags(), _l, _r, _coma, _head, comaArr, comaLen );

		if( _node )
		{
			_CM_basic_Node_property( _node, xml_node );
			_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		}
	}

	return _node;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_construct_set( 
	enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_mfenced_tuned set2insert;
	QString _l = _T(""), _r = _T(""), _coma = FBL_VAL_DEFAULT_mfenced_separator;
	if( tag_id == TAG2MML_set )	{ _l = _T("{"); _r = _T("}"); }
	else if( tag_id == TAG2MML_list ) { _l = _T("["); _r = _T("]"); }
	else if( tag_id == TAG2MML_NONE ) { tag_id = TAG2MML_list; /*but there must be no brackets!!!*/}

    CFormulaNode *_node = 0;

	if( !xml_node->getTags() || !xml_node->isExistTagChild() )
	{
		// /*static*/ MMLContent_TT_mrow _empty_mrow;
		//set2insert.set( _empty_mrow.getRoot() );
		//_node = InsertMMLFormulaNode( set2insert.getRoot() );
		//set2insert.reset();
		MMLContent_TT_mrow _empty_mrow;
		_node = InsertMMLFormulaNode( _empty_mrow.getRoot() );
		_empty_mrow.reset();

		if( _node )
		{
			CLineNode *_line = _node->getFormulaLineChild( 0 );
			if( _line )
			{
				if( _l.length() )
					_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _l );
				if( _r.length() )
					_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _r );
			}
			_CM_basic_Node_property( _node, xml_node );
			_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		}
		return _node;
	}

	xml_node->deleteValueChildren();

	int is_x = 0;
	std::vector<AST_TXML_Tag*> bvar_nodes;
	std::vector<AST_TXML_Tag*> cond_nodes;
	InsertMML_CM_construct_set__parse( xml_node->getTags(), is_x, bvar_nodes, cond_nodes );
	long is_content = 0;

	std::vector<QString> comaList;
	QString *comaArr = 0;
	long comaLen = -1;

	int isForceRender = 0;
	CLineNode *line_node = new CLineNode();
	if( bvar_nodes.size() && cond_nodes.size() )
	{
		AST_TXML_Compound _tags;
		long k;
		if( is_x )
		{
			k = (long) (bvar_nodes.size() + cond_nodes.size());
			for( ; k < xml_node->getTags()->getChildNumber(); k++ )
			{
				comaList.push_back( _coma );
				_tags.Add( xml_node->getTags()->getChild( k ) );
			}

			isForceRender = 1;
			for( k = 0; k < (long) bvar_nodes.size(); k++ )
			{
				if( !::isRenderMode_Force() && !bvar_nodes[ k ]->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) )
				{
					isForceRender = 0;
					break;
				}
			}

			if( isForceRender )
			{
				for( k = 0; k < (long) bvar_nodes.size(); k++ )
				{
					comaList.push_back( k ? _coma : _T("") );
					//_tags.Add( bvar_nodes[ k ] );
				}

				comaArr = &(comaList[ 0 ]);
				comaLen = (long) comaList.size();
			}
			else
			{
				for( k = 0; k < (long) bvar_nodes.size(); k++ )
					line_node->InsertMML_CM_standalone_transparent( TAG2MML_bvar, bvar_nodes[ k ] );
			}
		}
		else
		{
			for( k = 0; k < (long) bvar_nodes.size(); k++ )
				_tags.Add( bvar_nodes[ k ] );
		}
		_tags.Add( cond_nodes[ 0 ] );

		if( isForceRender )
			_node = InsertMML_CM_construct_list__constructive( tag_id, &_tags, _l, _r, _coma, QString( _T("") ), 0, -1, &bvar_nodes );
		else
			_node = InsertMML_CM_construct_list__constructive( tag_id, &_tags, _l, _r, _coma, QString( _T("") )/*, comaArr, comaLen*/ );
		is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_SET;

		_tags.detachChildren();
	}
	else
	{
		_node = InsertMML_CM_construct_list__constructive( tag_id, xml_node->getTags(), _l, _r, _coma, QString( _T("") ) );
		is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	}

	if( !_node ) return _node;

	_CM_basic_Node_property( _node, xml_node );
	_node->to_mathml_data.is_content = (short) is_content;

	if( line_node )
	{
		if( line_node->GetChildCount() )
			_node->setAddonNode( line_node );
		else
			delete line_node;
	}

	return _node;
}

void CLineNode::InsertMML_CM_construct_set__parse( AST_TXML_Compound *_tags, 
	int& is_x, std::vector<AST_TXML_Tag*>& bvar_nodes, std::vector<AST_TXML_Tag*>& cond_nodes )
{
	AST_TXML_Node *_n = 0;
	long k;
	for( k = 0; k < _tags->getChildNumber(); k++ )
	{
		_n = _tags->getChild( k );
		if( _n && _n->getNodeType() == ASTT_TXML_Tag )
		{
			if( !xml_strcmp(((AST_TXML_Tag*)_n)->getTagName(), 
							FBL_TAG_bvar ))
				bvar_nodes.push_back((AST_TXML_Tag*)_n);
			else
				break;
		}
	}
	if( k < _tags->getChildNumber() )
	{
		if( (_n = _tags->getChild( k )) != NULL )
		{
			if( _n->getNodeType() == ASTT_TXML_Tag )
			{
				if( !xml_strcmp(((AST_TXML_Tag*)_n)->getTagName(), FBL_TAG_condition ))
					cond_nodes.push_back((AST_TXML_Tag*)_n);
			}
			k++;
		}
	}
	if( k < _tags->getChildNumber() )
	{
		if( k != _tags->getChildNumber() - 1 )
		{
			//bvar_nodes.erase(cond_nodes.begin(),cond_nodes.end());
			//bvar_nodes.erase(bvar_nodes.begin(),bvar_nodes.end());
			//cond_nodes.erase(cond_nodes.begin(),cond_nodes.end());
			bvar_nodes.clear();
			cond_nodes.clear();
		}
		else
		{
			_n = _tags->getChild( k );
			if( _n && _n->getNodeType() == ASTT_TXML_Tag )
				is_x = 1;
		}
	}   	
}       	

CFormulaNode* CLineNode::InsertMML_CM_construct_list__constructive( 
	enum TAG2MML tag_id, AST_TXML_Compound *_tags, 
	QString _l, QString _r, QString _coma, QString _head, QString *comaList, long comaListLen,
	std::vector<AST_TXML_Tag*>* pBvarNodes )
{
	std::vector<QString> vectorComaList;
	long i, j;
	if( comaList != 0 && comaListLen > 0 )
	{
		vectorComaList.resize( comaListLen > _tags->getChildNumber() ? comaListLen : _tags->getChildNumber(), _T("") );
		for( i = 0; i < comaListLen; i++ )
			vectorComaList[ i ] = comaList[ i ];
		for( ; i < (long) vectorComaList.size(); i++ )
			vectorComaList[ i ] = _coma;
	}
	else if( _tags->getChildNumber() > 0 )
		vectorComaList.resize( _tags->getChildNumber(), _coma );

	AST_TXML_Tag *_n;
	/*static*/ MMLContent_TT_mrow setitem2insert;
	CFormulaNode *_node = InsertMMLFormulaNode( setitem2insert.getRoot() );
	setitem2insert.reset();
	if( !_node ) return _node;
	CLineNode *_line = _node->getFormulaLineChild( 0 );
	if( !_line ) return _node;

	if( _head.length() )
		_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _head );
	if( _l.length() )
		_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _l );

	long k;
	for( k = 0; k < _tags->getChildNumber(); k++ )
	{
		if( _tags->getChild( k ) && 
			_tags->getChild( k )->getNodeType() == ASTT_TXML_Tag )
		{
			_n = (AST_TXML_Tag*)(_tags->getChild( k ));

			if( k )
			{
				if( (tag_id == TAG2MML_set || tag_id == TAG2MML_list) && 
					_n->getTagName() == FBL_TAG_condition )
				{
					if( pBvarNodes == 0 )
						_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("|") );
					else
					{
						MMLContent_TT_MSupSub item2insert( QString( FBL_TAG_msub ) );
						CFormulaNode *_node = _line->InsertMMLFormulaNode( item2insert.getRoot() );
						if( _node )
						{
							CLineNode *_lineParent = _node->getFormulaLineChild( 0 );
							CLineNode *_lineBase = _node->getFormulaLineChild( 1 );
							if( _lineParent != 0 && _lineBase != 0 )
							{
								for( j = 0; j < (long) pBvarNodes->size(); j++ )
								{
									if( j ) _lineBase->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _coma );
									_lineBase->read_mathml( (*pBvarNodes)[ j ] );
								}
								_lineParent->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("|") );
								_node->to_mathml_data.reserved = _node->to_mathml_data.name;
								_node->to_mathml_data.name = _T("");
								_node->to_mathml_data.mathml_template_string = ::getContentMathMLReferenceID_separatorbvar();
								_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
							}
						}
					}
				}
				else if( tag_id == TAG2MML_interval )
				{
					 if( k < 2 )
						_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _coma );
				}
				else
				{
					if( k < (long) vectorComaList.size() )
					{
						if( vectorComaList[ k ].length() )
							_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, vectorComaList[ k ] );
					}
					else
						_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _coma );
				}
			}

			_line->read_mathml( _n );
		}
	}

	if( tag_id == TAG2MML_interval && k < 2 )
	{
		CFormulaNode *_row = 0;
		for( ; k < 2; k++ )
		{
			if( k )
				_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _coma );
			_row = _line->InsertMMLFormulaNode( setitem2insert.getRoot() );
			if( _row )
			{
				_row->to_mathml_data.name = _T("");
				_row->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			}
		}
	}

	if( _r.length() )
		_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _r );
	return _node;
}

CFormulaNode* CLineNode::InsertMML_CM_construct_lambda__domainofapplication( 
        enum TAG2MML /*tag_id*/, AST_TXML_Compound *_tags, QString /*_head*/ )
{
	MMLContent_TT_MSupSub item2insert( QString( FBL_TAG_msub ) );
	CFormulaNode *_node = InsertMMLFormulaNode( item2insert.getRoot() );
	if( !_node ) return _node;
	CLineNode *_lineParent = _node->getFormulaLineChild( 0 );
	CLineNode *_lineBase = _node->getFormulaLineChild( 1 );
	if( _lineParent == 0 || _lineBase == 0 ) return _node;

	AST_TXML_Tag *_n0 = (AST_TXML_Tag*)(_tags->getChild( 0 ));
	AST_TXML_Tag *_n1 = (AST_TXML_Tag*)(_tags->getChild( 1 ));
	if( !_n0 || _n0->getNodeType() != ASTT_TXML_Tag || !_n1 || _n1->getNodeType() != ASTT_TXML_Tag ) return _node;

	_lineParent->read_mathml( (AST_TXML_Tag*)_n1 );
	_lineBase->read_mathml( (AST_TXML_Tag*)_n0 );

	_node->iterateNodeTree( clearRecursiveReadOnly, 0 );
	_node->clearReadOnly();

	_lineParent->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("|") );

	return _node;
}

///////////////////////////////////////////////////////////////////////////////
