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

#include "appstyle.h"
#include "edit.h"

union RULES_APPLYVALUE
{
	RULES_APPLYVALUE( long v ) : style( v ) {};
	RULES_APPLYVALUE( RealFde v ) : kegl( v ) {};

	long	style;
	RealFde	kegl;
};

typedef int (*RULES_APPLYFUNC)( 
	RULES_APPLYVALUE value, 
	CPlaneText *pPlaneText, long fr, long to, SelectInfo *psi );

/////////////////////////////////////////////////////////////////////////////

int wouldChangeStyleOnMathStyleAutodetect( CPlaneText *pPlaneText )
{
	if( pPlaneText->isUnicode() )
	{
		//pPlaneText->setFStyle( FSTYLE_UNICODE_DEFAULT, ::getFontStyle( FSTYLE_UNICODE_DEFAULT ) );
		return 1;
	}
	else
	{
		for( long i = 0; i < pPlaneText->GetStringLength(); i++ )
		{
			long nstyle = ::getCurrentFormulatorStyle().getStyleByCharOnly( pPlaneText->GetString().at(i) );
			if( pPlaneText->getFStyle() != nstyle )
				return 1;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int ApplyUserOther_Style( 
	RULES_APPLYVALUE value, 
	CPlaneText *pPlaneText, long fr, long to, SelectInfo *psi )
{
	Q_ASSERT( pPlaneText != NULL );
	Q_ASSERT( fr > -1 && fr < pPlaneText->GetStringLength() );
	Q_ASSERT( to > -2 && to <= pPlaneText->GetStringLength() );

	if( to == -1 )
		to = pPlaneText->GetStringLength();

	if( value.style == FSTYLE_NONE )
	{
		if( pPlaneText->isUnicode() )
		{
			pPlaneText->setFStyle( FSTYLE_UNICODE_DEFAULT, ::getFontStyle( FSTYLE_UNICODE_DEFAULT ) );
		}
		else
		{
			for( long i = fr; i < to; i++ )
			{
				long nstyle = ::getCurrentFormulatorStyle().getStyleByCharOnly( pPlaneText->GetString().at(i) );
				if( pPlaneText->getFStyle() != nstyle )
				{
					if( i )
					{
						SelectNode sn( pPlaneText, i );
						BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
						Q_ASSERT( pPlaneText->GetNext() != NULL );
						Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
						pPlaneText = (CPlaneText*) pPlaneText->GetNext();

						to -= i;
						i = 0;
					}

					pPlaneText->setFStyle( nstyle, ::getFontStyle( nstyle ) );
				}
			}
		}
	}
	else
	{
		if( fr == 0 && to == pPlaneText->GetStringLength() )
		{
		}
		else if( fr > 0 && to == pPlaneText->GetStringLength() )
		{
			SelectNode sn( pPlaneText, fr );
			BreakPlaneText( sn, 0, (psi ? &psi->GetBegin() : NULL) );
			Q_ASSERT( pPlaneText->GetNext() != NULL );
			Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
			pPlaneText = (CPlaneText*) pPlaneText->GetNext();
		}
		else if( fr == 0 && to < pPlaneText->GetStringLength() )
		{
			SelectNode sn( pPlaneText, to );
			BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
			Q_ASSERT( pPlaneText->GetNext() != NULL );
			Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
		}
		else
		{
			SelectNode sn( pPlaneText, fr );
			BreakPlaneText( sn, 0, (psi ? &psi->GetBegin() : NULL) );
			Q_ASSERT( pPlaneText->GetNext() != NULL );
			Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
			pPlaneText = (CPlaneText*) pPlaneText->GetNext();

			sn.SetNode( pPlaneText );
			sn.SetPosition( to - fr );
			BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
			Q_ASSERT( pPlaneText->GetNext() != NULL );
			Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
		}

		pPlaneText->setFStyle( value.style, ::getFontStyle( value.style ) );
	}

	pPlaneText->RecalculateSize( 0 );
	return 0;
}

int ApplyUserOther_Kegl( 
	RULES_APPLYVALUE value, 
	CPlaneText *pPlaneText, long fr, long to, SelectInfo *psi )
{
	Q_ASSERT( pPlaneText != NULL );
	Q_ASSERT( fr > -1 && fr < pPlaneText->GetStringLength() );
	Q_ASSERT( to > -2 && to <= pPlaneText->GetStringLength() );

	if( to == -1 )
		to = pPlaneText->GetStringLength();

	if( fr == 0 && to == pPlaneText->GetStringLength() )
	{
	}
	else if( fr > 0 && to == pPlaneText->GetStringLength() )
	{
		// From string break
		SelectNode sn( pPlaneText, fr );
		BreakPlaneText( sn, 0, (psi ? &psi->GetBegin() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
		pPlaneText = (CPlaneText*) pPlaneText->GetNext();
	}
	else if( fr == 0 && to < pPlaneText->GetStringLength() )
	{
		SelectNode sn( pPlaneText, to );
		BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
	}
	else
	{
		SelectNode sn( pPlaneText, fr );
		BreakPlaneText( sn, 0, (psi ? &psi->GetBegin() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
		pPlaneText = (CPlaneText*) pPlaneText->GetNext();

		sn.SetNode( pPlaneText );
		sn.SetPosition( to - fr );
		BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
	}

	pPlaneText->setTextKeglSize( value.kegl );
	pPlaneText->RecalculateSize( 0 );
	return 0;
}

int ApplyUserOther_Kegl_IncDec(
	RULES_APPLYVALUE value, 
	CPlaneText *pPlaneText, long fr, long to, SelectInfo *psi )
{
	Q_ASSERT( pPlaneText != NULL );
	Q_ASSERT( fr > -1 && fr < pPlaneText->GetStringLength() );
	Q_ASSERT( to > -2 && to <= pPlaneText->GetStringLength() );

	if( to == -1 )
		to = pPlaneText->GetStringLength();

	if( fr == 0 && to == pPlaneText->GetStringLength() )
	{
	}
	else if( fr > 0 && to == pPlaneText->GetStringLength() )
	{
		SelectNode sn( pPlaneText, fr );
		BreakPlaneText( sn, 0, (psi ? &psi->GetBegin() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
		pPlaneText = (CPlaneText*) pPlaneText->GetNext();
	}
	else if( fr == 0 && to < pPlaneText->GetStringLength() )
	{
		SelectNode sn( pPlaneText, to );
		BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
	}
	else
	{
		SelectNode sn( pPlaneText, fr );
		BreakPlaneText( sn, 0, (psi ? &psi->GetBegin() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
		pPlaneText = (CPlaneText*) pPlaneText->GetNext();

		sn.SetNode( pPlaneText );
		sn.SetPosition( to - fr );
		BreakPlaneText( sn, 0, (psi ? &psi->GetEnd() : NULL) );
		Q_ASSERT( pPlaneText->GetNext() != NULL );
		Q_ASSERT( pPlaneText->GetNext()->GetType() == NODE_PLANETEXT );
	}

	pPlaneText->setTextKeglSize( pPlaneText->getTextKeglSize() + value.kegl );
	pPlaneText->RecalculateSize( 0 );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int ApplyUserOther_Value( RULES_APPLYFUNC pFunc, 
	RULES_APPLYVALUE value, CParentNode *pParentNode, long fr, long to, SelectInfo *psi )
{
	Q_ASSERT( pParentNode != NULL );
	if( pParentNode->GetChildCount() == 0 )
		return -1;

	Q_ASSERT( fr > -1 && fr < pParentNode->GetChildCount() );
	Q_ASSERT( to > -2 && to <= pParentNode->GetChildCount() );

	CNode *pFr = pParentNode->GetChild( fr );
	CNode *pTo = (to == -1 || to == pParentNode->GetChildCount() ? NULL : pParentNode->GetChild( to ));
	while( pFr != pTo )
	{
		if( pFr->GetType() == NODE_PLANETEXT )
		{
			(*pFunc)( value, (CPlaneText*) pFr, 0, -1, psi );
		}
		else
		{
			ApplyUserOther_Value( pFunc, value, (CParentNode*) pFr, 0, -1, psi );
		}
		pFr = pFr->GetNext();
	}

	if( pParentNode->GetType() == NODE_LINE )
	{
		GlueTextInLine( (CLineNode*) pParentNode, psi );
	}

	pParentNode->RecalculateSize( 0 );
	return -1;
}

/////////////////////////////////////////////////////////////////////////////

int ApplyUserOther_Value( RULES_APPLYFUNC pFunc, 
	RULES_APPLYVALUE value, SelectNode &sn, SelectInfo &si )
{
	if( !si.IsSelection() ) return -1;

	Q_ASSERT( si.GetBegin().GetNode()->GetFrameNode() == si.GetEnd().GetNode()->GetFrameNode() );

	Q_ASSERT( sn == si.GetBegin() || sn == si.GetEnd() );
	int fSel2Left = (sn == si.GetBegin());

	if( si.GetBegin().GetNode()->GetType() == NODE_PLANETEXT )
	{
		BreakPlaneText( si.GetBegin(), 0, &si.GetEnd() );
	}

	if( si.GetEnd().GetNode()->GetType() == NODE_PLANETEXT )
	{
		BreakPlaneText( si.GetEnd(), 0, NULL );
	}

	Q_ASSERT( si.GetBegin().GetNode()->GetType() == NODE_LINE );
	CLineNode *pLineBegin = (CLineNode*) si.GetBegin().GetNode();
	long nPosBegin = si.GetBegin().GetPosition();

	Q_ASSERT( si.GetEnd().GetNode()->GetType() == NODE_LINE );
	CLineNode *pLineEnd = (CLineNode*) si.GetEnd().GetNode();
	long nPosEnd = si.GetEnd().GetPosition();

	if( pLineBegin == pLineEnd )
	{
		ApplyUserOther_Value( pFunc, value, pLineBegin, nPosBegin, nPosEnd, &si );
	}
	else
	{
		ApplyUserOther_Value( pFunc, value, pLineBegin, nPosBegin, -1, &si );
		pLineBegin = (CLineNode*) pLineBegin->GetNext();
		while ( pLineBegin != pLineEnd )
		{
			ApplyUserOther_Value( pFunc, value, pLineBegin, 0, -1, NULL );
			pLineBegin = (CLineNode*) pLineBegin->GetNext();
			Q_ASSERT( pLineBegin != NULL );
		}
		ApplyUserOther_Value( pFunc, value, pLineBegin, 0, nPosEnd, &si );
	}

	sn = (fSel2Left ? si.GetBegin() : si.GetEnd());
	Q_ASSERT( sn == si.GetBegin() || sn == si.GetEnd() );
	pLineBegin->GetParent()->RecalculateSize( 1 );
	return 0;
}

/////////////////////////////////////////////////////////////////////////////

int ApplyUserOtherStyle( long style, SelectNode &sn, SelectInfo &si )
{
	return ApplyUserOther_Value( &ApplyUserOther_Style, RULES_APPLYVALUE( style ), sn, si );
}

int ApplyUserOtherKegl( RealFde kegl, SelectNode &sn, SelectInfo &si )
{
	return ApplyUserOther_Value( &ApplyUserOther_Kegl, RULES_APPLYVALUE( kegl ), sn, si );
}

int ApplyUserOtherKegl_IncDec( RealFde kegl, SelectNode &sn, SelectInfo &si )
{
	return ApplyUserOther_Value( &ApplyUserOther_Kegl_IncDec, RULES_APPLYVALUE( kegl ), sn, si );
}

/////////////////////////////////////////////////////////////////////////////

RealFde GetCurrentKegl( SelectNode &sn )
{
	if( sn.GetNode()->GetType() == NODE_PLANETEXT )
		return ((CPlaneText*) sn.GetNode())->getTextKeglSize();

	if( sn.GetNode()->GetType() == NODE_LINE )
	{
		if( sn.GetPosition() == 0 || ((CLineNode*) sn.GetNode())->GetChildCount() == 0 )
			return ::getKegl( sn.GetNode()->GetLevel() );

		CNode *pNode = ((CLineNode*) sn.GetNode())->GetChild( sn.GetPosition() - 1 );
		Q_ASSERT( pNode != NULL );
		if( pNode == NULL )
			return ::getKegl( sn.GetNode()->GetLevel() );

		return (pNode->GetType() == NODE_PLANETEXT ?
			((CPlaneText*) pNode)->getTextKeglSize() : ::getKegl( pNode->GetLevel() ));
	}

	Q_ASSERT( 0 );
	return ::getKegl( FTEXT_HEIGHT_FRAME );
}

/////////////////////////////////////////////////////////////////////////////
