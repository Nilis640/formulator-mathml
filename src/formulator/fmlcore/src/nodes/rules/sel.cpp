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

#include "../node.h"
#include "sel.h"

/////////////////////////////////////////////////////////////////////////////

int DrawRect( void* pRect, void* pVec )
{
	Q_ASSERT( pRect != NULL );
	Q_ASSERT( pVec != NULL );

	((std::vector<RectFde>*) pVec)->push_back( *((RectFde*) pRect) );
	return -1;
}

int TestRect( void* pRect, void* pPoint )
{
	Q_ASSERT( pRect != NULL );
	Q_ASSERT( pPoint != NULL );

	return (((RectFde*) pRect)->contains(*((PointFde*)pPoint)) ? 0 : -1);
}

typedef int (*RULES_SELFUNC)( void* pRect, void* pPoint );

/////////////////////////////////////////////////////////////////////////////

int Call_SelFunc( const RectFde &rc, 
	SelectNode &sb, SelectNode &se, void* pParam, RULES_SELFUNC pSelFunc );

int Line_SelFunc( const RectFde &rc, PointFde &tl, 
	SelectNode &sb, SelectNode &se, void* pParam, RULES_SELFUNC pSelFunc )
{
	if( sb == se )
	{
		if( ((CParentNode*) sb.GetNode())->GetChildCount() == 0 )
		{
			// пустая линия
			RectFde rcItem;
			PointFde res = tl + sb.GetNode()->GetPosition();
			rcItem.setLeft( res.x() );
			rcItem.setRight( rcItem.left() + sb.GetNode()->GetSize().width() );
			rcItem.setTop( res.y() );
			rcItem.setBottom( rcItem.top() + sb.GetNode()->GetSize().height() );

			if( (*pSelFunc)( (void*) &rcItem, pParam ) == 0 ) return 0;
		}
	}
	else
	{
		if( Call_SelFunc( rc, sb, se, pParam, pSelFunc ) == 0 ) return 0;
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int Call_SelFunc( const RectFde &rc, 
	SelectNode &sb, SelectNode &se, void* pParam, RULES_SELFUNC pSelFunc )
{
	if( sb == se ) return -1;

	RectFde rcItem;
	if( sb.GetNode() == se.GetNode() )
	{
		if( sb.GetNode()->GetType() == NODE_PLANETEXT )
		{
			::assign( rcItem, sb.GetNode()->GetParent()->GetAbsolutePosition() );
			RectFde rcItem1 = ((CPlaneText*) sb.GetNode())->getDrawSize( 0, sb.GetPosition() );
			RectFde rcItem2 = ((CPlaneText*) sb.GetNode())->getDrawSize( 0, se.GetPosition() );
			rcItem.setLeft( rcItem.left() + sb.GetNode()->GetPosition().x() + rcItem1.right() );
			rcItem.setRight( rcItem.right() + sb.GetNode()->GetPosition().x() + rcItem2.right() );
			rcItem.setBottom( rcItem.top() + sb.GetNode()->GetParent()->GetSize().height() );
		}
		else if( sb.GetNode()->GetType() == NODE_LINE )
		{
			::assign( rcItem, se.GetNode()->GetAbsolutePosition() );
			if( sb.GetPosition() < ((CLineNode*) sb.GetNode())->GetChildCount() )
				rcItem.setLeft( rcItem.left() + ((CLineNode*) sb.GetNode())->GetChild( sb.GetPosition() )->GetPosition().x() );
			if( se.GetPosition() < ((CLineNode*) se.GetNode())->GetChildCount() )
				rcItem.setRight( rcItem.right() + ((CLineNode*) se.GetNode())->GetChild( se.GetPosition() )->GetPosition().x() );
			else
				rcItem.setRight( rcItem.right() + se.GetNode()->GetSize().width() );
			rcItem.setBottom( rcItem.bottom() + se.GetNode()->GetSize().height() );
		}
		else
		{
			Q_ASSERT( 0 );
		}

		if( (*pSelFunc)( (void*) &rcItem, pParam ) == 0 ) return 0;
	}
	else
	{
		if( sb.GetNode() == se.GetNode()->GetParent() )
		{
			Q_ASSERT( sb.GetNode()->GetType() == NODE_LINE );
			Q_ASSERT( se.GetNode()->GetType() == NODE_PLANETEXT );

			::assign( rcItem, sb.GetNode()->GetAbsolutePosition() );
			if( sb.GetPosition() < ((CLineNode*) sb.GetNode())->GetChildCount() )
				rcItem.setLeft( rcItem.left() + ((CLineNode*) sb.GetNode())->GetChild( sb.GetPosition() )->GetPosition().x() );
			rcItem.setRight( rcItem.right() + se.GetNode()->GetPosition().x() + 
				((CPlaneText*) se.GetNode())->getDrawSize( 0, se.GetPosition() ).width() );
			rcItem.setBottom( rcItem.bottom() + sb.GetNode()->GetSize().height() );

			if( (*pSelFunc)( (void*) &rcItem, pParam ) == 0 ) return 0;
		}
		else if( sb.GetNode()->GetParent() == se.GetNode() )
		{
			Q_ASSERT( sb.GetNode()->GetType() == NODE_PLANETEXT );
			Q_ASSERT( se.GetNode()->GetType() == NODE_LINE );

			::assign( rcItem, se.GetNode()->GetAbsolutePosition() );
			rcItem.setLeft( rcItem.left() + sb.GetNode()->GetPosition().x() + 
				((CPlaneText*) sb.GetNode())->getDrawSize( 0, sb.GetPosition() ).width() );
			if( se.GetPosition() < ((CLineNode*) se.GetNode())->GetChildCount() )
				rcItem.setRight( rcItem.right() + ((CLineNode*) se.GetNode())->GetChild( se.GetPosition() )->GetPosition().x() );
			else
				rcItem.setRight( rcItem.right() + se.GetNode()->GetSize().width() );
			rcItem.setBottom( rcItem.bottom() + se.GetNode()->GetSize().height() );

			if( (*pSelFunc)( (void*) &rcItem, pParam ) == 0 ) return 0;
		}
		else if( sb.GetNode()->GetParent() == se.GetNode()->GetParent() && sb.GetNode()->GetType() == NODE_PLANETEXT )
		{
			Q_ASSERT( sb.GetNode()->GetType() == NODE_PLANETEXT );
			Q_ASSERT( se.GetNode()->GetType() == NODE_PLANETEXT );

			::assign( rcItem, sb.GetNode()->GetParent()->GetAbsolutePosition() );
			rcItem.setLeft( rcItem.left() + sb.GetNode()->GetPosition().x() + 
				((CPlaneText*) sb.GetNode())->getDrawSize( 0, sb.GetPosition() ).width() );
			rcItem.setRight( rcItem.right() + se.GetNode()->GetPosition().x() + 
				((CPlaneText*) se.GetNode())->getDrawSize( 0, se.GetPosition() ).width() );
			rcItem.setBottom( rcItem.bottom() + se.GetNode()->GetParent()->GetSize().height() );

			if( (*pSelFunc)( (void*) &rcItem, pParam ) == 0 ) return 0;
		}
		else
		{
			CNode *pFirstLine = (sb.GetNode()->GetType() == NODE_PLANETEXT ? sb.GetNode()->GetParent() : sb.GetNode());
			CNode *pLastLine = (se.GetNode()->GetType() == NODE_PLANETEXT ? se.GetNode()->GetParent() : se.GetNode());

			if( pFirstLine->GetAbsolutePosition().y() > rc.bottom() ||
				pLastLine->GetAbsolutePosition().y() + pLastLine->GetSize().height() < rc.top() )
			{
				return -1;
			}

			PointFde tl = pFirstLine->GetParent()->GetAbsolutePosition();
			CNode *pNext = pFirstLine;
			while ( tl.y() + pNext->GetPosition().y() + pNext->GetSize().height() < rc.top() )
			{
				if( (pNext = pNext->GetNext()) == NULL ) return -1;
			}

			SelectNode ssb, sse;

			if( pFirstLine == pNext )
			{
				ssb = sb;
			}
			else
			{
				ssb.SetNode( pNext );
				ssb.SetPosition( 0 );
			}
			sse.SetNode( pNext );
			sse.SetPosition( ((CParentNode*) pNext)->GetChildCount() );
			if( Line_SelFunc( rc, tl, ssb, sse, pParam, pSelFunc ) == 0 ) return 0;
			if( (pNext = pNext->GetNext()) == NULL ) return -1;

			while ( pNext != pLastLine && (tl.y() + pNext->GetPosition().y()) < rc.bottom() )
			{
				ssb.SetNode( pNext );
				ssb.SetPosition( 0 );
				sse.SetNode( pNext );
				sse.SetPosition( ((CParentNode*) pNext)->GetChildCount() );
				if( Line_SelFunc( rc, tl, ssb, sse, pParam, pSelFunc ) == 0 ) return 0;
				if( (pNext = pNext->GetNext()) == NULL ) return -1;
			}

			ssb.SetNode( pNext );
			ssb.SetPosition( 0 );
			if( pLastLine == pNext )
			{
				sse = se;
			}
			else
			{
				sse.SetNode( pNext );
				sse.SetPosition( ((CParentNode*) pNext)->GetChildCount() );
			}
			if( Line_SelFunc( rc, tl, ssb, sse, pParam, pSelFunc ) == 0 ) return 0;
		}
	}
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int DrawSelection( const RectFde &rc, SelectInfo &si, std::vector<RectFde>& sel )
{
	sel.clear();
	if( si.IsSelection() )
		return Call_SelFunc( rc, si.GetBegin(), si.GetEnd(), (void*) &sel, &DrawRect );
	return -1;
}

int TestSelection( const RectFde &rc, SelectInfo &si, const PointFde &pt )
{
	if( si.IsSelection() )
		return Call_SelFunc( rc, si.GetBegin(), si.GetEnd(), (void*) &pt, &TestRect );
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

void FillMap( SelectNode &sn, HKMap<CNode*, long> &nodemap )
{
	CNode *pNode = sn.GetNode();
	long nPos = sn.GetPosition();
	while ( pNode )
	{
		if( pNode->GetType() == NODE_PLANETEXT )
		{
			nodemap[ pNode ] = nPos;
			pNode = pNode->GetLineNode( &nPos );
		}
		else
		{
			nodemap[ pNode ] = nPos;
			pNode = pNode->GetParent()->GetLineNode( &nPos );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

int DragInSelection( SelectNode &sn, SelectInfo &si )
{
	if( !si.IsSelection() || sn.IsEmpty() ) return -1;
	if( si.GetBegin() == sn || si.GetEnd() == sn ) return 0;

	HKMap<CNode*, long> sbmap, semap;
	FillMap( si.GetBegin(), sbmap );
	FillMap( si.GetEnd(), semap );

	CNode *pNode = sn.GetNode();
	long nPos = sn.GetPosition();

	CNode *pSbNode = NULL, *pSeNode = NULL;
	long *pnSbPos = NULL, *pnSePos = NULL;
	long nSbPos = 0, nSePos = 0;
	while ( pNode != NULL )
	{
		if( pnSbPos == NULL )
		{
			pnSbPos = sbmap.is_find( pNode );
			pSbNode = pNode;
			nSbPos = nPos;
		}
		if( pnSePos == NULL )
		{
			pnSePos = semap.is_find( pNode );
			pSeNode = pNode;
			nSePos = nPos;
		}

		if( pnSbPos != NULL && pnSePos != NULL )
		{
			if( *pnSbPos <= nSbPos && nSePos < *pnSePos )
				return 0;
			return -1;
		}
		else if( pnSbPos != NULL )
		{
			if( *pnSbPos <= nSbPos )
				return 0;
			return -1;
		}
		else if( pnSePos != NULL )
		{
			if( nSePos < *pnSePos )
				return 0;
			return -1;
		}

		if( pNode->GetType() == NODE_PLANETEXT )
			pNode = pNode->GetLineNode( &nPos );
		else
			pNode = pNode->GetParent()->GetLineNode( &nPos );
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
