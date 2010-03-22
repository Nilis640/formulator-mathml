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
#include "edit.h"
#include "nav.h"
#include "insert.h"
#include "la_insertstyle.h"

/////////////////////////////////////////////////////////////////////////////

CPlaneText* InsertPlaneText_Internal( SelectNode &sn, 
	QString &strText, long style, RealFde kegl, CNodeInfo &ni, CNodeExInfo &ex_ni, int fRecal = -1 )
{
	QChar decimalPoint = ::FORMULATOR_getCurrentLocale().decimalPoint();
	bool isDecimalPoint = (strText == QString(decimalPoint));
	CPlaneText *pPlaneText = NULL;
	if( ni.isUnicode() )
	{
		if( style == FSTYLE_NONE )
			style = ::getCurrentFormulatorStyle().getStyleByChar( strText[ 0 ] );

		Q_ASSERT( strText.length() );
		if( sn.GetNode()->GetType() == NODE_PLANETEXT )
			BreakPlaneText( sn, 0 );
		Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );

		{
			pPlaneText = new CPlaneText( strText, style, ni, ex_ni );
			((CLineNode*) sn.GetNode())->InsertChild( sn.GetPosition(), pPlaneText );

			sn.IncPosition();
		}
	}
	else
	{
		bool autodetectStyle = false;
		Q_ASSERT( strText.length() );
		if( style == FSTYLE_NONE )
		{
			autodetectStyle = true;
			style = ::getCurrentFormulatorStyle().getStyleByChar( strText[ 0 ] );
		}

		if( sn.GetNode()->GetType() == NODE_PLANETEXT )
		{
			pPlaneText = (CPlaneText*) sn.GetNode();
			if( ((CPlaneText*) pPlaneText)->getFStyle() == FSTYLE_NUMBER && isDecimalPoint )
				style = FSTYLE_NUMBER;
			if( pPlaneText->isNewPlaneText( kegl, style ) )
			{
				BreakPlaneText( sn, 0 );
			}
			else
			{
				pPlaneText->Insert( sn.GetPosition(), strText );
				sn.IncPosition();
			}
		}

		if( sn.GetNode()->GetType() == NODE_LINE )
		{
			bool continueSearch = true;
			CNode *pPrev, *pNext;
			GetPrevNextInLine( sn, pPrev, pNext );
			if( pPrev && pPrev->GetType() == NODE_PLANETEXT )
			{
				if( autodetectStyle && style != FSTYLE_NUMBER )
				{
					if( ((CPlaneText*) pPrev)->getFStyle() == FSTYLE_NUMBER && isDecimalPoint )
						style = FSTYLE_NUMBER;
				}

				if( !((CPlaneText*) pPrev)->isNewPlaneText( kegl, style ) )
				{
					pPlaneText = (CPlaneText*) pPrev;
					pPlaneText->Add( strText );
					continueSearch = false;
				}
			}
			if( continueSearch )
			{
				bool isNewText = true;
				if( pNext && pNext->GetType() == NODE_PLANETEXT )
				{
					if( ((CPlaneText*) pNext)->getFStyle() == FSTYLE_NUMBER && isDecimalPoint )
						style = FSTYLE_NUMBER;
					if( !((CPlaneText*) pNext)->isNewPlaneText( kegl, style ) )
					{
						pPlaneText = (CPlaneText*) pNext;
						pPlaneText->Insert( 0, strText );

						sn.SetNode( pPlaneText );
						sn.SetPosition( (long) strText.length() );

						isNewText = false;
					}
				} 

				if( isNewText )
				{
					pPlaneText = new CPlaneText( strText, style, ni, ex_ni );
					((CLineNode*) sn.GetNode())->InsertChild( sn.GetPosition(), pPlaneText );

					pPlaneText->setTextKeglSize( kegl );

					sn.IncPosition();
				}
			}
		}
	}

	if( fRecal )
		pPlaneText->RecalculateSize( 1 );
	return pPlaneText;
}

