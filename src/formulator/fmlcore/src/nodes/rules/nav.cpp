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

#include <QRegExp>

#include "nav.h"
#include "movstruc.h"

/////////////////////////////////////////////////////////////////////////////

void GetPrevNextInLine( SelectNode &sn, CNode *&pPrev, CNode *&pNext )
{
	Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );

	if( ((CParentNode*) sn.GetNode())->GetChildCount() )
	{
		if( sn.GetPosition() < ((CParentNode*) sn.GetNode())->GetChildCount() )
		{
			pNext = ((CParentNode*) sn.GetNode())->GetChild( sn.GetPosition() );
			pPrev = pNext->GetPrev();
		}
		else
		{
			pPrev = ((CParentNode*) sn.GetNode())->GetLastChild();
			pNext = pPrev->GetNext();
		}
	}
	else
	{
		pPrev = NULL;
		pNext = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////

void GetCaretInfo( const SelectNode& sn, PointFde &pt, SizeFde &sz )
{
	if( sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		sz = ((CPlaneText*) sn.GetNode())->getDrawSize( sn.GetPosition() ).size();

		pt = sn.GetNode()->GetAbsolutePosition();
		pt.ry() = sn.GetNode()->GetParent()->GetAbsolutePosition().y();
		pt.rx() += sz.width();
		Q_ASSERT( sn.GetPosition() > 0 && sn.GetPosition() < ((CPlaneText*) sn.GetNode())->GetStringLength() );

		sz.rwidth() = 1.0;
		sz.rheight() = sn.GetNode()->GetParent()->GetSize().height();
	}
	else if( sn.GetNode()->GetType() == NODE_LINE )
	{
		sz = sn.GetNode()->GetSize();
		sz.rwidth() = 1.0;

		pt = sn.GetNode()->GetAbsolutePosition();
		if( sn.GetPosition() )
		{
			CNode *pNode = ((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() - 1 );
			Q_ASSERT( pNode != NULL );
			if( pNode )
			{
				pt.rx() += pNode->GetPosition().x();
				pt.rx() += pNode->GetSize().width();
			}
		}
	}
	else
	{
		Q_ASSERT( 0 );
	}
}

/////////////////////////////////////////////////////////////////////////////

long GetEmptyLineCount( CParentNode *pParentNode )
{
	long nNumber = 0;
	for( long i = 0; i < pParentNode->GetChildCount(); i++ )
	{
		switch( pParentNode->GetChild( i )->GetType() )
		{
		case NODE_LINE:
			if( ((CLineNode*) pParentNode->GetChild( i ))->GetChildCount() == 0 && pParentNode->GetChild( i )->canBeEdited() )
			{
				nNumber++;
				break;
			}

		case NODE_FRAME:
		case NODE_FORMULA:
			nNumber += GetEmptyLineCount( (CParentNode*) pParentNode->GetChild( i ) );
			break;

		default:
			break;
		}
	}
	return nNumber;
}

/////////////////////////////////////////////////////////////////////////////

int CaretToEmptyLine( CParentNode *pParentNode, SelectNode &sn, long &nNumber )
{
	CParentNode *prNode = 0;
	for( long i = 0; i < pParentNode->GetChildCount(); i++ )
	{
		switch( pParentNode->GetChild( i )->GetType() )
		{
		case NODE_LINE:
			if( ((CLineNode*) pParentNode->GetChild( i ))->GetChildCount() == 0 && pParentNode->GetChild( i )->canBeEdited() )
			{
				if( nNumber )
				{
					nNumber--;
					continue;
				}

				Q_ASSERT( pParentNode->GetChild( i )->canBeEdited() );
				sn.SetNode( pParentNode->GetChild( i ) );
				sn.SetPosition( 0 );
				return 0;
			}

		case NODE_FRAME:
		case NODE_FORMULA:
			prNode = (CParentNode*) pParentNode->GetChild( i );
			if( CaretToEmptyLine( prNode, sn, nNumber ) == 0 )
				return 0;
			break;

		default:
			break;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int CaretToFirstEmptyLine( CParentNode *pParentNode, SelectNode &sn )
{
	long nNumber = 0;
	return CaretToEmptyLine( pParentNode, sn, nNumber );
}

/////////////////////////////////////////////////////////////////////////////

int CaretToSecondEmptyLine( CParentNode *pParentNode, SelectNode &sn )
{
	long nNumber = 1;
	return CaretToEmptyLine( pParentNode, sn, nNumber );
}

/////////////////////////////////////////////////////////////////////////////

int IsHideSelection( int fLeft, int fSel, SelectNode &sn, SelectInfo& si )
{
	if( !fSel && !si.GetBegin().IsEmpty() )
	{
		// снять выделение, курсор в позицию направления движения
		sn = (fLeft ? si.GetBegin() : si.GetEnd());
		si.SetEmpty();
		return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int InternalMoveInPlaneText( CPlaneText *pPlaneText, long nPos, MotionData &md )
{
	Q_ASSERT( pPlaneText != NULL );
	static QString strWordBreakCharacter = _T(" ~!@#$%^&*()-=_/\\[]{}?<>,.:;|\"\'");

	if( md.IsLeft() )
	{
		if( nPos > 1 )
		{
			if( md.IsWord() )
			{
				if( !md.IsAllowIntoText() )
					return -1;

				long idx;
				long pos;
				QString strText;
				for( idx = pPlaneText->GetString().length() - 1; idx >= 0; idx-- )
					strText.append( pPlaneText->GetString().at(idx) );
				nPos = pPlaneText->GetStringLength() - nPos;

				for( idx = 0; idx < strWordBreakCharacter.length(); idx++ )
				{
					pos = strText.indexOf( strWordBreakCharacter.at(idx), nPos );
					if( pos != -1 )
					{
						nPos = pPlaneText->GetStringLength() - 
							(long) (strWordBreakCharacter.indexOf( strText.at(nPos) ) == -1 ? pos : pos + 1);
						md.GetSN().SetNode( pPlaneText );
						md.GetSN().SetPosition( nPos );
						return 0;
					}
				}
			}
			else
			{
				md.GetSN().SetNode( pPlaneText );
				md.GetSN().SetPosition( nPos - 1 );
				return 0;
			}
		}
	}
	else
	{
		if( nPos + 1 < pPlaneText->GetStringLength() )
		{
			if( md.IsWord() )
			{
				if( !md.IsAllowIntoText() )
					return -1;

				long idx;
				long pos;
				QString strText( pPlaneText->GetString() );

				for( idx = 0; idx < strWordBreakCharacter.length(); idx++ )
				{
					pos = strText.indexOf( strWordBreakCharacter.at(idx), nPos );
					if( pos != -1 )
					{
						nPos = (long) (strWordBreakCharacter.indexOf( strText.at(nPos) ) == -1 ? pos : pos + 1);
						md.GetSN().SetNode( pPlaneText );
						md.GetSN().SetPosition( nPos );
						return 0;
					}
				}
			}
			else
			{
				md.GetSN().SetNode( pPlaneText );
				md.GetSN().SetPosition( nPos + 1 );
				return 0;
			}
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveInFrame( CFrameNode *pFrameNode, long nPos, MotionData &md );
int MoveInLine( CLineNode *pLineNode, long nPos, MotionData &md );
int MoveInText( CPlaneText *pPlaneText, long nPos, MotionData &md );
int MoveInFormula( CFormulaNode *pFormulaNode, long nPos, MotionData &md );

int MoveInText( CPlaneText *pPlaneText, long nPos, MotionData &md )
{
	if( md.IsLeft() )
	{
		if( InternalMoveInPlaneText( pPlaneText, nPos, md ) != 0 )
		{
			Q_ASSERT( pPlaneText->GetParent()->GetType() == NODE_LINE );
			md.GetSN().SetNode( pPlaneText->GetParent() );
			md.GetSN().SetPosition( pPlaneText->GetParent()->GetChildIndex( pPlaneText ) );
		}
	}
	else
	{
		if( InternalMoveInPlaneText( pPlaneText, nPos, md ) != 0 )
		{
			Q_ASSERT( pPlaneText->GetParent()->GetType() == NODE_LINE );
			md.GetSN().SetNode( pPlaneText->GetParent() );
			md.GetSN().SetPosition( pPlaneText->GetParent()->GetChildIndex( pPlaneText ) + 1 );
		}
	}
	md.Update();
	return 0;
}

int MoveInLine( CLineNode *pLineNode, long nPos, MotionData &md )
{
	if( md.IsLeft() )
	{
		while( nPos )
		{
			if( md.IsSel() && pLineNode->isCannotBeEdited() ) 
				break;

			nPos--;

			if( md.IsWord() ) 
			{
				// если дальше следует текст, анализируем не нужно ли войти внутрь текста
				if( md.IsAllowIntoText() && pLineNode->GetChild( nPos )->GetType() == NODE_PLANETEXT )
				{
					CPlaneText *pPlaneText = (CPlaneText*) pLineNode->GetChild( nPos );
					if( InternalMoveInPlaneText( pPlaneText, pPlaneText->GetStringLength()/* - 1*/, md ) == 0 )
					{
						md.Update();
						return 0;
					}
				}

				if( md.GetSN().GetNode() == pLineNode )
				{
					md.GetSN().SetPosition( nPos );
					md.Update();
					return 0;
				}
			}

			switch( pLineNode->GetChild( nPos )->GetType() )
			{
			case NODE_FRAME:
			case NODE_LINE:
				Q_ASSERT( 0 );
				break;
	
			case NODE_PLANETEXT:
				{
					CPlaneText *pPlaneText = (CPlaneText*) pLineNode->GetChild( nPos );
					if( pPlaneText->canBeEdited() )
					{
						long nLen = pPlaneText->GetStringLength();
						if( nLen == 1 )
						{
							md.GetSN().SetNode( pLineNode );
							md.GetSN().SetPosition( nPos );
						}
						else
						{
							md.GetSN().SetNode( pPlaneText );
							md.GetSN().SetPosition( nLen - 1 );
						}
					}
					else
					{
						if( pLineNode->isCannotBeEdited() ) break;

						md.GetSN().SetNode( pLineNode );
						md.GetSN().SetPosition( nPos );
					}
					md.Update();
					return 0;
				}
				break;
	
			case NODE_FORMULA:
				{
					if( md.IsSel() )
					{
						md.GetSN().SetNode( pLineNode );
						md.GetSN().SetPosition( nPos );
						md.Update();
						return 0;
					}
					CFormulaNode *pFormulaNode = (CFormulaNode*) pLineNode->GetChild( nPos );
					if( MoveInFormula( pFormulaNode, pFormulaNode->GetChildCount(), md ) == 0 )
						return 0;
				}
				break;

			default:
				break;
			}
		}

		CParentNode *pParentNode = pLineNode->GetParent();
		nPos = pParentNode->GetChildIndex( pLineNode );
		switch( pParentNode->GetType() )
		{
		case NODE_FRAME:
			if( MoveInFrame( (CFrameNode*) pParentNode, nPos, md ) == 0 ) return 0;
			break;

		case NODE_LINE:
			Q_ASSERT( 0 );
			break;

		case NODE_FORMULA:
			if( MoveInFormula( (CFormulaNode*) pParentNode, nPos, md ) == 0 ) return 0;
			break;

		default:
			break;
		}
	}
	else
	{
		while( nPos < pLineNode->GetChildCount() )
		{
			if( md.IsSel() && pLineNode->isCannotBeEdited() ) break;

			if( md.IsWord() ) 
			{
				if( md.IsAllowIntoText() && pLineNode->GetChild( nPos )->GetType() == NODE_PLANETEXT )
				{
					CPlaneText *pPlaneText = (CPlaneText*) pLineNode->GetChild( nPos );
					if( InternalMoveInPlaneText( pPlaneText, 0/*1*/, md ) == 0 )
					{
						md.Update();
						return 0;
					}
				}
				if( md.GetSN().GetNode() == pLineNode )
				{
					md.GetSN().SetPosition( nPos + 1 );
					md.Update();
					return 0;
				}
			}

			switch( pLineNode->GetChild( nPos )->GetType() )
			{
			case NODE_FRAME:
			case NODE_LINE:
				Q_ASSERT( 0 );
				break;
	
			case NODE_PLANETEXT:
				{
					CPlaneText *pPlaneText = (CPlaneText*) pLineNode->GetChild( nPos );
					if( pPlaneText->canBeEdited() )
					{
						long nLen = pPlaneText->GetStringLength();
						if( nLen == 1 )
						{
							md.GetSN().SetNode( pLineNode );
							md.GetSN().SetPosition( nPos + 1 );
						}
						else
						{
							md.GetSN().SetNode( pPlaneText );
							md.GetSN().SetPosition( 1 );
						}
					}
					else
					{
						if( pLineNode->isCannotBeEdited() ) break;

						md.GetSN().SetNode( pLineNode );
						md.GetSN().SetPosition( nPos + 1 );
					}
					md.Update();
					return 0;
				}
				break;
	
			case NODE_FORMULA:
				{
					if( md.IsSel() )
					{
						md.GetSN().SetNode( pLineNode );
						md.GetSN().SetPosition( nPos + 1 );
						md.Update();
						return 0;
					}
					CFormulaNode *pFormulaNode = (CFormulaNode*) pLineNode->GetChild( nPos );
					if( MoveInFormula( pFormulaNode, 0, md ) == 0 ) return 0;
				}
				break;

			default:
				break;
			}
			nPos++;
		}
		CParentNode *pParentNode = pLineNode->GetParent();
		nPos = pParentNode->GetChildIndex( pLineNode ) + 1;
		switch( pParentNode->GetType() )
		{
		case NODE_FRAME:
			if( MoveInFrame( (CFrameNode*) pParentNode, nPos, md ) == 0 ) return 0;
			break;

		case NODE_LINE:
			Q_ASSERT( 0 );
			break;

		case NODE_FORMULA:
			if( MoveInFormula( (CFormulaNode*) pParentNode, nPos, md ) == 0 ) return 0;
			break;
		default:
			break;
		}
	}
	return -1;
}

int MoveInFrame( CFrameNode *pFrameNode, long nPos, MotionData &md )
{
	if( md.IsLeft() )
	{
		while( nPos )
		{
			nPos--;
			switch( pFrameNode->GetChild( nPos )->GetType() )
			{
			case NODE_FRAME:
			case NODE_PLANETEXT:
			case NODE_FORMULA:
				Q_ASSERT( 0 );
				break;
	
			case NODE_LINE:
				{
					CLineNode *pLineNode = (CLineNode*) pFrameNode->GetChild( nPos );
					if( pLineNode->canBeEdited() )
					{
						md.GetSN().SetNode( pLineNode );
						md.GetSN().SetPosition( pLineNode->GetChildCount() );
						md.Update();
						return 0;
					}
					else
					{
						if( MoveInLine( pLineNode, pLineNode->GetChildCount(), md ) == 0 ) return 0;
					}
				}
				break;
			default:
				break;
			}
		}

		CParentNode *pParentNode = pFrameNode->GetParent();
		if( pParentNode == NULL )
		{
			return -1;
		}
		nPos = pParentNode->GetChildIndex( pFrameNode );
		switch( pParentNode->GetType() )
		{
		case NODE_FRAME:
			Q_ASSERT( 0 );
			break;

		case NODE_LINE:
			{
				CLineNode *pLineNode = (CLineNode*) pParentNode;
				if( pLineNode->canBeEdited() )
				{
					md.GetSN().SetNode( pLineNode );
					md.GetSN().SetPosition( nPos );
					md.Update();
					return 0;
				}
				else
				{
					if( MoveInLine( pLineNode, nPos, md ) == 0 ) return 0;
				}
			}
			break;

		case NODE_FORMULA:
			if( MoveInFormula( (CFormulaNode*) pParentNode, nPos, md ) == 0 ) return 0;
			break;

		default:
			break;
		}
	}
	else
	{
		while( nPos < pFrameNode->GetChildCount() )
		{
			switch( pFrameNode->GetChild( nPos )->GetType() )
			{
			case NODE_FRAME:
			case NODE_PLANETEXT:
			case NODE_FORMULA:
				Q_ASSERT( 0 );
				break;
	
			case NODE_LINE:
				{
					CLineNode *pLineNode = (CLineNode*) pFrameNode->GetChild( nPos );
					if( pLineNode->canBeEdited() )
					{
						md.GetSN().SetNode( pLineNode );
						md.GetSN().SetPosition( 0 );
						md.Update();
						return 0;
					}
					else
					{
						if( MoveInLine( pLineNode, 0, md ) == 0 ) return 0;
					}
				}
				break;

			default:
				break;
			}
			nPos++;
		}

		CParentNode *pParentNode = pFrameNode->GetParent();
		if( pParentNode == NULL )
		{
			return -1;
		}
		nPos = pParentNode->GetChildIndex( pFrameNode );
		switch( pParentNode->GetType() )
		{
		case NODE_FRAME:
			Q_ASSERT( 0 );
			break;

		case NODE_LINE:
			{
				CLineNode *pLineNode = (CLineNode*) pParentNode;
				if( pLineNode->canBeEdited() )
				{
					md.GetSN().SetNode( pLineNode );
					md.GetSN().SetPosition( nPos + 1 );
					md.Update();
					return 0;
				}
				else
				{
					if( MoveInLine( pLineNode, nPos + 1, md ) == 0 ) return 0;
				}
			}
			break;

		case NODE_FORMULA:
			if( MoveInFormula( (CFormulaNode*) pParentNode, nPos + 1, md ) == 0 ) return 0;
			break;

		default:
			break;
		}
	}
	return -1;
}

int MoveInFormula( CFormulaNode *pFormulaNode, long nPos, MotionData &md )
{
	if( md.IsLeft() )
	{
		while( !md.IsSel() && nPos )
		{
			nPos--;
			switch( pFormulaNode->GetChild( nPos )->GetType() )
			{
			case NODE_LINE:
			case NODE_PLANETEXT:
			case NODE_FORMULA:
				Q_ASSERT( 0 );
				break;
	
			case NODE_FRAME:
				{
					CFrameNode *pFrameNode = (CFrameNode*) pFormulaNode->GetChild( nPos );
					if( MoveInFrame( pFrameNode, pFrameNode->GetChildCount(), md ) == 0 ) return 0;
				}
				break;

			default:
				break;
			}
		}

		CParentNode *pParentNode = pFormulaNode->GetParent();
		nPos = pParentNode->GetChildIndex( pFormulaNode );
		switch( pParentNode->GetType() )
		{
		case NODE_FRAME:
		case NODE_FORMULA:
			Q_ASSERT( 0 );
			break;

		case NODE_LINE:
			{
				CLineNode *pLineNode = (CLineNode*) pParentNode;
				if( pLineNode->canBeEdited() )
				{
					md.GetSN().SetNode( pLineNode );
					md.GetSN().SetPosition( nPos );
					md.Update();
					return 0;
				}
				else
				{
					if( MoveInLine( pLineNode, nPos, md ) == 0 ) return 0;
				}
			}
			break;

		default:
			break;
		}
	}
	else
	{
		while( !md.IsSel() && nPos < pFormulaNode->GetChildCount() )
		{
			switch( pFormulaNode->GetChild( nPos )->GetType() )
			{
			case NODE_LINE:
			case NODE_PLANETEXT:
			case NODE_FORMULA:
				Q_ASSERT( 0 );
				break;
	
			case NODE_FRAME:
				{
					CFrameNode *pFrameNode = (CFrameNode*) pFormulaNode->GetChild( nPos );
					if( MoveInFrame( pFrameNode, 0, md ) == 0 ) return 0;
				}
				break;

			default:
				break;
			}
			nPos++;
		}

		CParentNode *pParentNode = pFormulaNode->GetParent();
		nPos = pParentNode->GetChildIndex( pFormulaNode );
		switch( pParentNode->GetType() )
		{
		case NODE_FRAME:
		case NODE_FORMULA:
			Q_ASSERT( 0 );
			break;

		case NODE_LINE:
			{
				CLineNode *pLineNode = (CLineNode*) pParentNode;
				if( pLineNode->canBeEdited() )
				{
					md.GetSN().SetNode( pLineNode );
					md.GetSN().SetPosition( nPos + 1 );
					md.Update();
					return 0;
				}
				else
				{
					if( MoveInLine( pLineNode, nPos + 1, md ) == 0 ) return 0;
				}
			}
			break;

		default:
			break;
		}
	}
	return -1;
}

int MoveLeftRight( MotionData &md )
{
	switch( md.GetSN().GetNode()->GetType() )
	{
	case NODE_PLANETEXT:
		if( MoveInText( (CPlaneText*) md.GetSN().GetNode(), md.GetSN().GetPosition(), md ) == 0 ) return 0;
		break;	

	case NODE_LINE:
		if( MoveInLine( (CLineNode*) md.GetSN().GetNode(), md.GetSN().GetPosition(), md ) == 0 ) return 0;
		break;	

	default:
		Q_ASSERT( 0 );
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveLeftRight( SelectNode &sn, SelectInfo &si, int fLeft, int fSel, int fWord, int fAllowIntoText )
{
	if( IsHideSelection( fLeft, fSel, sn, si ) == 0 )
		return 0;

	MotionData md( fLeft, fSel, fWord, fAllowIntoText, sn, si );
	if( MoveLeftRight( md ) == 0 )
	{
		sn = md.GetSN();
		si.GetBegin() = md.GetSB();
		si.GetEnd() = md.GetSE();
		return 0;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////

void SimultaneousSorting( std::vector<RealFde> &values, std::vector<CNode*> &nodes )
{
	bool fCountinue;
	do
	{
		fCountinue = false;
		for( long i = 1; i < (long) values.size(); i++ )
		{
			if( values[ i - 1 ] > values[ i ] )
			{
				std::swap( values[ i - 1 ], values[ i ] );
				std::swap( nodes[ i - 1 ], nodes[ i ] );
				fCountinue = true;
			}
		}
	}
	while( fCountinue );
}

/////////////////////////////////////////////////////////////////////////////

inline 
int IsAcceptableRect( int fUp, const RectFde &rc, const PointFde &pt )
{
	return (fUp ? (rc.bottom() <= pt.y()) : (rc.top() >= pt.y()));
}

/////////////////////////////////////////////////////////////////////////////

void Arrange_UpDown_Frame( 
	CFrameNode *pFrameNode, CNode *pNode, const RectFde &rc, const PointFde &tl, 
	Nearest_Node &nn, MotionData &md, std::vector<RealFde> &values, std::vector<CNode*> &nodes )
{
	values.clear();
	nodes.clear();

	CNode *pFirstNode = NULL;
	if( pNode != NULL )
	{
		pFirstNode = md.IsLeft() ? pNode->GetPrev() : pNode->GetNext();
	}
	else
	{
		pFirstNode = md.IsLeft() ? pFrameNode->GetLastChild() : pFrameNode->GetFirstChild();
	}

	RectFde rcItem;
	while( pFirstNode != NULL )
	{
		if( pFirstNode != pNode )
		{
			Q_ASSERT( pFirstNode->GetType() == NODE_LINE );
			::assign( rcItem, tl + pFirstNode->GetPosition() );
			rcItem += pFirstNode->GetSize();

			if( IsAcceptableRect( md.IsLeft(), rcItem, nn.m_pt ) )
			{
				switch( pFirstNode->GetType() )
				{
				case NODE_LINE:
					values.push_back( GetDistance_ShortestNormal( nn.m_pt.y(), rcItem.top(), rcItem.bottom() ) );
					nodes.push_back( pFirstNode );
					break;

				case NODE_FRAME:
				case NODE_PLANETEXT:
				case NODE_FORMULA:
					break;

				default:
					break;
				}
			}

			if( md.IsLeft() )
			{
				if( rcItem.bottom() < rc.top() ) break;
				pFirstNode = pFirstNode->GetPrev();
			}
			else
			{
				if( rcItem.top() > rc.bottom() ) break;
				pFirstNode = pFirstNode->GetNext();
			}
		}
	}

	SimultaneousSorting( values, nodes );
}

void Arrange_UpDown_Line( 
	CLineNode *pLineNode, CNode *pNode, const RectFde &rc, const PointFde &tl, 
	Nearest_Node &nn, MotionData &md, std::vector<RealFde> &values, std::vector<CNode*> &nodes )
{
	values.clear();
	nodes.clear();

	CNode *pFirstNode = NULL, *pLastNode = NULL;
	pLineNode->GetVisibleEdge( rc, tl, pFirstNode, pLastNode );

	RectFde rcItem;
	if( pFirstNode != NULL )
	{
		RealFde distance, value = FLT_MAX;

		do
		{
			if( pFirstNode != pNode )
			{
				::assign( rcItem, tl + pFirstNode->GetPosition() );
				rcItem += pFirstNode->GetSize();
    
				if( IsAcceptableRect( md.IsLeft(), rcItem, nn.m_pt ) )
				{
					switch( pFirstNode->GetType() )
					{
					case NODE_FRAME:
					case NODE_LINE:
						Q_ASSERT( 0 );
						break;

					case NODE_PLANETEXT:
					case NODE_FORMULA:
						values.push_back( GetDistance_Point2Parent( nn.m_pt, rcItem ) );
						nodes.push_back( pFirstNode );
						break;

					default:
						break;
					}

					// find nearest position in line
					distance = GetDistance_Point2Line( nn.m_pt, rcItem );
					if( value > distance ) value = distance;
				}
			}

			if( pFirstNode == pLastNode ) break;
			pFirstNode = pFirstNode->GetNext();
		}
		while( pFirstNode != NULL );

		if( value < FLT_MAX )
		{
			values.insert( values.begin(), value - 1 );
			nodes.insert( nodes.begin(), pLineNode );
		}
	}

	SimultaneousSorting( values, nodes );
}

void Arrange_UpDown_Formula( 
        CFormulaNode *pFormulaNode, CNode *pNode, const RectFde & /*rc*/, const PointFde &tl,
	Nearest_Node &nn, MotionData &md, std::vector<RealFde> &values, std::vector<CNode*> &nodes )
{
	values.clear();
	nodes.clear();

	CNode *pFirstNode = pFormulaNode->GetFirstChild();
	CNode *pLastNode = pFormulaNode->GetLastChild();

	RectFde rcItem;
	if( pFirstNode != NULL )
	{
		do
		{
			if( pFirstNode != pNode )
			{
				::assign( rcItem, tl + pFirstNode->GetPosition() );
				rcItem += pFirstNode->GetSize();

				if( IsAcceptableRect( md.IsLeft(), rcItem, nn.m_pt ) )
				{
					switch( pFirstNode->GetType() )
					{
					case NODE_PLANETEXT:
					case NODE_FORMULA:
					case NODE_LINE:
						break;

					case NODE_FRAME:
						values.push_back( GetDistance_Point2Parent( nn.m_pt, rcItem ) );
						nodes.push_back( pFirstNode );
						break;

					default:
						break;
					}
				}
			}

			if( pFirstNode == pLastNode ) break;
			pFirstNode = pFirstNode->GetNext();
		}
		while( pFirstNode != NULL );
	}

	SimultaneousSorting( values, nodes );
}

/////////////////////////////////////////////////////////////////////////////

int MoveInFrame( CFrameNode *pFrameNode, CNode *pNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn, MotionData &md );
int MoveInLine( CLineNode *pLineNode, CNode *pNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn, MotionData &md );
int MoveInFormula( CFormulaNode *pFormulaNode, CNode *pNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn, MotionData &md );

/////////////////////////////////////////////////////////////////////////////

int MoveInFrame( CFrameNode *pFrameNode, CNode *pNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn, MotionData &md )
{
	Q_ASSERT( pFrameNode->GetChildCount() );

	std::vector<RealFde> values;
	std::vector<CNode*> nodes;

	Arrange_UpDown_Frame( pFrameNode, pNode, rc, tl, nn, md, values, nodes );

	PointFde ofs;
	for( long i = 0; i < (long) nodes.size(); i++ )
	{
		if( values[ i ] > nn.m_value ) break;

		switch( nodes[ i ]->GetType() )
		{
		case NODE_PLANETEXT:
		case NODE_FRAME:
		case NODE_FORMULA:
			Q_ASSERT( 0 );
			break;

		case NODE_LINE:
			if( values[ i ] <= nn.m_value && nodes[ i ]->canBeEdited() && ((CLineNode*) nodes[ i ])->GetChildCount() == 0 )
			{
				nn.m_value = values[ i ];
				nn.m_pNode = nodes[ i ];
				return 0;
			}
			else if( ((CLineNode*) nodes[ i ])->GetChildCount() )
			{
				ofs = tl + nodes[ i ]->GetPosition();
				if( MoveInLine( (CLineNode*) nodes[ i ], NULL, rc, ofs, nn, md ) == 0 )
					return 0;
			}
			break;

		default:
			break;
		}
	}

	if( nodes.size() == 0 && pFrameNode->GetParent() )
	{
		switch( pFrameNode->GetParent()->GetType() )
		{
		case NODE_FRAME:
		case NODE_LINE:
		case NODE_PLANETEXT:
			Q_ASSERT( 0 );
			break;

		case NODE_FORMULA:
			ofs = tl - pFrameNode->GetPosition();
			if( MoveInFormula( (CFormulaNode*) pFrameNode->GetParent(), pFrameNode, rc, ofs, nn, md ) == 0 )
			{
				return 0;
			}
			break;

		default:
			break;
		}
	}

	return -1;
}

int MoveInLine( CLineNode *pLineNode, CNode *pNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn, MotionData &md )
{
	std::vector<RealFde> values;
	std::vector<CNode*> nodes;

	Arrange_UpDown_Line( pLineNode, pNode, rc, tl, nn, md, values, nodes );

	PointFde ofs;
	for( long i = 0; i < (long) nodes.size(); i++ )
	{
		switch( nodes[ i ]->GetType() )
		{
		case NODE_FRAME:
			Q_ASSERT( 0 );
			break;

		case NODE_LINE:
			Q_ASSERT( nodes[ i ] == pLineNode );
			if( values[ i ] <= nn.m_value && nodes[ i ]->canBeEdited() )
			{
				nn.m_value = values[ i ];
				nn.m_pNode = nodes[ i ];
				return 0;
			}
			break;

		case NODE_PLANETEXT:
			if( values[ i ] <= nn.m_value && nodes[ i ]->canBeEdited() )
			{
				nn.m_value = values[ i ];
				nn.m_pNode = nodes[ i ];
				return 0;
			}
			break;

		case NODE_FORMULA:
			ofs = tl + nodes[ i ]->GetPosition();
			if( MoveInFormula( (CFormulaNode*) nodes[ i ], NULL, rc, ofs, nn, md ) == 0 )
				return 0;
			break;

		default:
			Q_ASSERT( 0 );
			break;
		}
	}

	if( nodes.size() == 0 )
	{
		switch( pLineNode->GetParent()->GetType() )
		{
		case NODE_FRAME:
			ofs = tl - pLineNode->GetPosition();
			if( MoveInFrame( (CFrameNode*) pLineNode->GetParent(), pLineNode, rc, ofs, nn, md ) == 0 )
				return 0;
			break;

		case NODE_LINE:
		case NODE_PLANETEXT:
		case NODE_FORMULA:
			Q_ASSERT( 0 );
			break;

		default:
			break;
		}
	}

	return -1;
}

int MoveInFormula( CFormulaNode *pFormulaNode, CNode *pNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn, MotionData &md )
{
	PointFde ofs;
	std::vector<RealFde> values;
	std::vector<CNode*> nodes;

	if( !md.IsSel() )
	{
		Arrange_UpDown_Formula( pFormulaNode, pNode, rc, tl, nn, md, values, nodes );

		for( long i = 0; i < (long) nodes.size(); i++ )
		{
			switch( nodes[ i ]->GetType() )
			{
			case NODE_PLANETEXT:
			case NODE_LINE:
			case NODE_FORMULA:
				Q_ASSERT( 0 );
				break;

			case NODE_FRAME:
				ofs = tl + nodes[ i ]->GetPosition();
				if( MoveInFrame( (CFrameNode*) nodes[ i ], NULL, rc, ofs, nn, md ) == 0 )
					return 0;
				break;

			default:
				break;
			}
		}
	}

	if( nodes.size() == 0 )
	{
		switch( pFormulaNode->GetParent()->GetType() )
		{
		case NODE_LINE:
			ofs = tl - pFormulaNode->GetPosition();
			if( MoveInLine( (CLineNode*) pFormulaNode->GetParent(), pFormulaNode, rc, ofs, nn, md ) == 0 )
				return 0;
			break;

		case NODE_FRAME:
		case NODE_PLANETEXT:
		case NODE_FORMULA:
			Q_ASSERT( 0 );
			break;

		default:
			Q_ASSERT( 0 );
			break;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int NearestNode2MotionData( const RectFde &rc, Nearest_Node &nn, MotionData &md )
{
	if( nn.m_pNode && nn.m_value < LONG_MAX )
	{
		long nPos = 0;

		switch( nn.m_pNode->GetType() )
		{
		case NODE_PLANETEXT:
			nPos = ((CPlaneText*) nn.m_pNode)->getPosFromPoint( nn.m_pt );
			if( nPos == 0 )
			{
				md.GetSN().SetNode( nn.m_pNode->GetParent() );
				md.GetSN().SetPosition( nn.m_pNode->GetParent()->GetChildIndex( nn.m_pNode ) );
			}
			else if( nPos == ((CPlaneText*) nn.m_pNode)->GetStringLength() )
			{
				md.GetSN().SetNode( nn.m_pNode->GetParent() );
				md.GetSN().SetPosition( nn.m_pNode->GetParent()->GetChildIndex( nn.m_pNode ) + 1 );
			}
			else
			{
				md.GetSN().SetNode( nn.m_pNode );
				md.GetSN().SetPosition( nPos );
			}
			md.Update();
			return 0;

		case NODE_LINE:
			nPos = ((CLineNode*) nn.m_pNode)->getPosFromPoint( rc, nn.m_pt );
			md.GetSN().SetNode( nn.m_pNode );
			md.GetSN().SetPosition( nPos );
			md.Update();
			return 0;

		default:
			Q_ASSERT( 0 );
			break;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveUpDown( const RectFde &rc, const PointFde &pt, MotionData &md )
{
	CLineNode *pLineNode = NULL;
	Nearest_Node nn( pt );

	if( md.GetSN().GetNode()->GetType() == NODE_PLANETEXT )
		pLineNode = (CLineNode*) md.GetSN().GetNode()->GetParent();
	else if( md.GetSN().GetNode()->GetType() == NODE_LINE )
		pLineNode = (CLineNode*) md.GetSN().GetNode();
	else
		Q_ASSERT( 0 );

	CFrameNode *pFrameNode = (CFrameNode*) pLineNode->GetParent();
	Q_ASSERT( pFrameNode->GetType() == NODE_FRAME );
	MoveInFrame( pFrameNode, pLineNode, rc, pFrameNode->GetAbsolutePosition(), nn, md );

	return NearestNode2MotionData( rc, nn, md );
}

/////////////////////////////////////////////////////////////////////////////

int MoveUpDown( const RectFde &rc, const PointFde &pt, SelectNode &sn, SelectInfo &si, int fUp, int fSel )
{
	if( IsHideSelection( fUp, fSel, sn, si ) == 0 )
		return 0;

	MotionData md( fUp, fSel, 0, 0, sn, si );
	if( MoveUpDown( rc, pt, md ) == 0 )
	{
		sn = md.GetSN();
		si.GetBegin() = md.GetSB();
		si.GetEnd() = md.GetSE();
		return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveHomeEnd( MotionData &md, CFrameNode *pFrameNode )
{
	Q_ASSERT( pFrameNode->GetParent() == NULL );

	if( md.IsWord() )
	{
		if( md.IsLeft() )
		{
			if( md.GetSN().GetNode() == pFrameNode->GetFirstChild() && md.GetSN().GetPosition() == 0 )
				return -1;
		}
		else
		{
			if( md.GetSN().GetNode() == pFrameNode->GetLastChild() && md.GetSN().GetPosition() == ((CLineNode*) pFrameNode->GetLastChild())->GetChildCount() )
				return -1;
		}
	}
	else if( md.GetSN().GetNode()->GetParent() == pFrameNode )
	{
		if( md.IsLeft() )
		{
			if( md.GetSN().GetPosition() == 0 )
				return -1;
		}
		else
		{
			if( md.GetSN().GetPosition() == ((CLineNode*) md.GetSN().GetNode())->GetChildCount() )
				return -1;
		}
	}

	CNode *pPrev = NULL;
	if( md.IsWord() )
	{
		pPrev = (md.IsLeft() ? pFrameNode->GetFirstChild() : pFrameNode->GetLastChild());
	}
	else
	{
		CNode *pNext = pPrev = md.GetSN().GetNode();
		while( pNext->GetParent() ) 
		{
			pPrev = pNext;
			pNext = pNext->GetParent();
		}
	}
	Q_ASSERT( pPrev != NULL && pPrev->GetType() == NODE_LINE );
	md.GetSN().SetNode( pPrev );
	md.GetSN().SetPosition( (md.IsLeft() ? 0 : ((CLineNode*) pPrev)->GetChildCount()) );

	md.Update();
	return 0;
}

int MoveHomeEnd( CFrameNode *pFrameNode, SelectNode &sn, SelectInfo &si, int fHome, int fSel, int fCtrl )
{
	if( IsHideSelection( fHome, fSel, sn, si ) == 0 )
		return 0;

	if( fSel )
	{
		if( (fHome && sn == si.GetEnd()) || (!fHome && sn == si.GetBegin()) )
		{
			sn = (fHome ? si.GetBegin() : si.GetEnd());
			si.SetEmpty();
		}
	}

	MotionData md( fHome, fSel, fCtrl, 0, sn, si );
	if( MoveHomeEnd( md, pFrameNode ) == 0 )
	{
		sn = md.GetSN();
		si.GetBegin() = md.GetSB();
		si.GetEnd() = md.GetSE();
		return 0;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////

CNode* GetPropertyNode( SelectNode &sn, SelectInfo &si )
{
	CNode *pNode = NULL ;
	if( si.IsSelection() && si.GetBegin().GetNode() == si.GetEnd().GetNode() )
	{
		if( si.GetBegin().GetNode()->GetType() == NODE_PLANETEXT )
			pNode = si.GetBegin().GetNode();
		else if( sn.GetNode()->GetType() == NODE_LINE )
		{
			if( si.GetBegin().GetPosition() + 1 == si.GetEnd().GetPosition() )
				pNode = ((CLineNode*) si.GetBegin().GetNode())->GetChild( si.GetBegin().GetPosition() );
		}
	}
	else
	{
		if( sn.GetNode()->GetType() == NODE_PLANETEXT )
			pNode = sn.GetNode();
		else if( sn.GetNode()->GetType() == NODE_LINE )
		{
			if( ((CLineNode*) sn.GetNode())->GetChildCount() == 0 )
				pNode = sn.GetNode()->getParentFormulaNode( 0 );
			else if( sn.GetPosition() == ((CLineNode*) sn.GetNode())->GetChildCount() )
				pNode = ((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() - 1 );
			else 
				pNode = ((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() );
		}
	}

	while( pNode != NULL && pNode->isCannotBeEdited() )
	{
		Q_ASSERT( pNode->GetParent() != NULL );
		pNode = pNode->GetParent()->getParentFormulaNode( 0 );
	}

	return pNode;
}

int SelectPropertyNode( CNode *pNode, SelectNode &sn, SelectInfo &si )
{
	if( pNode == NULL || !(pNode->GetType() == NODE_PLANETEXT || pNode->GetType() == NODE_FORMULA) )
	{
		return 0;
	}

	int bEnd = (sn == si.GetEnd());

	CParentNode *pParentNode = pNode->GetParent();
	Q_ASSERT( pParentNode != NULL );

	si.GetBegin().SetPosition( pParentNode->GetChildIndex( pNode ) );
	si.GetBegin().SetNode( pParentNode );
	si.GetEnd().SetPosition( si.GetBegin().GetPosition() + 1 );
	si.GetEnd().SetNode( pParentNode );

	sn = (bEnd ? si.GetEnd() : si.GetBegin());
	return 1;
}

/////////////////////////////////////////////////////////////////////////////
