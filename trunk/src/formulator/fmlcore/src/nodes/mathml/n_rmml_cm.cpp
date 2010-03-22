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

#include "../../dictionary/op_iddefs.h"
#include "../../dictionary/op_autokeyword.h"
#include "n_rmml_cm.h"

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMML_CM_annotation( enum TAG2MML /*tag_id*/, AST_TXML_Tag *xml_node )
{
	if( !xml_node ) return 0;
	CFormulaNode *retnode = InsertMMLFormulaNode( MMLContent_TT_mrow().getRoot() );
	_CM_basic_Node_property( retnode, xml_node );
	QString str = _T("");
	if( xml_node->getTags() )
	{
		xml_node->setLevelThrouhTree();
		xml_node->getTags()->getXML( str, FL_XML_PRINT_TAG_SINGLE_SPACING );
		if( str.length() && str[ str.length() - 1 ] != _T('\n') )
			str += _T('\n');
	}
	retnode->to_mathml_data.mathml_template_string = (str[ 0 ] != _T('\n')) ? str : (str + 1);
	retnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__UNKNOWN_XML;
	return retnode;
}

CFormulaNode* CLineNode::InsertMML_CM_annotation_xml( enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	if( !xml_node ) return 0;
	CFormulaNode *_node = 0;

	AST_TXML_Attribute *_attr = xml_node->getAttribute( FBL_ATTR_encoding );
	if( !_attr )
		return InsertMML_CM_annotation( tag_id, xml_node );

	if( _attr->getStringValue() == FBL_VAL_MathML_Presentation || ( _attr->getStringValue() == FBL_VAL_MathML_Content && 
		(::isRenderMode_Force() || xml_node->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER ) ) ) )
	{
		if( xml_node->getTags() )
		{
			MMLContent_TT_mrow _row;
			_node = InsertMMLFormulaNode( _row.getRoot() );
			_row.reset();
			if( !_node ) return _node;
			CLineNode *_line = _node->getFormulaLineChild( 0 );
			if( !_line ) return _node;
			_line->read_mathml( xml_node->getTags() );
			_CM_basic_Node_property( _node, xml_node );

			_CM_basic_Node_property( _node, xml_node );
			_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		}
	}
	else
		_node = InsertMML_CM_annotation( tag_id, xml_node );

	return _node;
}

CFormulaNode *CLineNode::InsertMML_CM_semantics( 
        enum TAG2MML /*tag_id*/, AST_TXML_Tag *xml_node )
{
	long i;
	if( !xml_node || !xml_node->getTags() || !xml_node->isExistTagChild() )
		return 0;
	AST_TXML_Compound *children = xml_node->getTags();
	for( i = 0; i < children->getChildNumber(); )
	{
		if( children->getChild( i ) == 0 ||
			children->getChild( i )->getNodeType() != ASTT_TXML_Tag )
			children->Erase( i );
		else
			i++;
	}
	if( children->getChildNumber() == 0 )
		return 0;
	
	AST_TXML_Tag *_node1st = (AST_TXML_Tag*)(children->getChild( 0 ));
	AST_TXML_Tag *_pm = 0, *_cm = 0, *_n = 0;
	std::vector<AST_TXML_Tag*> _to_addon;
	for( i = 1; i < children->getChildNumber(); i++ )
	{
		_n = (AST_TXML_Tag*)(children->getChild( i ));
		QString tn = _n->getTagName();
		if( tn == FBL_TAG_annotation_xml )
		{
			AST_TXML_Attribute *_attr = _n->getAttribute( FBL_ATTR_encoding );
			if( _attr && _attr->getStringValue() == FBL_VAL_MathML_Presentation )
			{
				_pm = _n;
				_n = 0;
			}
			if( _attr && _attr->getStringValue() == FBL_VAL_MathML_Content )
			{
				_cm = _n;
				_n = 0;
			}
		}
		if( _n ) _to_addon.push_back( _n );
	}

	if( ::mml_is_row_tag( _node1st->getTagName() ) )
		_node1st->getFlags().setFlag( OLIMP_MATHML_FLAG__MROW_KEEP1CHILD );

	long precedence = mmlPrecedence_Atom;
	short assoc = 0;
	CLineNode *_cm_line = 0, *_node1st_line = 0, *_addon = 0;

