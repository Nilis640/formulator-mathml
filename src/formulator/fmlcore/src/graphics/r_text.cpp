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

#include <math.h>
#include "baseratio.h"
#include "../nodes/node.h"
#include "../style/style.h"
#include "../nodes/dump/dumptags.h"

//////////////////////////////////////////////////////////////////////

CGraphText::CGraphText()
	: CGraphPrim()
	, m_Str( _T("") )
	, m_txtSize( 0, 0 )
{
	m_height = FTEXT_HEIGHT_FRAME;
	m_style = FSTYLE_VARIABLE;
}

CGraphText::CGraphText( const CGraphText& a ) 
	: CGraphPrim( a )
	, m_Str( a.m_Str )
	, m_txtSize( a.m_txtSize )
{
}

CGraphText::~CGraphText()
{
}

/////////////////////////////////////////////////////////////////////

RealFde CGraphText::getFontHeight( void )
{
	RealFde kegl = (m_height == FTEXT_HEIGHT_FRAME ? ::getKegl( GetLevel() ) : GetHeight() - 1.0f);
	return ::getCurrentFormulatorStyle().kegl2pixel( kegl );
}

/////////////////////////////////////////////////////////////////////

RealFde CGraphText::GetVerticalCenter()
{
	FS_LogFont lfs( ::getFontStyle( GetStyle() ), getFontHeight() );

	RealFde asc = ::getCurrentDefaultAscent( GetLevel() );
	RealFde height = ::getCurrentDefaultSize( GetLevel() ).height();
	RealFde ascentThis = CFmlDrawEngineQt::GetAscentEx( lfs );

	return (ascentThis - asc) + height / 2.0;
}

/////////////////////////////////////////////////////////////////////

void CGraphText::setTxtSize()
{
	FS_LogFont lfs( ::getFontStyle( GetStyle() ), getFontHeight() );
	m_txtSize = CFmlDrawEngineQt::MeasureStringEx( GetString(), -1, lfs );
}

/////////////////////////////////////////////////////////////////////

int CGraphText::Create( const CGraphPrimFormula& templatePrim )
{
	if( !CGraphPrim::Create( templatePrim ) )
		return 0;

	m_Str = templatePrim.GetString();
	m_style = templatePrim.GetStyle();
	return 1;
}

/////////////////////////////////////////////////////////////////////

int CGraphText::Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *parent )
{
	if( parent->GetType() != NODE_FORMULA )
	{
		Q_ASSERT( 0 );
		return 0; // CPlainText can not contain any primitives of CGraphText type
	}

	CParentNode *pParentNode = (CParentNode*) parent;
	if( !canDraw( pParentNode ) ) return 0;

	// calculate rectangle to draw text
	RectFde objPosition( 0, 0, 0, 0 );
	GetObjPosition( parent, objPosition );
	objPosition.translate( tl.x(), tl.y() );

	// get font
	FS_LogFont lfs( ::getFontStyle( GetStyle() ), getFontHeight() );

	// default color
	lfs.m_color = (GetColor() == DEFAULT_GRAPH_COLOR ? ::getCurrentFormulatorStyle().getLogPen().m_color : GetColor());

	// draw text
	fde.DrawString( objPosition.left(), objPosition.top(), GetString(), lfs );

	return 1;
}

/////////////////////////////////////////////////////////////////////

