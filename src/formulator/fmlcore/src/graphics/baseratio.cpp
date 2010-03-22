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

#include <cstdlib>

#include "baseratio.h"
#include "../nodes/node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphPrim::CGraphPrim()
{
	m_color = DEFAULT_GRAPH_COLOR;
	m_width = DEFAULT_GRAPH_WIDTH;
	m_style = DEFAULT_GRAPH_STYLE;
	m_is_formula = 0;
	m_height = 0.0;
	m_type = -1;
	m_expr_enable = 0;
	SetLevel( FTEXT_HEIGHT_FRAME );
	m_stretchy = 0;
	m_align = 0;
	m_bind_no = -1;
	SetPosition( RectFde( PointFde(0, 0), PointFde(0, 0) ) );
}

CGraphPrim::CGraphPrim( const CGraphPrim& a )
{
	m_color = a.m_color;
	m_width = a.m_width;
	m_style = a.m_style;
	m_is_formula = a.m_is_formula;
	m_height = a.m_height;
	m_type = a.m_type;
	m_expr_enable = a.m_expr_enable;
	m_level = a.m_level;
	m_stretchy = a.m_stretchy;
	m_align = a.m_align;
	m_bind_no = a.m_bind_no;
	for(int i = 0; i < 4; i++)
		m_aRealPosition[ i ] = a.m_aRealPosition[ i ];
}

CGraphPrim::~CGraphPrim()
{
}

/////////////////////////////////////////////////////////////////////

CNode* CGraphPrim::getBoundFrame( CParentNode *parent )
{
	return (isBound() && parent) ? parent->GetChild( GetBindToFrame() - 1 ) : 0;
}

int CGraphPrim::Create( const CGraphPrimFormula& templatePrim )
{
	m_color = templatePrim.m_color;
	m_height = templatePrim.m_height;
	m_width = templatePrim.m_width;
	m_style = templatePrim.m_style;
	SetPosition( RectFde( PointFde(0, 0), PointFde(0, 0) ) );
	m_type = templatePrim.m_type;
	m_stretchy = templatePrim.m_stretchy;
	m_align = templatePrim.m_align;
	m_bind_no = templatePrim.m_bind_no;
	m_expr_enable = templatePrim.m_expr_enable;
	m_is_formula = templatePrim.m_is_formula;
	return 1;
}

void CGraphPrim::SetPosition( const RectFde& point )
{
	m_aRealPosition[ SVG_NodeTemplate_Parameter_Left ]		= point.left();
	m_aRealPosition[ SVG_NodeTemplate_Parameter_Top ]		= point.top();
	m_aRealPosition[ SVG_NodeTemplate_Parameter_Right ]		= point.right();
	m_aRealPosition[ SVG_NodeTemplate_Parameter_Bottom ]	= point.bottom();
}

RectFde CGraphPrim::GetPositionRect() const
{
	return RectFde( PointFde(m_aRealPosition[ SVG_NodeTemplate_Parameter_Left ], m_aRealPosition[ SVG_NodeTemplate_Parameter_Top ]),
					PointFde(m_aRealPosition[ SVG_NodeTemplate_Parameter_Right ],m_aRealPosition[ SVG_NodeTemplate_Parameter_Bottom ]) );
}

QDataStream& operator << ( QDataStream& ar, const CGraphPrim& lp )
{
	return ar << lp.m_aRealPosition[ 0 ] << lp.m_aRealPosition[ 1 ] << lp.m_aRealPosition[ 2 ] << lp.m_aRealPosition[ 3 ] << 
				 lp.m_height << lp.m_width << lp.m_is_formula << lp.m_bind_no << lp.m_align << lp.m_style << lp.m_type << 
				 lp.m_level << lp.m_stretchy << lp.m_color << lp.m_expr_enable;
}

QDataStream& operator >> ( QDataStream& ar, CGraphPrim& lp )
{
	return ar >> lp.m_aRealPosition[ 0 ] >> lp.m_aRealPosition[ 1 ] >> lp.m_aRealPosition[ 2 ] >> lp.m_aRealPosition[ 3 ] >> 
				 lp.m_height >> lp.m_width >> lp.m_is_formula >> lp.m_bind_no >> lp.m_align >> lp.m_style >> lp.m_type >> 
				 lp.m_level >> lp.m_stretchy >> lp.m_color >> lp.m_expr_enable;
}

//////////////////////////////////////////////////////////////////////