	_node1st_line = new CLineNode();
	_node1st_line->read_mathml( _node1st );

	int forceRender = ::isRenderMode_Force() || xml_node->getFlags().isSetFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER );

	if( _cm )
	{
		_cm_line = new CLineNode();
		if( forceRender ) _cm->getFlags().setFlag( OLIMP_MATHML_FLAG__MROW_FORCE_RENDER );
		_cm_line->read_mathml( _cm );
		if( _cm_line->GetFirstChild() )
		{
			precedence = _cm_line->GetFirstChild()->to_mathml_data.precedence;
			assoc = _cm_line->GetFirstChild()->to_mathml_data.assoc;
		}
	}
	else if( _node1st_line->GetFirstChild() )
	{
		precedence = _node1st_line->GetFirstChild()->to_mathml_data.precedence;
		assoc = _node1st_line->GetFirstChild()->to_mathml_data.assoc;
	}

	CFormulaNode *retnode = 0;
	MMLContent_TT_mrow _row;
	_addon = new CLineNode();

	if( !forceRender )
	{
		if( _pm ) _addon->read_mathml( _pm );
		if( _cm_line ) _addon->PumpLine2Line( _cm_line );
		for( i = 0; i < (long) _to_addon.size(); i++ )
			_addon->read_mathml( _to_addon[ i ] );

		retnode = InsertMMLFormulaNode( _row.getRoot() );
		if( retnode )
		{
			CLineNode *_l = retnode->getFormulaLineChild( 0 );
			if( _l ) _l->PumpLine2Line( _node1st_line );
			retnode->to_mathml_data.is_content = 
				CONTENT_MATHML_CONVERTION2__SEMANTICS_LITERALLY;
		}
	}
	else
	{
		for( i = 0; i < (long) _to_addon.size(); i++ )
			_addon->read_mathml( _to_addon[ i ] );

		retnode = InsertMMLFormulaNode( _row.getRoot() );
		if( retnode )
		{
			retnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__SEMANTICS_LITERALLY;
			CLineNode *_l = retnode->getFormulaLineChild( 0 );
			if( _l )
			{
				_l->PumpLine2Line( _node1st_line );

				if( _pm )
				{
					MMLContent_TT_MSupSub item2insert( QString( FBL_TAG_msub ) );
					CFormulaNode *_node = _l->InsertMMLFormulaNode( item2insert.getRoot() );
					if( _node )
					{
						CLineNode *_lineParent = _node->getFormulaLineChild( 0 );
						CLineNode *_lineBase = _node->getFormulaLineChild( 1 );
						if( _lineParent != 0 && _lineBase != 0 )
						{
							_lineParent->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("|") );
							_lineBase->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("P") );
						}
						_node->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
					}
					_l->read_mathml( _pm );
				}

				if( _cm_line )
				{
					MMLContent_TT_MSupSub item2insert( QString( FBL_TAG_msub ) );
					CFormulaNode *_node = _l->InsertMMLFormulaNode( item2insert.getRoot() );
					if( _node )
					{
						CLineNode *_lineParent = _node->getFormulaLineChild( 0 );
						CLineNode *_lineBase = _node->getFormulaLineChild( 1 );
						if( _lineParent != 0 && _lineBase != 0 )
						{
							_lineParent->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("|") );
							_lineBase->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("C") );
						}
						_node->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
					}
					_l->PumpLine2Line( _cm_line );
				}
			}		
		}
	}
	