CPlaneText* InsertPlaneText_Internal( SelectNode &sn, CExtNode *pNode, int fRecal = -1 )
{
	CPlaneText *pPlaneText = NULL;
	if( sn.GetNode()->GetType() == NODE_PLANETEXT )
	{
		pPlaneText = (CPlaneText*) sn.GetNode();
		BreakPlaneText( sn, 0 );
	}

	if( sn.GetNode()->GetType() == NODE_LINE )
	{
		CNode *pPrev, *pNext;
		GetPrevNextInLine( sn, pPrev, pNext );
		pPlaneText = pNode;
		((CLineNode*) sn.GetNode())->InsertChild( sn.GetPosition(), pPlaneText );
		sn.IncPosition();
	}

	if( fRecal )
		pPlaneText->RecalculateSize( 1 );
	return pPlaneText;
}


/////////////////////////////////////////////////////////////////////////////

CPlaneText* InsertPlaneText( SelectNode &sn, 
	QChar ch, long style, RealFde kegl, CNodeInfo &ni, CNodeExInfo &ex_ni )
{
	Q_ASSERT( !ni.isUnicode() );
	QString sV( 1, ch );
	return InsertPlaneText_Internal( sn, sV, style, kegl, ni, ex_ni );
}

/////////////////////////////////////////////////////////////////////////////

CPlaneText* InsertAlnumPlaneText( SelectNode &sn, QChar ch, RealFde kegl )
{
	long style;
	QString strText;
	CNodeExInfo ex_ni;
	CNodeInfo ni, ignore_ni;
	FXmlBtn* ptrFXmlBtn = 0;

	if( ::getCurrentToolBarSet()->text2DsrButton( QString( 1, ch ), ptrFXmlBtn, ni ) != -1 && 
		ptrFXmlBtn && CNode::UpdateTextNode( *ptrFXmlBtn, strText, style, ignore_ni, ex_ni ) != -1 )
	{
		Q_ASSERT( strText.length() == 1 );
		return InsertPlaneText( sn, strText[ 0 ], style, kegl, ni, ex_ni );
	}
	else
	{
		return InsertPlaneText( sn, ch, -1, kegl, ni, ex_ni );
	}
}

/////////////////////////////////////////////////////////////////////////////

CPlaneText* InsertPlaneText( 
	SelectNode &sn, QString &strText, 
	long style, RealFde kegl, CNodeInfo& ni, CNodeExInfo& ex_ni )
{
	if( ni.isUnicode() )
	{
		if( sn.GetNode()->GetType() == NODE_PLANETEXT ) 
			BreakPlaneText( sn, 0 );

		Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );
		CParentNode *pParentNode = sn.GetNode()->GetParent();

		CNode *pPrev, *pNext;
		GetPrevNextInLine( sn, pPrev, pNext );

		long nLen;
		QString str;
		CPlaneText *pPlaneText = NULL;
		while( (nLen = strText.length()) )
		{
			long nPos = strText.indexOf("\r\n");
			if( nPos == -1 )
			{
				str = strText.mid(0, nLen);
				strText.clear();
			}
			else
			{
				str = strText.mid(0, nPos + 2);
				strText.remove(0, nPos);
			}

			pPlaneText = InsertPlaneText_Internal( sn, str, style, kegl, ni, ex_ni );

			if( nPos != -1 ) 
				BreakLine( sn, 0 );
		}

		pPlaneText->GetParent()->RecalculateSize( 0 );

		if( pPrev ) GlueText( sn, pPrev, 0 );
		if( pNext ) GlueText( sn, pNext->GetPrev(), 0 );

		pParentNode->RecalculateSize( 1 );

		return pPlaneText;
	}
	else
	{
		if( sn.GetNode()->GetType() == NODE_PLANETEXT ) 
			BreakPlaneText( sn, 0 );

		Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );
		CParentNode *pParentNode = sn.GetNode()->GetParent();

		CNode *pPrev, *pNext;
		GetPrevNextInLine( sn, pPrev, pNext );

		//
		long i, cb;
		long nLen;
		QString str;
		QVector<CNode*> texts, lines;

		while( (nLen = strText.length()) )
		{
			long nPos = strText.indexOf( "\r\n" );
			if( nPos == -1 )
			{
				str = strText.mid(0, nLen);
				strText.clear();
			}
			else
			{
				str = strText.mid(0, nPos);
				strText.remove( 0, nPos + 2 );
			}
			lines.push_back( sn.GetNode()->GetLineNode() );

			char ch = 0;
			LFInsertStyle style2analyse;
			long nNewStyle = -1;
			for( cb = 0; ; )
			{
				ch = style2analyse.Analisys( str.midRef(cb) );
				if( ch == 0 ) break;
				if( style == -1 )
					nNewStyle = ::getCurrentFormulatorStyle().getStyleByCharOnly( ch );
				else
					nNewStyle = style;

				QString sV( style2analyse.getText() );
				texts.push_back( InsertPlaneText_Internal( sn, sV, nNewStyle, kegl, ni, ex_ni, 0 ) );

				cb += style2analyse.GetRead();
				style2analyse.reset();
			}

			if( nPos != -1 ) 
				BreakLine( sn, 0 );
		}

		for( i = 0; i < (long) texts.size(); i++ )
		{
			Q_ASSERT( texts[ i ] != NULL );
			texts[ i ]->RecalculateSize( 0 );
		}

		for( i = 0; i < (long) lines.size(); i++ )
		{
			Q_ASSERT( lines[ i ] != NULL );
			lines[ i ]->RecalculateSize( 0 );
		}

		if( pPrev ) GlueText( sn, pPrev, 0 );
		if( pNext ) GlueText( sn, pNext->GetPrev(), 0 );

		pParentNode->RecalculateSize( 1 );

		return (CPlaneText*) (texts.size() ? texts[ texts.size() - 1 ] : NULL);
	}
}

