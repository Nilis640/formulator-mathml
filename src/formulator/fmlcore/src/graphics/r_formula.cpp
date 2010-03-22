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

#include "baseratio.h"
#include "../nodes/node.h"
#include "../nodes/dump/dumptags.h"

//////////////////////////////////////////////////////////////////////

CGraphPrimFormula::CGraphPrimFormula( void ) 
{
	m_color = DEFAULT_GRAPH_COLOR;
	m_width = DEFAULT_GRAPH_WIDTH;
	m_style = DEFAULT_GRAPH_STYLE;
	m_height = 0.0;
	m_bind_no = -1;
	m_is_formula = 0;
	m_type = -1;
	m_expr_enable = 0;
	m_align = 0;
	m_size_type = 0;
	m_str = "";
	m_stretchy = 0;
	m_offset_direction_x = 0.0;
	m_offset_direction_y = 0.0;
}

CGraphPrimFormula::CGraphPrimFormula( const CGraphPrimFormula& a )
{
	m_type = a.m_type;
	m_pecents = a.m_pecents;
	m_color = a.m_color;
	m_width = a.m_width;
	m_style = a.m_style;
	m_str = a.m_str;
	m_size_type = a.m_size_type;
	m_height = a.m_height;
	m_stretchy = a.m_stretchy;
	m_align = a.m_align;
	m_bind_no = a.m_bind_no;
	m_expr_enable = a.m_expr_enable;
	m_is_formula = a.m_is_formula;
	m_point_type = a.m_point_type;
	m_point_xy = a.m_point_xy;
	m_offset_direction_x = a.m_offset_direction_x;
	m_offset_direction_y = a.m_offset_direction_y;
}

CGraphPrimFormula::~CGraphPrimFormula()
{
}

/////////////////////////////////////////////////////////////////////

int CGraphPrimFormula::Create( const FBtnGraphics& a )
{
	m_type			= a.grType;
	m_pecents		= a.pos;
	m_color			= a.color;
	m_width			= a.width;
	m_style			= a.style;
	m_str			= a.text_reserve;
	m_size_type		= a.sizeType;
	m_height		= a.height;
	m_stretchy		= a.stretchy;
	m_align			= a.align;
	m_bind_no		= a.bind_to;
	m_expr_enable	= a.expr_enable;
	m_is_formula	= a.coord_type == COORD_TYPE_FORMULA;

	long count = a.point_type.size();
	m_point_type.resize( count );
	m_point_xy.resize( count );
	for( long i = 0; i < count; i++ )	
	{
		m_point_type[ i ] = a.point_type.at(i);
		m_point_xy[ i ].rx() = a.point_x.at(i);
		m_point_xy[ i ].ry() = a.point_y.at(i);
	}

	m_offset_direction_x = a.offset_direction_x;
	m_offset_direction_y = a.offset_direction_y;

	return 1;
}

//////////////////////////////////////////////////////////////////////

RectFde CGraphPrimFormula::GetRealPosition( CNode* node )
{
	if( !node || node->GetType() != NODE_FORMULA ) return RectFde();

	CFormulaNode* pFormulaNode = (CFormulaNode*) node;
	RectFde realPos;
	if( m_pecents.left() < 0 || m_pecents.top() < 0 || m_pecents.right() < 0 || m_pecents.bottom() < 0 )
	{
		bool correctLeft = false, correctTop = false;
		bool correctRight = false, correctBottom = false;
		if( m_pecents.left() < 0 )
		{
			correctLeft = true;
			m_pecents.setLeft( HF_BINBUTTON_VERTICAL_SCALE );
		}
		if( m_pecents.top() < 0 )
		{
			correctTop = true;
			m_pecents.setTop( HF_BINBUTTON_VERTICAL_SCALE );
		}
		if( m_pecents.right() < 0 )
		{
			correctRight = true;
			m_pecents.setRight( HF_BINBUTTON_VERTICAL_SCALE );
		}
		if( m_pecents.bottom() < 0 )
		{
			correctBottom = true;
			m_pecents.setBottom( HF_BINBUTTON_VERTICAL_SCALE );
		}
		realPos = pFormulaNode->GetRealPosition( m_pecents );
		if( correctLeft )
		{
			realPos.setLeft( realPos.left() - m_width );
			m_pecents.setLeft( -1 );
		}
		if( correctTop )
		{
			realPos.setTop( realPos.top() - m_width );
			m_pecents.setTop( -1 );
		}
		if( correctRight )
		{
			realPos.setRight( realPos.right() - m_width );
			m_pecents.setRight( -1 );
		}
		if( correctBottom )
		{
			realPos.setBottom( realPos.bottom() - m_width );
			m_pecents.setBottom( -1 );
		}
	}
	else
	{
		realPos = pFormulaNode->GetRealPosition( m_pecents );
	}

	if ( GetSizeType() == FIXED_SIZE )
	{
		RealFde stepH = pFormulaNode->GetDefaultSize().width() / HF_BINBUTTON_HORIZONTAL_SCALE;
		RealFde stepV = pFormulaNode->GetDefaultSize().height() / HF_BINBUTTON_VERTICAL_SCALE;
		SizeFde realSize( m_pecents.width() * stepH, m_pecents.height() * stepV );

		RealFde dx = (realPos.width() - realSize.width()) / 2.0;
		RealFde dy = (realPos.height() - realSize.height()) / 2.0;

		realPos.translate( dx, dy );
		realPos.setRight( realPos.left() + realSize.width() );
		realPos.setBottom( realPos.top() + realSize.height() );
	}
	return realPos;
}

QDataStream& operator << ( QDataStream& ar, const CGraphPrimFormula& lp )
{
	return ar << lp.m_color << lp.m_str << lp.m_pecents << lp.m_width << lp.m_height << lp.m_offset_direction_x << 
				 lp.m_offset_direction_y << lp.m_style << lp.m_bind_no << lp.m_is_formula << lp.m_type << 
				 lp.m_align << lp.m_size_type << lp.m_stretchy << lp.m_point_type << lp.m_point_xy << lp.m_expr_enable;
}

QDataStream& operator >> ( QDataStream& ar, CGraphPrimFormula& lp )
{
	return ar >> lp.m_color >> lp.m_str >> lp.m_pecents >> lp.m_width >> lp.m_height >> lp.m_offset_direction_x >> 
				 lp.m_offset_direction_y >> lp.m_style >> lp.m_bind_no >> lp.m_is_formula >> lp.m_type >> 
				 lp.m_align >> lp.m_size_type >> lp.m_stretchy >> lp.m_point_type >> lp.m_point_xy >> lp.m_expr_enable;
}

//////////////////////////////////////////////////////////////////////