/*	if( _pm )
	{
		_addon->PumpLine2Line( _node1st_line );
		_addon->PumpLine2Line( _cm_line );
		for( i = 0; i < _to_addon.size(); i++ )
			_addon->read_mathml( _to_addon[ i ] );

		_row.set( _pm );
		retnode = InsertMMLFormulaNode( _row.getRoot() );
		if( retnode )
			retnode->to_mathml_data.is_content = 
				CONTENT_MATHML_CONVERTION2__SEMANTICS_PRESENTATION_SUBSTITUTE;
	}
	else
	{
		if( _cm_line ) _addon->PumpLine2Line( _cm_line );
		for( i = 0; i < _to_addon.size(); i++ )
			_addon->read_mathml( _to_addon[ i ] );

		//_row.set( _node1st );
		retnode = InsertMMLFormulaNode( _row.getRoot() );
		if( retnode )
		{
			CLineNode *_l = retnode->getFormulaLineChild( 0 );
			if( _l ) _l->PumpLine2Line( _node1st_line );
			retnode->to_mathml_data.is_content = 
				CONTENT_MATHML_CONVERTION2__SEMANTICS_LITERALLY;
		}
	}
*/	
	_row.reset();
	if( retnode )
	{
		retnode->setAddonNode( _addon );
		_CM_basic_Node_property( retnode, xml_node );
		retnode->to_mathml_data.precedence = (short) precedence;
		retnode->to_mathml_data.assoc = assoc;
	}

	if( _cm_line ) delete _cm_line;
	if( _node1st_line ) delete _node1st_line;

	return retnode;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode *CLineNode::InsertMML_CM_piecewise( 
        enum TAG2MML /*tag_id*/, AST_TXML_Tag *xml_node )
{
	if( !xml_node || !xml_node->getTags() || !xml_node->isExistTagChild() )
		return 0;
	xml_node->deleteValueChildren();

	MMLContent_TT_mrow _row;
	CFormulaNode *retnode = InsertMMLFormulaNode_OTHERS_byID( HF_BTNID_DEFAULT_BRACKET_LF, _row.getRoot() );
	if( !retnode )
		return retnode;

	CLineNode* _lineNode = retnode->getFormulaLineChild( 0, 0 );
	if( !_lineNode )
		return retnode;

	std::vector<QString> attrSet;
	attrSet.push_back( FBL_ATTR_columnalign );
	attrSet.push_back( FBL_VAL_left );
	/*static*/ MMLContent_TT_Vector vec2insert( &attrSet );

	long other_idx = -1, cb = 0;
	std::vector<AST_TXML_Tag*> _nArray;
	AST_TXML_Tag *_n = 0;
	for( long i = 0; i < xml_node->getTags()->getChildNumber(); i++ )
	{
		_n = (AST_TXML_Tag*)(xml_node->getTags()->getChild( i ));
		if( _n->getTagName() == FBL_TAG_piece )
		{
			_nArray.push_back( _n );
			cb++;
		}
		else if( _n->getTagName() == FBL_TAG_otherwise )
			other_idx = i;
	}
	if( other_idx != -1 )
	{
		_n = (AST_TXML_Tag*)(xml_node->getTags()->getChild( other_idx ));
		_nArray.push_back( _n );
		cb++;
	}

	vec2insert.set_2columns( _nArray );
	CFormulaNode *_vecnode = _lineNode->InsertMMLFormulaNode( vec2insert.getRoot() );
	vec2insert.reset();
	if( !_vecnode || _vecnode->GetChildCount() != (long) (_nArray.size() << 1) )
		return retnode;
    
	for( long i = 0; i < (long)(_vecnode->GetChildCount() >> 1) && i < (long)_nArray.size(); i++ )
	{
		CNode *_frmNode = _vecnode->GetChild( i << 1 );
		if( _frmNode == 0 || _frmNode->GetType() != NODE_FRAME || 
			((CFrameNode*)_frmNode)->GetFirstChild() == 0 || 
			((CFrameNode*)_frmNode)->GetFirstChild()->GetType() != NODE_LINE ) continue;
		CLineNode *_line = (CLineNode*) (((CFrameNode*)_frmNode)->GetFirstChild());
		_line->read_mathml( _nArray[ i ] );
	}

	_CM_basic_Node_property( retnode, xml_node );
	retnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	retnode->to_mathml_data.precedence = mmlPrecedence_apply;
	retnode->to_mathml_data.assoc = 0;
	_vecnode->to_mathml_data.name = _T("");
	_vecnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;

	return retnode;
}

