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

#include "node.h"
#include "node2proplist.h"

///////////////////////////////////////////////////////////////////////////

QString convertAttrName2Key( QString tagName, QString attrName );

///////////////////////////////////////////////////////////////////////////

void CNode::setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports )
{
	props.reset();
	props = ::getFullMathMLAttrList();
	supports.clear();
	supports.resize( props.getParameterNumber(), 0 );
	::parseNodeAttr2PropList( props, supports, this );
	Q_ASSERT( supports.size() == props.getParameterNumber() );
}

void CPlaneText::setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports )
{
	CNode::setTextPropertyList( props, supports );
}

void CFormulaNode::setTextPropertyList( CNode_TextPropertyList& props, std::vector<char>& supports )
{
	CNode::setTextPropertyList( props, supports );
}

///////////////////////////////////////////////////////////////////////////

void parseNodeAttr2PropList( CNode_TextPropertyList& props, std::vector<char>& supports, CNode *frmNode )
{
	Q_ASSERT( frmNode != 0 );

	long level = 2, nodesCount = 0;
	QString mathmlRes = frmNode->to_mathml( level, nodesCount );

	parseNodeAttr2PropList( props, supports, mathmlRes );
}

void parseNodeAttr2PropList( CNode_TextPropertyList& props, std::vector<char>& supports, QString mathmlRes )
{
	long level = 2;
	mathmlRes = ::mml_tag_nl( QString(FBL_TAG_math), mathmlRes, QString( _T("") ), level );

	DRMessageManager dmm;
	long i, idx, dmm_page = 0;
	AST_TXML_Compound* bms = 0;
	if( LS_CompileXML( _T("(internal processing)"), mathmlRes, bms, dmm, dmm_page ) || !bms )
		return;

	AST_TXML_Tag* root = bms->getFirstTag( FBL_TAG_math );
	if( root == 0 || (root = root->getTagChild( 0 )) == 0 )
	{
		if( bms != 0 ) delete bms;
		return;
	}

	std::vector<long> attrIdx;
	getFullMathMLAttrList().getTag2AttrList( root->getTagName(), attrIdx );
	CNode_TextPropertyData *data = 0;
	for( i = 0; i < (long) attrIdx.size(); i++ )
	{
		if( (data = props.getParameter( attrIdx[ i ] )) == 0 )
			continue;

		if( data->m_typeCtrl == CNode_TextProperty_NONE )
			continue;

		supports[ attrIdx[ i ] ] = 1;
	}

	AST_TXML_Attribute *_attr = 0;
	QString xmlAttrN, xmlAttrV;
	const AST_TXML_Compound* xmlAttrList = root->getAttributes();
	CNode_TextPropertyData theprop( _T(""), _T(""), __Hint4UserAttr, CNode_TextProperty_TEXT, -1 );
	for( i = 0; i < xmlAttrList->getChildNumber(); i++ )
	{
		if( xmlAttrList->stmt.at(i) == NULL || 
			xmlAttrList->stmt.at(i)->getNodeType() != ASTT_TXML_Attribute )
			continue;
		_attr = (AST_TXML_Attribute*)(xmlAttrList->stmt[ i ]);

		theprop.m_name = _attr->getAttributeName();
		theprop.m_value = _attr->getStringValue();
		theprop.m_key = ::convertAttrName2Key( root->getTagName(), theprop.m_name );
		idx = props.getParameterIndex( theprop.m_key );
		data = (idx == -1) ? 0 : props.getParameter( idx );
		if( !data )
		{
			props.setParameter( theprop );
			supports.push_back( char(2) );
		}
		else
		{
			data->m_value = theprop.m_value;
			supports[ idx ] = 2;
		}

	}

	long opidx = ::getFullMathMLAttrList().getContentMMLOperatorProperty( root->getTagName() );
	if( opidx >= 0 && opidx < CNode_TextProperty_ContentOpArrLen )
	{
		if( (idx = props.getParameterIndex( TXTPROP_IKEY__OP )) != -1 )
		{
			props.getParameter( idx )->m_value = CNode_TextProperty_ContentOpArr[ opidx ];
			supports[ idx ] = 2;
		}
	}

	props.setXMLPtrs( bms, root );
}

///////////////////////////////////////////////////////////////////////////
