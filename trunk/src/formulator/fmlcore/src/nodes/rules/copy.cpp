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

#include "copy.h"
#include "nav.h"
#include "insert.h"
#include "edit.h"

/////////////////////////////////////////////////////////////////////////////

int RemoveToFrame( CFrameNode *pFrameNode, SelectNode &sn, 
        SelectInfo &si, int /*fUpCalc*/, SelectNode *psn )
{
	if( pFrameNode == NULL ) return -1;

	Q_ASSERT( si.GetBegin().GetNode() != NULL );
	Q_ASSERT( si.GetEnd().GetNode() != NULL );
	Q_ASSERT( si.IsSelection() );

#ifdef _DEBUG
	Q_ASSERT( si.GetBegin().GetNode()->GetFrameNode() == si.GetEnd().GetNode()->GetFrameNode() );

	if( si.GetBegin().GetNode() == si.GetEnd().GetNode() )
	{
		Q_ASSERT( si.GetBegin().GetPosition() < si.GetEnd().GetPosition() );
	}
	else if( si.GetBegin().GetNode()->GetParent() == si.GetEnd().GetNode() )
	{
		Q_ASSERT( ((CParentNode*) si.GetEnd().GetNode())->GetChildIndex( si.GetBegin().GetNode() ) < si.GetEnd().GetPosition() );
	}
	else if( si.GetBegin().GetNode() == si.GetEnd().GetNode()->GetParent() )
	{
		Q_ASSERT( si.GetBegin().GetPosition() <= ((CParentNode*) si.GetBegin().GetNode())->GetChildIndex( si.GetEnd().GetNode() ) );
	}
	else
	{
		PointFde ptb = si.GetBegin().GetNode()->GetAbsolutePosition();
		PointFde pte = si.GetEnd().GetNode()->GetAbsolutePosition();
		Q_ASSERT( ptb.y() < pte.y() || ptb.x() < pte.x() );
	}
#endif //_DEBUG

	BreakLine( si.GetEnd(), 0, psn );
	BreakLine( si.GetBegin(), 0, psn );

	CNode *pPrev = si.GetBegin().GetNode()->GetPrev();
	CNode *pNext = si.GetEnd().GetNode();

	QVector<CNode*> children;
	CParentNode *pParentNode = si.GetBegin().GetNode()->GetParent();
	pParentNode->RemoveOrCopyChildren( children, 
		pParentNode->GetChildIndex( si.GetBegin().GetNode() ), 
		pParentNode->GetChildIndex( si.GetEnd().GetNode() ), -1 );

	sn = si.GetEnd();
	si.SetEmpty();

	if( pFrameNode->GetChildCount() ) pFrameNode->DeleteAllChild();
	pFrameNode->InsertChildren( children, 0 );
	pFrameNode->RecalculateSize( 1 );

	if( pNext == NULL || pNext->GetPrev() == pPrev ) 
	{
		GlueLine( sn, pPrev, 0, psn );
	}
	else
	{
		GlueLine( sn, pNext->GetPrev(), 0, psn );
		GlueLine( sn, pPrev, 0, psn );
	}

	pParentNode->RecalculateSize( 0 );
	if( pParentNode->GetParent() )
		pParentNode->GetParent()->RecalculateSize( 1 );
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int CopyToFrame( CFrameNode *pFrameNode, SelectNode& /*sn*/,
	SelectInfo &si, int fUpCalc )
{
	CFrameNode *pParentFrame = si.GetBegin().GetNode()->GetFrameNode();
	Q_ASSERT( pParentFrame == si.GetEnd().GetNode()->GetFrameNode() );

	SelectInfo ssi;

	CFrameNode *pFrameCopy = new CFrameNode( pParentFrame );
	if( si.GetBegin().GetNode()->GetType() == NODE_PLANETEXT )
	{
		Q_ASSERT( pParentFrame == si.GetBegin().GetNode()->GetParent()->GetParent() );

		long nText = si.GetBegin().GetNode()->GetParent()->GetChildIndex( si.GetBegin().GetNode() );
		long nLine = pParentFrame->GetChildIndex( si.GetBegin().GetNode()->GetParent() );
		ssi.GetBegin().SetNode( ((CParentNode*) pFrameCopy->GetChild( nLine ))->GetChild( nText ) );
		ssi.GetBegin().SetPosition( si.GetBegin().GetPosition() );
	}
	else
	{
		Q_ASSERT( pParentFrame == si.GetBegin().GetNode()->GetParent() );

		long nLine = pParentFrame->GetChildIndex( si.GetBegin().GetNode() );
		ssi.GetBegin().SetNode( pFrameCopy->GetChild( nLine ) );
		ssi.GetBegin().SetPosition( si.GetBegin().GetPosition() );
	}
	if( si.GetEnd().GetNode()->GetType() == NODE_PLANETEXT )
	{
		Q_ASSERT( pParentFrame == si.GetEnd().GetNode()->GetParent()->GetParent() );

		long nText = si.GetEnd().GetNode()->GetParent()->GetChildIndex( si.GetEnd().GetNode() );
		long nLine = pParentFrame->GetChildIndex( si.GetEnd().GetNode()->GetParent() );
		ssi.GetEnd().SetNode( ((CParentNode*) pFrameCopy->GetChild( nLine ))->GetChild( nText ) );
		ssi.GetEnd().SetPosition( si.GetEnd().GetPosition() );
	}
	else
	{
		Q_ASSERT( pParentFrame == si.GetEnd().GetNode()->GetParent() );

		long nLine = pParentFrame->GetChildIndex( si.GetEnd().GetNode() );
		ssi.GetEnd().SetNode( pFrameCopy->GetChild( nLine ) );
		ssi.GetEnd().SetPosition( si.GetEnd().GetPosition() );
	}
	SelectNode ssn;
	int nRet = ::RemoveToFrame( pFrameNode, ssn, ssi, fUpCalc );
	delete pFrameCopy;
	return nRet;
}

/////////////////////////////////////////////////////////////////////////////
