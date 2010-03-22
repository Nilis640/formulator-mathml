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

#include "HUtils/ihkmap.h"

#include "movstruc.h"
#include "nav.h"
#include "mouse.h"

#include "../node.h"

/////////////////////////////////////////////////////////////////////////////

void Arrange_Click_Frame( 
	CFrameNode *pFrameNode, const RectFde &rc, const PointFde &tl, 
	Nearest_Node &nn, std::vector<RealFde> &values, std::vector<CNode*> &nodes )
{
	values.clear();
	nodes.clear();

	CNode *pFirstNode = NULL, *pLastNode = NULL;
	pFrameNode->GetVisibleEdge( rc, tl, pFirstNode, pLastNode );

	RectFde rcItem;
	if( pFirstNode != NULL )
	{
		do
		{
			::assign( rcItem, tl + pFirstNode->GetPosition() );
			rcItem += pFirstNode->GetSize();
    
			switch( pFirstNode->GetType() )
			{
			case NODE_FRAME:
			case NODE_PLANETEXT:
			case NODE_FORMULA:
				Q_ASSERT( 0 );
				break;

			case NODE_LINE:
				values.push_back( GetDistance_Point2Parent( nn.m_pt, rcItem ) );
				nodes.push_back( pFirstNode );
				break;
			default:
				break;
			}

			if( pFirstNode == pLastNode ) break;
			pFirstNode = pFirstNode->GetNext();
		}
		while( pFirstNode != NULL );
	}

	SimultaneousSorting( values, nodes );
}

void Arrange_Click_Line( 
	CLineNode *pLineNode, const RectFde &rc, const PointFde &tl, 
	Nearest_Node &nn, std::vector<RealFde> &values, std::vector<CNode*> &nodes )
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
			::assign( rcItem, tl + pFirstNode->GetPosition() );
			rcItem += pFirstNode->GetSize();
    
			switch( pFirstNode->GetType() )
			{
			case NODE_FRAME:
			case NODE_LINE:
				Q_ASSERT( 0 );
				break;

			case NODE_PLANETEXT:
				values.push_back( GetDistance_Point2Text( nn.m_pt, rcItem ) );
				nodes.push_back( pFirstNode );
				break;

			case NODE_FORMULA:
				values.push_back( GetDistance_Point2Parent( nn.m_pt, rcItem ) );
				nodes.push_back( pFirstNode );
				break;
			default:
				break;
			}

			distance = GetDistance_Point2Line( nn.m_pt, rcItem );
			if( value > distance ) value = distance;

			if( pFirstNode == pLastNode ) break;
			pFirstNode = pFirstNode->GetNext();
		}
		while( pFirstNode != NULL );

		if( value < LONG_MAX )
		{
			values.insert( values.begin(), value - 1 );
			nodes.insert( nodes.begin(), pLineNode );
		}
	}

	SimultaneousSorting( values, nodes );
}

void Arrange_Click_Formula( 
	CFormulaNode *pFormulaNode, const RectFde &rc, const PointFde &tl, 
	Nearest_Node &nn, std::vector<RealFde> &values, std::vector<CNode*> &nodes )
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
			::assign( rcItem, tl + pFirstNode->GetPosition() );
			rcItem += pFirstNode->GetSize();
    
			if( !(rcItem.left() > rc.right() || rcItem.top() > rc.bottom() || 
				  rcItem.right() < rc.left() || rcItem.bottom() < rc.top()) )
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

			if( pFirstNode == pLastNode ) break;
			pFirstNode = pFirstNode->GetNext();
		}
		while( pFirstNode != NULL );
	}

	SimultaneousSorting( values, nodes );
}

/////////////////////////////////////////////////////////////////////////////

int GetNearestNode_Frame( CFrameNode *pFrameNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn );
int GetNearestNode_Line( CLineNode *pLineNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn );
int GetNearestNode_Formula(	CFormulaNode *pFormulaNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn );

/////////////////////////////////////////////////////////////////////////////

int GetNearestNode_Frame( CFrameNode *pFrameNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn )
{
	std::vector<RealFde> values;
	std::vector<CNode*> nodes;

	Arrange_Click_Frame( pFrameNode, rc, tl, nn, values, nodes );

	PointFde ofs;
	for( long i = 0; i < (long) nodes.size(); i++ )
	{
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
			}
			else if( ((CLineNode*) nodes[ i ])->GetChildCount() )
			{
				ofs = tl + nodes[ i ]->GetPosition();
				GetNearestNode_Line( (CLineNode*) nodes[ i ], rc, ofs, nn );
			}
			break;

		default:
			break;
		}
	}
	return (nn.m_value < FLT_MAX ? 0 : -1);
}

