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

#include "../../dictionary/op_map.h"
#include "../../mathml/mml_tag2mml.h"
#include "n_cm_treetemplate.h"
#include "../node.h"

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLContent_cn( AST_TXML_Tag *node )
{
	if( !node ) return 0;

	AST_TXML_Tag *cloneNode = (AST_TXML_Tag*)(node->Clone());
	CFormulaNode *newNode = 0;

	QString base = _T("");
	long typeidx = -1;
	::mml_get_cn_attr( cloneNode, base, typeidx );

	for( long i = 0; i < cloneNode->getChildNumber(); i++ )
	{
		AST_TXML_Value *v = cloneNode->getValueChild( i );
		if( !v ) continue;
		QString& txt2process = v->getText();
		static QString _minus = _T("minus;");
		for( long offs = 0; offs < txt2process.length(); offs += 1 + _minus.length() )
		{
			if( (offs = txt2process.indexOf('-', offs)) == -1 )
				break;
			txt2process[ (int) offs ] = QChar('&');
			txt2process.insert(offs + 1, _minus);
		}
		v->setText( txt2process );
	}

	switch( typeidx)
	{
	case CONTENT_MATHML_CN_TYPE_RATIONAL:
		newNode = InsertMMLContent_cn_rational( cloneNode, base );
		break;
	case CONTENT_MATHML_CN_TYPE_COMPLEX_C:
		newNode = InsertMMLContent_cn_complex_cartesian( cloneNode, base );
		break;
	case CONTENT_MATHML_CN_TYPE_COMPLEX_P:
		newNode = InsertMMLContent_cn_complex_polar( cloneNode, base );
		break;
	default:
		newNode = InsertMMLContent_cn_simple( cloneNode, base );
		break;
	}
	delete cloneNode;
	return newNode;
}

CFormulaNode* CLineNode::InsertMMLContent_ci( AST_TXML_Tag *node )
{
	if( !node /*|| !node->getTags()*/ ) return 0;

	QString oldname = node->getTagName();
	AST_TXML_Compound *oldattr = node->getAttributes();

	//=== pretend that we have <mi> token, not <ci>
	QString nm = node->getTagName();
	if( nm == FBL_TAG_csymbol )
		nm = FBL_TAG_mo;
	else
		nm = FBL_TAG_mi;

	AST_TXML_Compound *newciattr = 0;
	AST_TXML_Attribute *type_attr = node->getAttribute( FBL_ATTR_type );
	if( type_attr && node->getTagName() == FBL_TAG_ci &&
		(!::xml_strcmp( type_attr->getStringValue(), FBL_VAL_vector ) ||
		 !::xml_strcmp( type_attr->getStringValue(), FBL_VAL_matrix )) )
	{
		newciattr = new AST_TXML_Compound();
		newciattr->Add( new AST_TXML_Attribute( &mathvariantV, &boldV ) );
	}
	node->setAttributes( newciattr );
	node->setTagName( nm );

	AST_TXML_Compound *newtags = 0;
	if( node->isExistTagChild() )
	{
		newtags = node->getTags();
	}
	else
	{
		newtags = new AST_TXML_Compound();
		newtags->Add( node );
	}
	AST_TXML_Tag *mrow_node = new AST_TXML_Tag( &mrowV, 0, newtags );

	CFormulaNode *PM_node = InsertMMLFormulaNode( mrow_node );

	if( node->isExistTagChild() )
		mrow_node->setTags( 0 );
	else
		newtags->ReplaceChild( 0, 0 );
	delete mrow_node;
	mrow_node = 0;
	if( newciattr )
	{
		delete newciattr;
		newciattr = 0;
	}
	node->setTagName( oldname );
	node->setAttributes( oldattr );

	if( PM_node )
	{
		InsertMMLContent__read_attributes( 
			PM_node, node, oldname, node->isExistTagChild() ? 
								CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE :
								CONTENT_MATHML_CONVERTION2__ONLY_TEXT_BODY );
	}

	return PM_node;
}

