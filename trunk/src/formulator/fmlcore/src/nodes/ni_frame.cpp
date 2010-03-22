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
#include "dump/dumptags.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFrameNode::CFrameNode()
	: CParentNode( NODE_FRAME ) 
	, m_lineSpacing( DEFAULT_LINESPACING )
{
	AddChild( new CLineNode() );
}

CFrameNode::CFrameNode( CFrameNode* pFrameNode )
	: CParentNode( pFrameNode )
{
}

CFrameNode::~CFrameNode()
{
}

//////////////////////////////////////////////////////////////////////

void CFrameNode::SetLineSpacing( RealFde lineSpacing )
{
	if( lineSpacing <= 0.0 ) 
		lineSpacing = DEFAULT_LINESPACING;

	if( lineSpacing != m_lineSpacing )
	{
		m_lineSpacing = lineSpacing;

		CNode *pNode;
		for( long i = 0; i < GetChildCount(); i++ )
		{
			pNode = GetChild( i );
			if( pNode->GetType() == NODE_FRAME )
				((CFrameNode*) pNode)->SetLineSpacing( lineSpacing );
		}
	}
}

//////////////////////////////////////////////////////////////////////

int CFrameNode::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc )
{
	if( isHidden() ) 
		return 0;

	// show nesting borders
	if( !fde.IsPrinting() && getCurrentFormulatorNestingState() )
	{
		fde.DrawLine( tl.x(), tl.y(), tl.x() + GetSize().width(), tl.y(), FS_LogPen( Qt::gray ) );
		fde.DrawLine( tl.x() + GetSize().width(), tl.y(), tl.x() + GetSize().width(), tl.y() + GetSize().height(), FS_LogPen( Qt::gray ) );
		fde.DrawLine( tl.x() + GetSize().width(), tl.y() + GetSize().height(), tl.x(), tl.y() + GetSize().height(), FS_LogPen( Qt::black ) );
		fde.DrawLine( tl.x(), tl.y() + GetSize().height(), tl.x(), tl.y(), FS_LogPen( Qt::black ) );
	}

	CNode *pFirst = NULL, *pLast = NULL;
	GetVisibleEdge( rc, tl, pFirst, pLast );
	do
	{
		pFirst->Draw( fde, tl + pFirst->GetPosition(), rc );
		pFirst = pFirst->GetNext();
	}
	while( pFirst != NULL && pFirst != pLast );
	if( pFirst != NULL )
		pFirst->Draw( fde, tl + pFirst->GetPosition(), rc );

	return 1;
}

//////////////////////////////////////////////////////////////////////

void CFrameNode::RecalculateSize_ReArrangeChildPos( SizeFde &sz )
{
	Q_ASSERT( GetChildCount() > 0 );

	sz.rwidth() = getLeftIndent(); 
	sz.rheight() = getTopIndent(); 

	PointFde pos( getLeftIndent(), 0.0 );
	RealFde lineSpacing = GetLineSpacing();
	long nCountMinusOne = GetChildCount() - 1;

	for( long i = 0; i < GetChildCount(); i++ )
	{
		if( i == nCountMinusOne )
			lineSpacing = DEFAULT_LINESPACING;

		pos.ry() = sz.height();
		GetChild( i )->SetPosition( pos );
		sz.rheight() += GetChild( i )->GetSize().height() * lineSpacing;
		sz.rwidth() = std::max( sz.width(), pos.x() + GetChild( i )->GetSize().width() );
	}
	sz.rwidth() += getRightIndent();
	sz.rheight() += getBottomIndent();

	// set vertical center
	if( GetChildCount() > 1 )
		SetVerticalCenter( sz.height() / 2.0 );
	else
		SetVerticalCenter( GetFirstChild()->GetPosition().y() + GetFirstChild()->GetVerticalCenter() );
}

void CFrameNode::RecalculateSize( int bRecalc )
{
	SizeFde sz;
//	CNode *pNode = NULL;

	RecalculateSize_ReArrangeChildPos( sz );
//	if( GetSize() != sz )
	{
		SetSize( sz );
		if( GetParent() != NULL && GetParent()->GetType() == NODE_FORMULA )
		{
			if( ((CFormulaNode*) GetParent())->getGraphPrimNumber() &&
				((CFormulaNode*) GetParent())->correctFrameIndent4TextPrimitive( this ) )
			{
				RecalculateSize_ReArrangeChildPos( sz );
				SetSize( sz );
			}
		}

		if( GetParent() != NULL && bRecalc )
			GetParent()->RecalculateSize( bRecalc );
	}
}

//////////////////////////////////////////////////////////////////////

QString CFrameNode::GetNodeText( void )
{
	QString retText("");
	CNode *pNode = GetFirstChild();
	while( pNode )
	{
		retText += pNode->GetNodeText();
		pNode = pNode->GetNext();
		if( pNode ) retText += "\r\n";
	}
	return retText;
}

//////////////////////////////////////////////////////////////////////

long CFrameNode::GetVisibleEdge( const RectFde &rc, const PointFde &tl, CNode *&pFirst, CNode *&pLast )
{
	CParentNode::GetVisibleEdge( rc, tl, pFirst, pLast );

	if( GetChildCount() == 0 ) return -1;

	CNode *pVisible = NULL;

	long t = 0, b = GetChildCount();
	long i = (b + t) >> 1;
	RealFde x, y, l, r;
	do
	{
		pVisible = GetChild( i );

		x = tl.y() + pVisible->GetPosition().y();
		y = x + pVisible->GetSize().height();
		l = rc.top();
		r = rc.bottom();

		if( y < l )
			t = i;
		else if( x > r )
			b = i;
		else
			break;

		i = (b + t) >> 1;
	}
	while( i != b && i != t );
	Q_ASSERT( pVisible != NULL );

	CNode *pNode;

	pNode = pVisible;
	i++;
	do
	{
		i--;		
		pFirst = pNode;
		pNode = pNode->GetPrev();
		if( pNode == NULL )
			break;

		y = tl.y() + pNode->GetPosition().y() + pNode->GetSize().height();
		l = rc.top();
	}
	while( y > l );

	pNode = pVisible;
	do
	{
		pLast = pNode;
		pNode = pNode->GetNext();
		if( pNode == NULL )
			break;

		x = tl.y() + pNode->GetPosition().y();
		r = rc.bottom();
	}
	while( x < r );
	return i;
}

//////////////////////////////////////////////////////////////////////