CPlaneText* InsertPlaneText( SelectNode &sn, CExtNode *pNode )
{
	if( sn.GetNode()->GetType() == NODE_PLANETEXT ) 
		BreakPlaneText( sn, 0 );

	Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );
	CParentNode *pParentNode = sn.GetNode()->GetParent();

	CNode *pPrev, *pNext;
	GetPrevNextInLine( sn, pPrev, pNext );

	long i;
	QString str;
	QVector<CNode*> texts, lines;

	lines.push_back( sn.GetNode()->GetLineNode() );
	texts.push_back( InsertPlaneText_Internal( sn, pNode, 0 ) );

	for( i = 0; i < (long) texts.size(); i++ )
	{
		Q_ASSERT( texts[ i ] != NULL );
		texts[ i ]->RecalculateSize( 0 );
	}

	for( i = 0; i < (long) lines.size(); i++ )
	{
		Q_ASSERT( lines[ i ] != NULL );
		lines[ i ]->RecalculateSize( 0 );
	}

	if( pPrev ) GlueText( sn, pPrev, 0 );
	if( pNext ) GlueText( sn, pNext->GetPrev(), 0 );

	pParentNode->RecalculateSize( 1 );

	return (CPlaneText*) (texts.size() ? texts[ texts.size() - 1 ] : NULL);
}

/////////////////////////////////////////////////////////////////////////////

int InsertFromText( CPlaneText *pPlaneText, SelectNode &sn, int fUpCalc )
{
	if( pPlaneText == NULL ) return -1;

	if( sn.GetNode()->GetType() == NODE_PLANETEXT ) BreakPlaneText( sn, 0 );

	Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );

	CNode *pPrev, *pNext;
	GetPrevNextInLine( sn, pPrev, pNext );

	((CLineNode*) sn.GetNode())->InsertChild( sn.GetPosition(), pPlaneText );

	sn.SetPosition( sn.GetPosition() + 1 );

	GlueText( sn, pPrev, 0 );
	if( pNext != NULL && (pPrev == NULL || pPrev->GetParent()->GetChildIndex( pNext ) != -1) )
		GlueText( sn, pNext->GetPrev(), 0 );

	sn.GetNode()->RecalculateSize( fUpCalc );

	return 0;
}