CFormulaNode* CLineNode::InsertMMLContent_csymbol( AST_TXML_Tag *node )
{
	return InsertMMLContent_ci( node );
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::InsertMMLContent__read_attributes( 
					CNode *node, AST_TXML_Tag *xml_node, QString tagname, long is_content )
{
	node->to_mathml_data.reserved = node->to_mathml_data.name;
	node->to_mathml_data.name = tagname;
	node->to_mathml_data.is_content = (short) is_content;
	node->readUnfilteredAttr( xml_node );
}

int _cn_reduce2simpleform(AST_TXML_Tag *node, AST_TXML_Tag*& sep, 
						  long& i, int& isExistTagChild, long& is_content)
{
	if( !node || !node->getTags() ) return 0;

	AST_TXML_Tag *a = 0;
	AST_TXML_Compound *_tags = node->getTags();
	for( long j = 0; j < _tags->getChildNumber(); j++ )
	{
		if( _tags->getChild( j ) && 
			_tags->getChild( j )->getNodeType() == ASTT_TXML_Tag )
		{
			a = (AST_TXML_Tag*)(_tags->getChild( j ));
			if( !xml_strcmp( a->getTagName(), FBL_TAG_sep ) )
			{			
				sep = a;
				i = j;
			}
			else
				isExistTagChild = 1;
		}
	}

	if( isExistTagChild )
		is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
	else
		is_content = CONTENT_MATHML_CONVERTION2__ONLY_TEXT_BODY;

	if( sep == 0 || (i > 0 && i == _tags->getChildNumber() - 1) )
	{
		if( i >= 0 && i < _tags->getChildNumber() )
			node->deleteChild( i );
		return 1;
	}

	return 0;
}

void _cn_wrapPCDATA( AST_TXML_Tag *node, MMLContent_TT_Root *envelope, long nSepPos )
{
	if( node->getTags() && nSepPos >= 0 )
	{
		long j;
		node->putValue2Tag( FBL_TAG_mn );
		AST_TXML_Compound aa, bb;
		for( j = 0; j < nSepPos; j++ )
			aa.Add( node->getTags()->getChild( j ) );
		for( j = nSepPos + 1; j < node->getTags()->getChildNumber(); j++ )
			bb.Add( node->getTags()->getChild( j ) );
		envelope->set( nSepPos ? &aa : NULL, &bb );
		aa.detachChildren();
		bb.detachChildren();
	}
}

void _cn_setcontent_conv4base( CFormulaNode* PM_node )
{
	PM_node->to_mathml_data.reserved = PM_node->to_mathml_data.name;
	PM_node->to_mathml_data.name  = ::getCM_XMLTemplate_set_property( 
		0/*dest*/, 1/*src*/, FBL_VAL_base, FBL_VAL_integer );
	PM_node->to_mathml_data.is_content = 
		CONTENT_MATHML_CONVERTION2__PARSE_NAME_AND_RECURSIVE_CM_TREE;
	CNode *snode = PM_node->getFormulaDeepChild( 1 );
	if( snode ) snode->setIgnoreContentConversion();
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLContent_cn_simple( 
		AST_TXML_Tag *node, QString base )
{
	long is_content = 0;
	MMLContent_TT_mrow					_st_node_mrow;
	MMLContent_TT_cnBase 				_st_node_cnbase;

	is_content = node->isExistTagChild() ?  CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE :
											CONTENT_MATHML_CONVERTION2__ONLY_TEXT_BODY;
	node->putValue2Tag( FBL_TAG_mn );
	_st_node_mrow.set( node->getTags() );

	CFormulaNode *PM_node = 0;
	CNode *fnode = 0;
	if( ::mml_is_default_cn_base( base ) )
	{
		fnode = PM_node = InsertMMLFormulaNode( _st_node_mrow.getRoot() );
	}
	else
	{
		_st_node_cnbase.setParent( _st_node_mrow.getRoot() );
		_st_node_cnbase.setBase( base );
		PM_node = InsertMMLFormulaNode( _st_node_cnbase.getRoot() );
		if( PM_node )
		{
			::_cn_setcontent_conv4base( PM_node );
			fnode = PM_node->getFormulaDeepChild( 0 );
		}
		_st_node_cnbase.reset();
	}
	if( fnode ) InsertMMLContent__read_attributes( fnode, node, FBL_TAG_cn, is_content );
	_st_node_mrow.reset();
	return PM_node;
}

//////////////////////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::InsertMMLContent_cn_rational( 
		AST_TXML_Tag *node, QString base )
{
	long is_content = 0, i = -1;
	int isExistTagChild = 0;
	AST_TXML_Tag* sep = 0;
	MMLContent_TT_cnBase 				_st_node_cnbase;
	MMLContent_TT_ciRational			_st_node_rational;

	if( ::_cn_reduce2simpleform( node, sep, i, isExistTagChild, is_content ) )
		return InsertMMLContent_cn_simple( node, base );

	if( i == 0 && sep && node->getTags() /*&& node->getTags()->getChildNumber() > 1*/ )
	{
		node->getTags()->InsertChild( 0, new AST_TXML_Value( &_emptyV ) );
		i++;
	}

	//=== wrap raw PCDATA in <mn> and the resultant tree in <...>
	::_cn_wrapPCDATA( node, &_st_node_rational, i );

	CFormulaNode *PM_node = 0, *fnode = 0;
	if( ::mml_is_default_cn_base( base ) )
	{
		fnode = PM_node = InsertMMLFormulaNode( _st_node_rational.getRoot() );
	}
	else
	{
		_st_node_cnbase.setParent( _st_node_rational );
		_st_node_cnbase.setBase( base );
		PM_node = InsertMMLFormulaNode( _st_node_cnbase.getRoot() );
		if( PM_node )
		{
			::_cn_setcontent_conv4base( PM_node );
			fnode = PM_node->getFormulaDeepFormulaChild( 0 );
		}
		_st_node_cnbase.reset();
	}

	if( fnode )	// mfrac
	{
		InsertMMLContent__read_attributes( fnode, node, FBL_TAG_cn, is_content );
		CFormulaNode *num = fnode->getFormulaDeepFormulaChild( 0 );
		CFormulaNode *denum = fnode->getFormulaDeepFormulaChild( 1 );
		if( num )
		{
			num->to_mathml_data.name = _T("");
			num->to_mathml_data.right = ::getCM_sep();
			num->to_mathml_data.is_content = (short) is_content;
		}
		if( denum )
		{
			denum->to_mathml_data.name = _T("");
			denum->to_mathml_data.is_content = (short) is_content;
		}
	} 

	_st_node_rational.reset();

	return PM_node;
}

CFormulaNode* CLineNode::InsertMMLContent_cn_complex_cartesian( 
		AST_TXML_Tag *node, QString base )
{
	long is_content = 0, i = -1;
	int isExistTagChild = 0;
	AST_TXML_Tag* sep = 0;
	MMLContent_TT_cnBase 					_st_node_cnbase;
	MMLContent_TT_ciComplexCartesian		_st_node_complex_c;

	if( ::_cn_reduce2simpleform( node, sep, i, isExistTagChild, is_content ) )
		return InsertMMLContent_cn_simple( node, base );

	if( i == 0 && sep && node->getTags() && node->getTags()->getChildNumber() == 1 )
	{	//=== if numenator is absent, /*but denumenator is present*/ assign numenator to 1
		node->getTags()->InsertChild( 0, new AST_TXML_Value( &_emptyV ) );
		i++;
	}

	//=== wrap raw PCDATA in <mn> and the resultant tree in <...>
	::_cn_wrapPCDATA( node, &_st_node_complex_c, i );

	CFormulaNode *PM_node = 0, *fnode = 0;
	if( ::mml_is_default_cn_base( base ) )
	{
		fnode = PM_node = InsertMMLFormulaNode( _st_node_complex_c.getRoot() );
	}
	else
	{
		_st_node_cnbase.setParent( _st_node_complex_c );
		_st_node_cnbase.setBase( base );
		PM_node = InsertMMLFormulaNode( _st_node_cnbase.getRoot() );
		if( PM_node )
		{
			::_cn_setcontent_conv4base( PM_node );
			fnode = PM_node->getFormulaDeepFormulaChild( 0 );	// mfenced
			if( fnode )
			{
				fnode->to_mathml_data.reserved = fnode->to_mathml_data.name;
				fnode->to_mathml_data.name = _T("");
				fnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				fnode = fnode->getFormulaDeepFormulaChild( 0 );
			}
		}
		_st_node_cnbase.reset();
	}

	if( fnode )	// mrow
	{
		fnode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		fnode->clearReadOnly();
		fnode->clearIgnoreContentConversion();
		InsertMMLContent__read_attributes( fnode, node, FBL_TAG_cn, is_content );

		CFormulaNode *re = 0;
		CFormulaNode *im = 0;
		if( i == 0 )
		{
			re = 0;
			im = fnode->getFormulaDeepFormulaChild( 0, 2 );
		}
		else
		{
			re = fnode->getFormulaDeepFormulaChild( 0 );
			im = fnode->getFormulaDeepFormulaChild( 0, 4 );
		}
		if( re )
		{
			re->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
			//re->clearReadOnly();
			re->to_mathml_data.name = _T("");
			re->to_mathml_data.right = ::getCM_sep();
			re->to_mathml_data.is_content = (short) is_content;
		}
		if( im )
		{
			im->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
			//im->clearReadOnly();
			im->to_mathml_data.name = _T("");
			if( !re ) im->to_mathml_data.left = ::getCM_sep();
			im->to_mathml_data.is_content = (short) is_content;
		}
	}

	_st_node_complex_c.reset();

	return PM_node;
}

CFormulaNode* CLineNode::InsertMMLContent_cn_complex_polar( 
		AST_TXML_Tag *node, QString base )
{
	//AST_TXML_Compound *_tags = node->getTags();
	long is_content = 0, i = -1;
	int isExistTagChild = 0;
	AST_TXML_Tag* sep = 0;
	MMLContent_TT_cnBase 				_st_node_cnbase;
	MMLContent_TT_ciComplexPolar		_st_node_complex_p;

	if( ::_cn_reduce2simpleform( node, sep, i, isExistTagChild, is_content ) )
		return InsertMMLContent_cn_simple( node, base );

	if( i == 0 && sep && node->getTags() && node->getTags()->getChildNumber() == 1 )
	{
		node->getTags()->InsertChild( 0, new AST_TXML_Value( &_emptyV ) );
		i++;
	}

	//=== wrap raw PCDATA in <mn> and the resultant tree in <...>
	::_cn_wrapPCDATA( node, &_st_node_complex_p, i );

	CFormulaNode *PM_node = 0, *fnode = 0;
	if( ::mml_is_default_cn_base( base ) )
	{
		fnode = PM_node = InsertMMLFormulaNode( _st_node_complex_p.getRoot() );
	}
	else
	{
		_st_node_cnbase.setParent( _st_node_complex_p );
		_st_node_cnbase.setBase( base );
		PM_node = InsertMMLFormulaNode( _st_node_cnbase.getRoot() );
		if( PM_node )
		{
			::_cn_setcontent_conv4base( PM_node );
			fnode = PM_node->getFormulaDeepFormulaChild( 0 );	// mfenced
			if( fnode )
			{
				fnode->to_mathml_data.reserved = fnode->to_mathml_data.name;
				fnode->to_mathml_data.name = _T("");
				fnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				fnode = fnode->getFormulaDeepFormulaChild( 0 );
			}
		}
		_st_node_cnbase.reset();
	}

	if( fnode )
	{
		fnode->iterateNodeTree( setRecursiveReadOnly_n_IgnoreContent, 0 );
		fnode->clearReadOnly();
		fnode->clearIgnoreContentConversion();

		InsertMMLContent__read_attributes( fnode, node, FBL_TAG_cn, is_content );

		CFormulaNode *re = 0, *im = 0;
		if( i == 0 )
		{
			re = 0;
			fnode = fnode->getFormulaDeepFormulaChild( 0 );
		}
		else
		{
			re = fnode->getFormulaDeepFormulaChild( 0 );
			fnode = fnode->getFormulaDeepFormulaChild( 0, 2 );
		}

		if( fnode )
		{
			fnode->to_mathml_data.reserved = fnode->to_mathml_data.name;
			fnode->to_mathml_data.name = _T("");
			fnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
			fnode->clearIgnoreContentConversion();

			fnode = fnode->getFormulaDeepFormulaChild( 1 );
			if( fnode )
			{
				fnode->to_mathml_data.reserved = fnode->to_mathml_data.name;
				fnode->to_mathml_data.name = _T("");
				fnode->to_mathml_data.is_content = CONTENT_MATHML_CONVERTION2__RECURSIVE_TREE;
				fnode->clearIgnoreContentConversion();
				im = fnode->getFormulaDeepFormulaChild( 0, 2 );
			}
		}

		if( re )
		{
			re->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
			//re->clearReadOnly();
			re->to_mathml_data.name = _T("");
			re->to_mathml_data.right = ::getCM_sep();
			re->to_mathml_data.is_content = (short) is_content;
		}
		if( im )
		{
			im->iterateNodeTree( clearRecursiveReadOnly_n_IgnoreContent, 0 );
			//im->clearReadOnly();
			im->to_mathml_data.name = _T("");
			if( !re ) im->to_mathml_data.left = ::getCM_sep();
			im->to_mathml_data.is_content = (short) is_content;
		}
	}

	_st_node_complex_p.reset();

	return PM_node;
}

///////////////////////////////////////////////////////////////////////////////
