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

/////////////////////////////////////////////////////////////////////

#include <vector>

#include "../node.h"
#include "../extcontent/ext_node.h"
#include "nodeinfo_property.h"

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const CNodeInfo_Property& lp )
{
	if( lp.getAddonNode() )
	{
		ar << (qint32) lp.getAddonNode()->GetType();
		ar << (qint32) (lp.getAddonNode()->isExtNode() ? 1 : 0);
		SelectNode sn;
		std::vector<SelectNode> vecV;
		lp.getAddonNode()->Store( ar, sn, vecV );
	}
	else
	{
		ar << (qint32) NODE_NONE;
		ar << (qint32) 0;
	}

	return ar << (const FBtnAttr&) lp;
}

QDataStream& operator >> ( QDataStream& ar, CNodeInfo_Property& lp )
{
	if( lp.m_markup_addon )
	{
		delete lp.m_markup_addon;
		lp.m_markup_addon = 0;
	}

	qint32 type = NODE_NONE;
	ar >> type;
	qint32 is_ext_node = 0;
	ar >> is_ext_node;

	switch( type )
	{
		case NODE_PLANETEXT:
			lp.m_markup_addon = (is_ext_node ? new CExtNode() : new CPlaneText());
			break;
		case NODE_LINE:
			lp.m_markup_addon = new CLineNode();
			break;
		case NODE_FRAME:
			lp.m_markup_addon = new CFrameNode();
			break;
		case NODE_FORMULA:
			lp.m_markup_addon = new CFormulaNode();
			break;
		default:
			break;
	}

	SelectNode sn;
	std::vector<SelectNode> toStoreEmpty;
	if( lp.getAddonNode() )
		lp.getAddonNode()->Load( ar, sn, toStoreEmpty, 0 );

	return ar >> (FBtnAttr&) lp;
}

/////////////////////////////////////////////////////////////////////

CNodeInfo_Property::CNodeInfo_Property( void )
{
	m_markup_addon = 0;
}

CNodeInfo_Property::~CNodeInfo_Property( void )
{
	if( m_markup_addon )
	{
		delete m_markup_addon;
		m_markup_addon = 0;
	}
}

const CNodeInfo_Property& CNodeInfo_Property::operator = ( const CNodeInfo_Property& v )
{
	(FBtnAttr&) (*this) = (const FBtnAttr&) v;
	if( m_markup_addon )
	{
		delete m_markup_addon;
		m_markup_addon = 0;
	}
	if( v.m_markup_addon )
	{
		switch( v.m_markup_addon->GetType() )
		{
		case NODE_PLANETEXT:
			m_markup_addon = v.m_markup_addon->isExtNode() ? 
				new CExtNode( (CExtNode*) v.m_markup_addon ) : new CPlaneText( (CPlaneText*) v.m_markup_addon );
			break;
		case NODE_LINE:
			m_markup_addon = new CLineNode( (CLineNode*) v.m_markup_addon );
			break;
		case NODE_FRAME:
			m_markup_addon = new CFrameNode( (CFrameNode*) v.m_markup_addon );
			break;
		case NODE_FORMULA:
			m_markup_addon = new CFormulaNode( (CFormulaNode*) v.m_markup_addon );
			break;
		default:
			break;
		}
	}
	return *this;
}

/////////////////////////////////////////////////////////////////////