int InsertFromLine( CLineNode *pLineNode, SelectNode &sn, int fUpCalc, SelectInfo *psi )
{
	if( pLineNode == NULL ) return -1;

	if( sn.GetNode()->GetType() == NODE_PLANETEXT ) BreakPlaneText( sn, -1, NULL );

	Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );
	CLineNode *pLineTo = ((CLineNode*) sn.GetNode());

	CNode *pPrev, *pNext;
	GetPrevNextInLine( sn, pPrev, pNext );

	long nCount = pLineNode->GetChildCount();

	int fRecalculateFull = (pLineNode->GetLevel() != pLineTo->GetLevel());

	QVector<CNode*> children;
	pLineNode->RecalculateSizeFull();
	pLineNode->RemoveOrCopyChildren( children );

	if( psi )  psi->GetBegin() = sn;

	pLineTo->InsertChildren( children, sn.GetPosition() );

	sn.SetPosition( sn.GetPosition() + nCount );
	if( psi )  psi->GetEnd() = sn;

	int nRet = GlueText( sn, pPrev, 1, (psi ? &psi->GetBegin() : NULL));

	if( psi )
	{
		if( nRet == 0 )
		{
			psi->GetBegin() = sn;
		}
		else if( pPrev )
		{
			psi->GetBegin().SetNode( pPrev->GetParent() );
			psi->GetBegin().SetPosition( pPrev->GetParent()->GetChildIndex( pPrev ) + 1 );
		}
	}

	if( pPrev && pPrev->GetParent()->GetChildIndex( pNext ) != -1 )
	{
		nRet = GlueText( sn, pNext->GetPrev(), (nRet == -1), (psi ? &psi->GetEnd() : NULL) );

		if( psi )
		{
			if( nRet == 0 )
			{
				psi->GetEnd() = sn;
			}
			else 
			{
				psi->GetEnd().SetNode( pNext->GetParent() );
				psi->GetEnd().SetPosition( pNext->GetParent()->GetChildIndex( pNext ) );
			}
		}
	}

	if( fRecalculateFull )
		pLineTo->RecalculateSizeFull();
	else
		pLineTo->RecalculateSize( 1 );

	if( pLineTo->GetParent() )
		pLineTo->GetParent()->RecalculateSize( fUpCalc );

	return 0;
}

int InsertFromFrame( CFrameNode *pFrameNode, SelectNode &sn, int fUpCalc, SelectInfo *psi )
{
	if( pFrameNode == NULL ) return -1;

	if( pFrameNode->GetChildCount() == 1 )
	{
		Q_ASSERT( pFrameNode->GetFirstChild()->GetType() == NODE_LINE );
		return ::InsertFromLine( (CLineNode*) pFrameNode->GetChild( 0 ), sn, fUpCalc, psi );
	}
	else
	{
		Q_ASSERT( pFrameNode->GetChildCount() > 0 );

		BreakLine( sn, 0 );

		CNode *pPrev = sn.GetNode()->GetPrev();
		CNode *pNext = sn.GetNode();
		Q_ASSERT( pPrev );
		Q_ASSERT( pNext );

		CParentNode *pParentNode = sn.GetNode()->GetParent();

		int fRecalculateFull = (pFrameNode->GetLevel() != pParentNode->GetLevel());
		
		QVector<CNode*> children;
		pFrameNode->RecalculateSizeFull();
		pFrameNode->RemoveOrCopyChildren( children );

		if( psi )
		{
			psi->GetBegin().SetNode( children[ 0 ] );
			psi->GetBegin().SetPosition( 0 );
			psi->GetEnd() = sn;
		}

		pParentNode->InsertChildren( children, 
			pParentNode->GetChildIndex( sn.GetNode() ) );

		if( pNext->GetPrev() == pPrev )
		{
			Q_ASSERT( 0 );
		}
		else
		{
			if( GlueLine( sn, pNext->GetPrev(), 0, NULL ) == 0 )
				if( psi ) 
					psi->GetEnd() = sn;
			if( GlueLine( sn, pPrev, 0, NULL ) == 0 )
				if( psi ) 
					psi->GetBegin() = sn;
		}

		if( fRecalculateFull )
			pParentNode->RecalculateSizeFull();
		else
			pParentNode->RecalculateSize( 0 );

		if( pParentNode->GetParent() )
			pParentNode->GetParent()->RecalculateSize( fUpCalc );
		return 0;
	}
}

int InsertFromFormula( CFormulaNode *pFormulaNode, SelectNode &sn, int fUpCalc )
{
	if( pFormulaNode == NULL ) return -1;

	if( sn.GetNode()->GetType() == NODE_PLANETEXT ) BreakPlaneText( sn, 0 );

	Q_ASSERT( sn.GetNode()->GetType() == NODE_LINE );

	((CLineNode*) sn.GetNode())->InsertChild( sn.GetPosition(), pFormulaNode );

	pFormulaNode->RecalculateSize( fUpCalc );

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