CFormulaNode* CLineNode::InsertMML_CM_standalone_piece( enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	if( !xml_node || !xml_node->getTags() || !xml_node->isExistTagChild() )
		return 0;
	xml_node->deleteValueChildren();

	MMLContent_TT_mrow _row;
	CFormulaNode *_node = InsertMMLFormulaNode( _row.getRoot() );
	_row.reset();
	if( !_node ) return _node;
	CLineNode *_line = _node->getFormulaLineChild( 0 );
	if( !_line ) return _node;

	if( xml_node->getTags()->getChild( 0 ) )
	{
		_line->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("&ThickSpace;") );
		_line->read_mathml( 
			(AST_TXML_Tag*)(xml_node->getTags()->getChild( 0 )) );
	}

	if( GetParent() == 0 || GetParent()->GetType() != NODE_FRAME ||
		GetParent()->GetNext() == 0 || GetParent()->GetNext()->GetType() != NODE_FRAME )
		return _node;
	CFrameNode *_rightFrame = (CFrameNode*) GetParent()->GetNext();
	if( _rightFrame->GetFirstChild() == 0 || _rightFrame->GetFirstChild()->GetType() != NODE_LINE )
		return _node;

	CLineNode *_line2 = (CLineNode*) _rightFrame->GetFirstChild();
	MMLContent_TT_mrow _row2;
	CFormulaNode *_node2 = _line2->InsertMMLFormulaNode( _row2.getRoot() );
	_row2.reset();
	if( !_node2 ) return _node;
	_line2 = _node2->getFormulaLineChild( 0 );
	if( !_line2 ) return _node;

/*
	QString iif = tag_id == TAG2MML_piece ? _T("") : _T("otherwise");
	// _line2->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, _T("&emsp;") );
	_line2->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mo, iif );
*/
	if( tag_id == TAG2MML_otherwise )
		_line2->InsertMML_CM_create_markup_rigid_token( FBL_TAG_mtext, _T("otherwise") );
	if( tag_id == TAG2MML_piece && xml_node->getTags()->getChild( 1 ) )
	{
		_line2->read_mathml( 
			(AST_TXML_Tag*)(xml_node->getTags()->getChild( 1 )) );
	}

	_CM_basic_Node_property( _node, xml_node );
	_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART1;
	_node->to_mathml_data.precedence = mmlPrecedence_Last;
	_node->to_mathml_data.assoc = 0;

	_CM_basic_Node_property( _node2, xml_node );
	_node2->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE_PART2;
	_node2->to_mathml_data.precedence = mmlPrecedence_Last;
	_node2->to_mathml_data.assoc = 0;

	return _node;
}

CPlaneText* CLineNode::InsertMML_CM_standalone_opaque( 
        enum TAG2MML /*tag_id*/, AST_TXML_Tag *xml_node )
{
	if( !isHidden() ) return 0;

	static MMLContent_TT_Token _token_xml_tree;
	static std::vector<QString> attr;
	_token_xml_tree.set( _T("FBL_TAG_mi"), _T("z"), &attr );
	std::vector<CPlaneText*> nodes_dump;
	InsertMMLTokenNode( _token_xml_tree.getRoot(), &nodes_dump );
	_token_xml_tree.reset();

	if( nodes_dump.size() == 1 )
		_CM_tune_Node_property( nodes_dump[ 0 ], xml_node );
	else
	{
		Q_ASSERT( 0 );
		return 0;
	}

	return nodes_dump[ 0 ];
}

CFormulaNode* CLineNode::InsertMML_CM_standalone_transparent( 
        enum TAG2MML /*tag_id*/, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_mrow _mrow_xml_tree;

	if( !xml_node->getTags() || !xml_node->isExistTagChild() )
		return 0;

	xml_node->deleteValueChildren();

	_mrow_xml_tree.set( xml_node->getTags() );
	CFormulaNode *_node = InsertMMLFormulaNode( _mrow_xml_tree.getRoot() );
	_mrow_xml_tree.reset();

	if( _node )
	{
		_CM_basic_Node_property( _node, xml_node );
		_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
		if( _node ) _node->setPrecedenceAsFirstChild();
	}

	return _node;
}