void CGraphText::GetObjPosition( CNode *parent, RectFde& objPosition )
{
	CParentNode *pParentNode = (CParentNode*) parent;
	objPosition = RectFde(PointFde(0.0, 0.0), PointFde(0.0, 0.0));
	if( isBound() && pParentNode != NULL )
	{
		CNode *node = getBoundFrame( pParentNode );
		if( !node ) node = pParentNode;

		Q_ASSERT( m_txtSize.width() != 0 && m_txtSize.height() != 0 );

		switch( GetAlign() & HORISONTAL_MASK )
		{
		case TO_LEFT:
			objPosition.setRight( node->GetPosition().x() + node->getLeftIndent() );
			objPosition.setLeft( objPosition.right() - m_txtSize.width() );
			break;
		case TO_RIGHT:
			objPosition.setRight( node->GetPosition().x() + node->GetSize().width() );
			objPosition.setLeft( objPosition.right() - m_txtSize.width() );
			break;
		case HORISONTAL_CENTER:
			objPosition.setLeft( node->GetPosition().x() + (node->GetSize().width() - m_txtSize.width()) / 2.0 );
			objPosition.setRight( objPosition.left() + m_txtSize.width() );
			break;
		default:
			break;
		}
		switch( GetAlign() & VERTICAL_MASK )
		{
		case TO_TOP:
			objPosition.setBottom( node->GetPosition().y() + node->getTopIndent() );
			objPosition.setTop( objPosition.bottom() - m_txtSize.height() );
			break;
		case TO_BOTTOM:
			objPosition.setTop( node->GetPosition().y() + node->GetSize().height() );
			objPosition.setBottom( objPosition.top() + m_txtSize.height() );
			break;
		case VERTICAL_CENTER:
			objPosition.setTop( node->GetPosition().y() + node->GetVerticalCenter() - GetVerticalCenter() );
			objPosition.setBottom( objPosition.top() + m_txtSize.height() );
			break;
		default:
			break;
		}
	}
	else
		objPosition = GetPositionRect();
}

/////////////////////////////////////////////////////////////////////

int CGraphText::canDraw( CParentNode *parent )
{
	CNode *node = getBoundFrame( parent );
	if( node == NULL || !parent->isStretchy() ) return 1;

	RealFde text_size = fabs( getFontHeight() );
	RealFde height = node->GetSize().height() - node->getTopIndent()- node->getBottomIndent();
	return text_size >= height || fabs( height - text_size ) / height <= FORMULATOR_PLANETEXT_CMPVALUE;
}

/////////////////////////////////////////////////////////////////////

int CGraphText::correctParentNodeSize( CParentNode *parent )
{
	setTxtSize();
	CNode *node = getBoundFrame( parent );
	if( !node ) return 0;

	RealFde left, right, top, bottom, p;
	switch( GetAlign() & HORISONTAL_MASK )
	{
	case TO_LEFT:
		node->setLeftIndent( m_txtSize.width() );
		break;
	case TO_RIGHT:
		node->setRightIndent( m_txtSize.width() );
		break;
	case HORISONTAL_CENTER:
		p = node->GetSize().width() - node->getLeftIndent() - node->getRightIndent();
		left = node->GetPosition().x() + (node->GetSize().width() - m_txtSize.width()) / 2.0;
		right = left + m_txtSize.width();
		if( right - left > p )
		{
			if( left < node->GetPosition().x() )
				node->setLeftIndent( node->GetPosition().x() - left );
			if( right > node->GetPosition().x() + p )
				node->setRightIndent( right - (node->GetPosition().x() + p) );
		}
		else
		{
			node->setLeftIndent( 0.0 );
			node->setRightIndent( 0.0 );
		}
		break;
	default:
		break;
	}
	switch( GetAlign() & VERTICAL_MASK )
	{
	case TO_TOP:
		node->setTopIndent( m_txtSize.height() );
		break;
	case TO_BOTTOM:
		node->setBottomIndent( m_txtSize.height() );
		break;
	case VERTICAL_CENTER:
		p = node->GetSize().height() - node->getTopIndent() - node->getBottomIndent();

		top = node->GetPosition().y() + node->GetVerticalCenter() - GetVerticalCenter();
		bottom = top + m_txtSize.height() - node->getTopIndent();

		if( bottom - top > p )
		{
			if( top < node->GetPosition().y() )
			{
				top = node->GetPosition().y() - top;
				node->setTopIndent( top );
				bottom += top;
			}
			if( bottom > node->GetPosition().y() + p + node->getTopIndent() + node->getBottomIndent() )
			{
				node->setBottomIndent( bottom - (node->GetPosition().y() + p + node->getTopIndent() + node->getBottomIndent()) );
			}
		}
		else
		{
			node->setTopIndent( 0.0 );
			node->setBottomIndent( 0.0 );
		}
		break;
	default:
		break;
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const CGraphText& lp )
{
	return ar << (const CGraphPrim&) lp << lp.m_Str << lp.m_txtSize;
}

QDataStream& operator >> ( QDataStream& ar, CGraphText& lp )
{
	return ar >> (CGraphPrim&) lp >> lp.m_Str >> lp.m_txtSize;
}

//////////////////////////////////////////////////////////////////////