int GetNearestNode_Line( CLineNode *pLineNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn )
{
	std::vector<RealFde> values;
	std::vector<CNode*> nodes;

	Arrange_Click_Line( pLineNode, rc, tl, nn, values, nodes );

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
			}
			break;

		case NODE_PLANETEXT:
			if( values[ i ] <= nn.m_value && nodes[ i ]->canBeEdited() )
			{
				nn.m_value = values[ i ];
				nn.m_pNode = nodes[ i ];
			}
			break;

		case NODE_FORMULA:
			ofs = tl + nodes[ i ]->GetPosition();
			GetNearestNode_Formula( (CFormulaNode*) nodes[ i ], rc, ofs, nn );
			break;

		default:
			break;
		}
	}
	return (nn.m_value < FLT_MAX ? 0 : -1);
}

int GetNearestNode_Formula( CFormulaNode *pFormulaNode, 
	const RectFde &rc, const PointFde &tl, Nearest_Node &nn )
{
	std::vector<RealFde> values;
	std::vector<CNode*> nodes;

	Arrange_Click_Formula( pFormulaNode, rc, tl, nn, values, nodes );

	PointFde ofs;
	for( long i = 0; i < (long) nodes.size(); i++ )
	{
		switch( nodes[ i ]->GetType() )
		{
		case NODE_FRAME:
			ofs = tl + nodes[ i ]->GetPosition();
			GetNearestNode_Frame( (CFrameNode*) nodes[ i ], rc, ofs, nn );
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
	return (nn.m_value < FLT_MAX ? 0 : -1);
}

/////////////////////////////////////////////////////////////////////////////

int NearestNode2SelectNode( const RectFde &rc, Nearest_Node &nn, SelectNode &sn )
{
	if( nn.m_pNode != NULL && nn.m_value < FLT_MAX )
	{
		long nPos = 0;

		switch( nn.m_pNode->GetType() )
		{
		case NODE_PLANETEXT:
			nPos = ((CPlaneText*) nn.m_pNode)->getPosFromPoint( nn.m_pt );
			if( nPos == 0 )
			{
				sn.SetNode( nn.m_pNode->GetParent() );
				sn.SetPosition( nn.m_pNode->GetParent()->GetChildIndex( nn.m_pNode ) );
			}
			else if( nPos == ((CPlaneText*) nn.m_pNode)->GetStringLength() )
			{
				sn.SetNode( nn.m_pNode->GetParent() );
				sn.SetPosition( nn.m_pNode->GetParent()->GetChildIndex( nn.m_pNode ) + 1 );
			}
			else
			{
				sn.SetNode( nn.m_pNode );
				sn.SetPosition( nPos );
			}
			return 0;

		case NODE_LINE:
			nPos = ((CLineNode*) nn.m_pNode)->getPosFromPoint( rc, nn.m_pt );
			sn.SetNode( nn.m_pNode );
			sn.SetPosition( nPos );
			return 0;

		default:
			Q_ASSERT( 0 );
			break;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

inline
int PtInSelectNode( SelectNode &sn, Nearest_Node &nn )
{
	if( sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		RectFde rcItem;
		::assign( rcItem, sn.GetNode()->GetAbsolutePosition() );
		rcItem += sn.GetNode()->GetSize();

		if( rcItem.contains( nn.m_pt ) )
		{
			nn.m_pNode = sn.GetNode();
			nn.m_value = 0.0;
			return 0;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveCaret( const PointFde &pt, const RectFde &rc, SelectNode &sn, CFrameNode *pFrameNode )
{
	Nearest_Node nn( pt );
	if( PtInSelectNode( sn, nn ) == 0 )
	{
		SelectNode ssn = sn;
		if( NearestNode2SelectNode( rc, nn, sn ) == 0 )
		{
			Q_ASSERT( ::isValidPosition( sn ) );
			return (ssn == sn ? -1 : 0);
		}
	}
	else if( GetNearestNode_Frame( pFrameNode, rc, pFrameNode->GetAbsolutePosition(), nn ) == 0 )
	{
		SelectNode ssn = sn;
		if( NearestNode2SelectNode( rc, nn, sn ) == 0 )
		{
			Q_ASSERT( isValidPosition( sn ) );
			return (ssn == sn ? -1 : 0);
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

struct UpLevelFrameInfo
{
	CLineNode* pLineNode;
	long nPos;
};

/////////////////////////////////////////////////////////////////////////////

void Check4Half( int fLeft, const PointFde &pt, SelectNode &sn )
{
	if( sn.GetNode()->GetType() == NODE_LINE )
	{
		if( fLeft )
		{
			if( sn.GetPosition() > 0 && sn.GetPosition() < ((CLineNode*) sn.GetNode())->GetChildCount() && 
				((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() )->GetType() == NODE_FORMULA )
			{
				CFormulaNode *pFormulaNode = (CFormulaNode*) ((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() );
				RealFde x = pFormulaNode->GetAbsolutePosition().x() + pFormulaNode->GetSize().width() / 2.0;
				if( x < pt.x() )
					sn.IncPosition();
			}
		}
		else
		{
			CNode *pNode = 0;
			if( sn.GetPosition() && sn.GetPosition() <= ((CLineNode*) sn.GetNode())->GetChildCount() && 
				(pNode = ((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() - 1 )) != NULL &&
				pNode->GetType() == NODE_FORMULA )
			{
				RealFde x = ((CFormulaNode*) pNode)->GetAbsolutePosition().x() + ((CFormulaNode*) pNode)->GetSize().width() / 2.0;
				if( x > pt.x() )
					sn.DecPosition();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void Update( int fLeft, const PointFde &pt, 
	UpLevelFrameInfo &ulfi1, UpLevelFrameInfo &ulfi2, 
	SelectNode &sn1, SelectNode &sn2, SelectInfo &si )
{
	Q_ASSERT( ulfi1.pLineNode->GetParent() == ulfi2.pLineNode->GetParent() );
	if( sn1.GetNode()->GetFrameNode() != ulfi1.pLineNode->GetParent() )
	{
		sn1.SetNode( ulfi1.pLineNode );
		sn1.SetPosition( (fLeft ? ulfi1.nPos : ulfi1.nPos + 1) );
	}
	if( sn2.GetNode()->GetFrameNode() != ulfi2.pLineNode->GetParent() )
	{
		sn2.SetNode( ulfi2.pLineNode );
		sn2.SetPosition( (fLeft ? ulfi2.nPos + 1: ulfi2.nPos) );
	}
	Check4Half( fLeft, pt, sn1 );

	if( fLeft )
	{
		si.GetBegin() = sn1;
		si.GetEnd() = sn2;
	}
	else
	{
		si.GetBegin() = sn2;
		si.GetEnd() = sn1;
	}
	sn2 = sn1;
}

/////////////////////////////////////////////////////////////////////////////

int Update( const PointFde &pt, SelectNode &ssn, SelectNode &sn, SelectInfo &si )
{
	SelectInfo ssi = si;

	if( si.IsSelection() )
	{
		if( si.GetBegin() == sn ) 
			si.GetBegin() = si.GetEnd();
		else if( si.GetEnd() == sn ) 
			si.GetEnd() = si.GetBegin();
		else
			Q_ASSERT( 0 );
		sn = si.GetBegin();
	}
	else
	{
		si.GetBegin() = si.GetEnd() = sn;
	}

	UpLevelFrameInfo ulfi;
	HKMap<CFrameNode*, UpLevelFrameInfo> nodemap;
	ulfi.pLineNode = ssn.GetNode()->GetLineNode( &ulfi.nPos );
	while ( ulfi.pLineNode )
	{
		Q_ASSERT( ulfi.pLineNode->GetParent() );
		Q_ASSERT( ulfi.pLineNode->GetParent()->GetType() == NODE_FRAME );
		if( !ulfi.pLineNode->isReadOnly() )
			nodemap[ (CFrameNode*) ulfi.pLineNode->GetParent() ] = ulfi;
		ulfi.pLineNode = ulfi.pLineNode->GetParent()->GetLineNode( &ulfi.nPos );
	}
	ulfi.pLineNode = sn.GetNode()->GetLineNode( &ulfi.nPos );
	while ( ulfi.pLineNode )
	{
		Q_ASSERT( ulfi.pLineNode->GetParent() );
		Q_ASSERT( ulfi.pLineNode->GetParent()->GetType() == NODE_FRAME );
		CFrameNode *pFrameNode = (CFrameNode*) ulfi.pLineNode->GetParent();
		if( nodemap.is_find( pFrameNode ) )
		{
			int fLeft = 0;
			UpLevelFrameInfo prev, next;
			prev = next = nodemap[ pFrameNode ];

			if( ulfi.pLineNode == prev.pLineNode )
			{
				SizeFde size;
				PointFde point;
				GetCaretInfo( si.GetEnd(), point, size );
				fLeft = (pt.x() < point.x());

				Update( fLeft, pt, prev, ulfi, ssn, sn, si );
				return (ssi == si ? -1 : 0);
			}

			while ( prev.pLineNode && next.pLineNode )
			{
				if( prev.pLineNode == ulfi.pLineNode )
				{
					fLeft = 0;
					break;
				}
				else if( next.pLineNode == ulfi.pLineNode )
				{
					fLeft = 1;
					break;
				}
				prev.pLineNode = (CLineNode*) prev.pLineNode->GetPrev();
				next.pLineNode = (CLineNode*) next.pLineNode->GetNext();
			}

			if( prev.pLineNode == NULL )
				fLeft = 1;
			else if( next.pLineNode == NULL )
				fLeft = 0;
			Update( fLeft, pt, nodemap[ pFrameNode ], ulfi, ssn, sn, si );
			return (ssi == si ? -1 : 0);
		}
		ulfi.pLineNode = ulfi.pLineNode->GetParent()->GetLineNode( &ulfi.nPos );
	}
	Q_ASSERT( 0 );
	return -1;
}


/////////////////////////////////////////////////////////////////////////////

int MoveCaret( int fSel, 
	const PointFde &pt, const RectFde &rc, 
	SelectNode &sn, SelectInfo &si, CFrameNode *pFrameNode )
{
	if( fSel )
	{
		SelectNode ssn = sn;
		if( MoveCaret( pt, rc, ssn, pFrameNode ) == 0 )
		{
			return Update( pt, ssn, sn, si );
		}
	}
	else
	{
		if( si.IsSelection() )
		{
			si.SetEmpty();
		}
		return MoveCaret( pt, rc, sn, pFrameNode );
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int DblClk( const PointFde & /*pt*/, const RectFde & /*rc*/,
        SelectNode &sn, SelectInfo &si, CFrameNode * /*pFrameNode*/ )
{
	if( sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		CLineNode *pLineNode = (CLineNode*) sn.GetNode()->GetParent();
		long nPos = pLineNode->GetChildIndex( sn.GetNode() );
		sn.SetNode( pLineNode );
		sn.SetPosition( nPos );
		si.GetBegin() = sn;
		si.GetEnd() = sn;
		si.GetEnd().IncPosition();
		return 0;
	}
	else if( sn.GetNode()->GetType() == NODE_LINE )
	{
		long nChild = ((CLineNode*) sn.GetNode())->GetChildCount();
		if( nChild > 0 )
		{
			si.GetBegin().SetNode( sn.GetNode() );
			si.GetBegin().SetPosition( 0 );
			si.GetEnd().SetNode( sn.GetNode() );
			si.GetEnd().SetPosition( nChild );
			return 0;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int MoveCaret( CNode *pNode, int isSNBeforeOpNode, SelectNode &sn )
{
	CNode *pNewFocus = 0;
	CLineNode *pNewFocusLine = 0;

	if( pNode )
	{
		if( pNode->canBeEdited() && pNode->canBeMoved() &&
			pNode->GetParent() && pNode->GetParent()->canBeEdited() && pNode->GetParent()->canBeMoved() )
			pNewFocus = pNode;
		else if( pNode->GetType() == NODE_FORMULA )
		{
			for( CFormulaNode *pFormulaNode = (CFormulaNode*) pNode; pFormulaNode != 0; )
			{
				pFormulaNode = pFormulaNode->passAutodetectSingleFormulaChildren();
				if( pFormulaNode->canBeEdited() && pFormulaNode->canBeMoved() &&
					pFormulaNode->GetParent() && pFormulaNode->GetParent()->canBeEdited() && pFormulaNode->GetParent()->canBeMoved() )
				{
					pNewFocus = pFormulaNode;
					break;
				}
				CLineNode *lineNode = pFormulaNode->getFormulaLineChild( 0, 0 );
				pNewFocusLine = 0;
				pFormulaNode = 0;
				if( lineNode )
				{
					CNode *childNode = isSNBeforeOpNode ? lineNode->GetLastChild() : lineNode->GetFirstChild();
					if( childNode )
					{
						if( childNode->canBeEdited() && childNode->canBeMoved() &&
							childNode->GetParent() && childNode->GetParent()->canBeEdited() && childNode->GetParent()->canBeMoved() )
						{
							pNewFocus = childNode;
							break;
						}
						else if( childNode->GetType() == NODE_FORMULA )
							pFormulaNode = (CFormulaNode*) childNode;
					}
					else if( lineNode->canBeEdited() && lineNode->canBeMoved() )
						pNewFocusLine = lineNode;
				}
			}
		}
	}

	if( pNewFocus && pNewFocus->GetParent() )
	{
		sn.SetPosition( pNewFocus->GetParent()->GetChildIndex( pNewFocus ) + (isSNBeforeOpNode ? 1 : 0) );
		sn.SetNode( pNewFocus->GetParent() );
		return 1;
	}
	else if( pNewFocusLine )
	{
		sn.SetPosition( 0 );
		sn.SetNode( pNewFocusLine );
		return 1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