CPlaneText* CLineNode::InsertMML_CM_standalone_operator( enum TAG2MML tag_id, AST_TXML_Tag *xml_node, long newfstyle )
{
	/*static*/ MMLContent_TT_Token _token_xml_tree;
	QString newtag = _T(""), newtext = _T("");
	std::vector<QString> attr;

	int ret = ::mml_get_CM2PM_simple( tag_id, xml_node, newtag, newtext, attr );
	if( !ret ) return 0;

	_token_xml_tree.set( newtag, newtext, &attr );
	std::vector<CPlaneText*> nodes_dump;
	InsertMMLTokenNode( _token_xml_tree.getRoot(), &nodes_dump );
	_token_xml_tree.reset();

	if( nodes_dump.size() == 1 )
		_CM_tune_Node_property( nodes_dump[ 0 ], xml_node, newfstyle );
	else
	{
		Q_ASSERT( 0 );
		return 0;
	}

	return nodes_dump[ 0 ];
}

CFormulaNode* CLineNode::InsertMML_CM_standalone_operator_msup( enum TAG2MML tag_id, AST_TXML_Tag *xml_node )
{
	/*static*/ MMLContent_TT_MSup _msup_xml_tree;
	QString szBody = _T(""), szBase = _T("");
	if( ::getSupContentByOperatorTagID( tag_id, szBody, szBase ) == -1 )
		return 0;
	_msup_xml_tree.setParent( szBody );
	_msup_xml_tree.setBase( szBase );
	CFormulaNode *newnode = InsertMMLFormulaNode( _msup_xml_tree.getRoot() );
	_msup_xml_tree.reset();
	if( newnode )
	{
		newnode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		newnode->clearReadOnly();
		newnode->clearIgnoreContentConversion();
		_CM_tune_Node_property( newnode, xml_node );
	}

	return newnode;
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::_CM_tune_Node_property( CNode *_node, AST_TXML_Tag *xml_node, long newfstyle )
{
	_CM_basic_Node_property( _node, xml_node );

	_node->setCannotBeEdited();
	_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__SELF_CLOSING;
	if( _node->GetType() == NODE_PLANETEXT && newfstyle != FSTYLE_NONE )
	{
		CPlaneText *txt_node = 0;
		txt_node = (CPlaneText*)_node;
		long fstyle = txt_node->getFStyle();
		if( fstyle != FSTYLE_OTHER_STYLE && fstyle < FSTYLE_DEFAULT_FONT_NUMBER )
			txt_node->setFStyle( newfstyle, txt_node->getOtherStyleFont() );
	}
}

void CLineNode::_CM_basic_Node_property( CNode *_node, AST_TXML_Tag *xml_node )
{
	_node->to_mathml_data.reserved = _node->to_mathml_data.name;
	_node->to_mathml_data.name = xml_node->getTagName();
	_node->readUnfilteredAttr( xml_node );
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::InsertMML_CM_create_markup_rigid_token( QString tag, QString name )
{
	std::vector<QString> attr;
	/*static*/ MMLContent_TT_Token _token;
	std::vector<CPlaneText*> nodes_dump;
	CPlaneText *txt_node = 0;

	_token.set( tag, name, &attr );

	InsertMMLTokenNode( _token.getRoot(), &nodes_dump );
	if( nodes_dump.size() == 1 ) txt_node = nodes_dump[ 0 ];
	if( txt_node )
	{
		txt_node->setReadOnly();
		txt_node->setIgnoreContentConversion();
	}

	_token.reset();
}

void setRecursiveLevel( CNode* node, void* arg )
{
	if( node && arg )
	{
		node->SetLevel( *(long*)arg );
		if( node->GetType() == NODE_FORMULA )
		{
			((CFormulaNode*) node)->GetSlot( 0 ).SetLevel( *(long*)arg );
		}
	}
}

void setIgnoreContent( CNode* node, void* /*arg*/ )
{
	if( node )
	{
		node->setIgnoreContentConversion();
	}
}

void setRecursiveReadOnly_n_IgnoreContent( CNode* node, void* /*arg*/ )
{
	if( node )
	{
		node->setReadOnly();
		node->setIgnoreContentConversion();
	}
}

void clearRecursiveReadOnly_n_IgnoreContent( CNode* node, void* /*arg*/ )
{
	if( node )
	{
		node->clearReadOnly();
		node->clearIgnoreContentConversion();
	}
}

void setRecursiveReadOnly( CNode* node, void* /*arg*/ )
{
	if( node ) node->setReadOnly();
}

void clearRecursiveReadOnly( CNode* node, void* /*arg*/ )
{
	if( node ) node->clearReadOnly();
}

void setRecursiveCannotEdit( CNode* node, void* /*arg*/ )
{
	if( node ) node->setCannotBeEdited();
}

void clearRecursiveCannotEdit( CNode* node, void* /*arg*/ )
{
	if( node ) node->clearCannotBeEdited();
}

void setRecursiveImmovable( CNode* node, void* /*arg*/ )
{
	if( node ) node->setCannotBeMoved();
}

void clearRecursiveImmovable( CNode* node, void* /*arg*/ )
{
	if( node ) node->clearCannotBeMoved();
}

void setRecursiveIgnoreContent( CNode* node, void* /*arg*/ )
{
	if( node ) node->setIgnoreContentConversion();
}

void clearRecursiveIgnoreContent( CNode* node, void* /*arg*/ )
{
	if( node ) node->clearIgnoreContentConversion();
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::InsertMML_CM_wrap_into_brackets()
{
	CLineNode *_this_line = this;
	CLineNode *newLine = new CLineNode();

	MMLContent_TT_mfenced fence;
	CFormulaNode *_node = newLine->InsertMMLFormulaNode_OTHERS_byID( HF_BTNID_DEFAULT_BRACKET_LR, fence.getRoot() );
	Q_ASSERT( _node != 0 );
	if( _node == 0 ) return;	// internal error

	_node->to_mathml_data.name = _T("");
	_node->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	if( _node->GetFirstChild() )
		_node->GetFirstChild()->iterateNodeTree( setIgnoreContent, 0 );
	newLine->setReadOnlyAttribute4AllNonEmptyNodes();

	CLineNode *_line = _node->getFormulaLineChild( 0 );
	Q_ASSERT( _line != 0 );
	if( _line == 0 ) return;
	_line->PumpLine2Line( _this_line );
	_line->setReadOnly();

	_this_line->PumpLine2Line( newLine );
	delete newLine;

	_this_line->to_mathml_data.precedence = mmlPrecedence_apply;
	_this_line->to_mathml_data.assoc = 0;
	if( _this_line->GetParent() )
		_this_line->GetParent()->setPrecedenceAsFirstChild();
}

void CLineNode::InsertMML_CM_remove_outof_brackets( CLineNode *bracketed_line )
{
	if( !bracketed_line ) return;
	CLineNode *_this_line = this;

	CLineNode *newLine = new CLineNode();
	newLine->PumpLine2Line( bracketed_line );

	CLineNode *newLine2 = new CLineNode();
	newLine2->PumpLine2Line( _this_line );
	delete newLine2;

	_this_line->PumpLine2Line( newLine );
	delete newLine;
}

void CFormulaNode::getPrecedence4Autodetect( short& precedence, short& assoc )
{
	if( !isAutodetectContent() )
	{
		precedence = to_mathml_data.precedence;
		assoc = to_mathml_data.assoc;
		return;
	}

	CLineNode *_this_line = getContentFormulaSingleLineChild();
	if( !_this_line )
	{
		precedence = to_mathml_data.precedence;
		assoc = to_mathml_data.assoc;
		return;
	}

	if( _this_line->GetChildCount() == 1 && _this_line->GetFirstChild() )
	{
		precedence = _this_line->GetFirstChild()->to_mathml_data.precedence;
		assoc = _this_line->GetFirstChild()->to_mathml_data.assoc;
		return;
	}

	precedence = mmlPrecedence_Atom;
	assoc = 0;
}

///////////////////////////////////////////////////////////////////////////////

CLineNode* CFormulaNode::CM_WrapIntoBracketsOnPrecedence()
{
	return CM_WrapIntoBracketsOnPrecedence( -1 /*don't know precedence*/, 0 /*don't know association*/, 1 /*I guess should warp into brackets*/, 0 /*don't know position*/);
}

CLineNode* CFormulaNode::CM_WrapIntoBracketsOnPrecedence( short parent_precedence, short parent_assoc, int bNeedWrap, short position )
{
	if( !GetParent() ) return 0;
	CLineNode *_this_line = getContentFormulaSingleLineChild();

	if( parent_precedence == -1 )
	{
		long childIndexOfRequest = -1;
		CFormulaNode *workFormula = passAutodetectParent( NULL );
		CFormulaNode *parentFormula = workFormula->GetParent()->getParentFormulaNode( 0, &childIndexOfRequest );

		if( parentFormula && parentFormula->GetParent() &&
			parentFormula->to_mathml_data.is_content && parentFormula->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
		{
			workFormula = parentFormula->passAutodetectParent( NULL );
			parentFormula = workFormula->GetParent()->getParentFormulaNode( 0, &childIndexOfRequest );
		}

		if( parentFormula && parentFormula->GetParent() &&
			parentFormula->to_mathml_data.is_content && ::mml_is_cm_apply( parentFormula->to_mathml_data.name ) )
		{
			parent_precedence = parentFormula->to_mathml_data.precedence;
			parent_assoc = parentFormula->to_mathml_data.assoc;
			if( workFormula->GetParent()->GetChildCount() > 1 )
			{
				if( workFormula == workFormula->GetParent()->GetLastChild() )
					position = 1;
				else if( workFormula == workFormula->GetParent()->GetFirstChild() )
					position = -1;
			}
			if( !shouldBracket4Apply( parentFormula->getCalcButtonID(), childIndexOfRequest ) )
				bNeedWrap = 0;
		}
		else
		{
			parent_precedence = mmlPrecedence_Last;
		}
	}

	if( !_this_line ) return 0;

	short self_precedence, self_assoc;
	getPrecedence4Autodetect( self_precedence, self_assoc );
	if( to_mathml_data.is_content && getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
	{
		CFormulaNode *bracketsContents = (CFormulaNode*) getSingleFormulaChild();
		if( bracketsContents )
		{
			self_precedence =
				to_mathml_data.precedence = bracketsContents->to_mathml_data.precedence;
			to_mathml_data.assoc = bracketsContents->to_mathml_data.assoc;
		}
		else
			self_precedence = recalculatePrecedenceOnChildren();
		if( !bNeedWrap || parent_precedence >= self_precedence )
		{
			return _this_line;
		}
	}
	else
	{
		CLineNode *bracketedLineNode = 0, *parentBracketedLineNode = 0;
		CFormulaNode *bracketsApplyNode = (CFormulaNode*) getSingleContentFormulaChild();
		if( bracketsApplyNode && bracketsApplyNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
		{
			bracketedLineNode = bracketsApplyNode->getFormulaLineChild( 0 );
			parentBracketedLineNode = _this_line;
		}
		else
		{
			if( GetParent() && GetParent()->GetType() == NODE_LINE )
			{
				CLineNode *parentNode = (CLineNode*) GetParent();
				CNode *frmNode = parentNode->GetParent();
				if( parentNode->GetChildCount() == 1 && frmNode && (frmNode = frmNode->GetParent()) != 0 && frmNode->GetType() == NODE_FORMULA )
				{
					bracketsApplyNode = (CFormulaNode*) frmNode;
					if( bracketsApplyNode->GetChildCount() == 1 && bracketsApplyNode->to_mathml_data.is_content && 
						bracketsApplyNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR &&
						bracketsApplyNode->GetParent() && bracketsApplyNode->GetParent()->GetType() == NODE_LINE &&
						bracketsApplyNode->GetParent()->GetChildCount() == 1 )
					{
						bracketedLineNode = parentNode;
						parentBracketedLineNode = (CLineNode*) bracketsApplyNode->GetParent();
					}
				}
			}
		}

		bool shouldWrapByPrecAssoc = ::mml_shouldWrapByPrecAssoc( parent_precedence, self_precedence, parent_assoc, self_assoc, position );

		if( bracketedLineNode && parentBracketedLineNode )
		{
			if( !bNeedWrap || !shouldWrapByPrecAssoc )
			{
				parentBracketedLineNode->InsertMML_CM_remove_outof_brackets( bracketedLineNode );
			}
		}
		else
		{
			if( bNeedWrap && shouldWrapByPrecAssoc )
			{
				_this_line->InsertMML_CM_wrap_into_brackets();
			}
		}
	}
	return 0;
}

void CFormulaNode::CM_WrapChildrenIntoBracketsOnPrecedence( CFrameNode *frameLeft, CFrameNode *frameRight )
{
	if( to_mathml_data.is_content == 0 || !::mml_is_cm_apply( to_mathml_data.name ) ) return;
	int bNeedWrap = (getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP) ? 0 : 1;
	if( !frameLeft && !frameRight ) return;

	CFormulaNode *bracketsFormulaNode = 0;
	if( frameLeft && frameLeft->GetChildCount() == 1 && frameLeft->GetFirstChild() )
	{
		Q_ASSERT( frameLeft->GetFirstChild()->GetType() == NODE_LINE );
		CLineNode *_line = (CLineNode*)frameLeft->GetFirstChild();
		if( _line && _line->GetChildCount() == 1 && _line->GetFirstChild() && _line->GetFirstChild()->GetType() == NODE_FORMULA )
		{
			bracketsFormulaNode = (CFormulaNode*) _line->GetFirstChild();
			CLineNode *bracketed_line = bracketsFormulaNode ? bracketsFormulaNode->CM_WrapIntoBracketsOnPrecedence( to_mathml_data.precedence, to_mathml_data.assoc, bNeedWrap, -1 /*left*/ ) : 0;
			if( bracketed_line )
				_line->InsertMML_CM_remove_outof_brackets( bracketed_line/*bracketsFormulaNode*/ );
		}
	}
	if( frameRight && frameRight->GetChildCount() == 1 && frameRight->GetFirstChild() )
	{
		Q_ASSERT( frameRight->GetFirstChild()->GetType() == NODE_LINE );
		CLineNode *_line = (CLineNode*)frameRight->GetFirstChild();
		if( _line && _line->GetChildCount() == 1 && _line->GetFirstChild() && _line->GetFirstChild()->GetType() == NODE_FORMULA )
		{
			bracketsFormulaNode = (CFormulaNode*) _line->GetFirstChild();
			CLineNode *bracketed_line = bracketsFormulaNode ? bracketsFormulaNode->CM_WrapIntoBracketsOnPrecedence( to_mathml_data.precedence, to_mathml_data.assoc, bNeedWrap, 1 /*right*/ ) : 0;
			if( bracketed_line )
				_line->InsertMML_CM_remove_outof_brackets( bracketed_line/*bracketsFormulaNode*/ );
		}
	}
}

void CFormulaNode::CM_WrapChildrenIntoBracketsOnPrecedence()
{
	if( to_mathml_data.is_content == 0 || !::mml_is_cm_apply( to_mathml_data.name ) ) return;
	int bNeedWrap = (getCalcButtonID() == HF_BTNID_DEFAULT_DIVISION_SP) ? 0 : 1;

	CLineNode *_this_line = getContentFormulaSingleLineChild();
	if( !_this_line ) return;

	CLineNode *parentLine = _this_line;
	CFormulaNode *bracketsFormulaNode = 0;

	CNode *pNode = parentLine->GetFirstChild();
	if( !pNode ) return;
	else if( pNode->GetType() == NODE_FORMULA && pNode->to_mathml_data.is_content &&
		((CFormulaNode*) pNode)->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
	{
		parentLine = ((CFormulaNode*) pNode)->getContentFormulaSingleLineChild();
		if( !parentLine ) return;
		pNode = parentLine->GetFirstChild();
	}
	for( long i = 0; pNode && i < parentLine->GetChildCount(); i++ )
	{
		if( pNode->GetType() == NODE_FORMULA )
		{
			CLineNode *childLine = ((CFormulaNode*) pNode)->getContentFormulaSingleLineChild();
			if( childLine && childLine->GetChildCount() == 1 && childLine->GetFirstChild() && 
				childLine->GetFirstChild()->GetType() == NODE_FORMULA )
			{
				bracketsFormulaNode = (CFormulaNode*) childLine->GetFirstChild();
				CLineNode *bracketed_line = bracketsFormulaNode ? bracketsFormulaNode->CM_WrapIntoBracketsOnPrecedence( to_mathml_data.precedence, to_mathml_data.assoc, bNeedWrap, i == 0 ? -1 : 1 /* left or right operand */ ) : 0;
				if( bracketed_line )
				{
					childLine->InsertMML_CM_remove_outof_brackets( bracketed_line/*bracketsFormulaNode*/ );
				}
			}
		}
		pNode = pNode->GetNext();
	}
}

///////////////////////////////////////////////////////////////////////////////
