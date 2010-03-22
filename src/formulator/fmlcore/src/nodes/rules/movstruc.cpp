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
#include "movstruc.h"

/////////////////////////////////////////////////////////////////////////////

int IsNonOneLineNodes( CNode *pNode1, CNode *pNode2 )
{
	Q_ASSERT( pNode1 && pNode2 );

	if( pNode1 == pNode2 )
	{
		return 0;
	}
	else
	{
		if( pNode1->GetType() == NODE_PLANETEXT && pNode2->GetType() == NODE_LINE )
		{
			if( pNode1->GetParent() == pNode2 )
				return 0;
		}
		else if( pNode1->GetType() == NODE_LINE && pNode2->GetType() == NODE_PLANETEXT )
		{
			if( pNode1 == pNode2->GetParent() )
				return 0;
		}
	}

	if( pNode1->GetType() == NODE_LINE )
		pNode1 = pNode1->GetParent();
	else if( pNode1->GetType() == NODE_PLANETEXT )
		pNode1 = pNode1->GetParent()->GetParent();

	if( pNode2->GetType() == NODE_LINE )
		pNode2 = pNode2->GetParent();
	else if( pNode2->GetType() == NODE_PLANETEXT )
		pNode2 = pNode2->GetParent()->GetParent();

	return (pNode1 != pNode2);
}

void UpdateSelectInfo( int fLeft, int fSel, int fWord, int fSel2Left, SelectNode &sn, SelectInfo &si )
{
	if( fSel )
	{
		CNode *pPrev = NULL, *pNext = NULL;
		if( fSel2Left )
		{
			si.GetBegin() = sn;
			pNext = si.GetEnd().GetNode();
			while ( IsNonOneLineNodes( si.GetBegin().GetNode(), pNext ) )
			{
				pPrev = pNext;
				pNext = pNext->GetParent();

				if( pNext->GetParent() == NULL )
				{
					pPrev = NULL;
					si.GetBegin() = sn;
					si.GetEnd() = sn;
					si.GetEnd().IncPosition();
					break;
				}
			}
			if( pPrev != NULL )
			{
				Q_ASSERT( pNext->GetType() == NODE_LINE );
				si.GetEnd().SetNode( pNext );
				si.GetEnd().SetPosition( ((CLineNode*) pNext)->GetChildIndex( pPrev ) + 1 );
			}
		}
		else
		{
			si.GetEnd() = sn;
			pNext = si.GetBegin().GetNode();
			while ( IsNonOneLineNodes( pNext, si.GetEnd().GetNode() ) )
			{
				pPrev = pNext;
				pNext = pNext->GetParent();

				if( pNext->GetParent() == NULL )
				{
					pPrev = NULL;
					si.GetBegin() = sn;
					si.GetEnd() = sn;
					si.GetEnd().DecPosition();
					break;
				}
			}
			if( pPrev != NULL )
			{
				Q_ASSERT( pNext->GetType() == NODE_LINE );
				si.GetBegin().SetNode( pNext );
				si.GetBegin().SetPosition( ((CLineNode*) pNext)->GetChildIndex( pPrev ) );
			}
		}

		if( fWord && fLeft != fSel2Left && fLeft == 0 )
		{
			if( si.GetBegin().GetNode() == si.GetEnd().GetNode() )
			{
				if( si.GetBegin().GetPosition() > si.GetEnd().GetPosition() )
					std::swap( si.GetBegin(), si.GetEnd() );
			}
			else if( si.GetBegin().GetNode()->GetParent() == si.GetEnd().GetNode() )
			{
				long nPos = si.GetBegin().GetNode()->
					GetParent()->GetChildIndex( si.GetBegin().GetNode() );
				if( nPos >= si.GetEnd().GetPosition() )
					std::swap( si.GetBegin(), si.GetEnd() );
			}
			else if( si.GetBegin().GetNode() == si.GetEnd().GetNode()->GetParent() )
			{
				long nPos = si.GetEnd().GetNode()->
					GetParent()->GetChildIndex( si.GetBegin().GetNode() );
				if( si.GetBegin().GetPosition() > nPos )
					std::swap( si.GetBegin(), si.GetEnd() );
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void MotionData::Update()
{
	UpdateSelectInfo( IsLeft(), IsSel(), IsWord(), IsSel2Left(), GetSN(), GetSelectInfo() );
}

/////////////////////////////////////////////////////////////////////////////
