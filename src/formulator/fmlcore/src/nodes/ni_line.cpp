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

#include <float.h>
#include "node.h"
#include "dump/dumptags.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineNode::CLineNode() 
	: CParentNode( NODE_LINE )
	, max_cy_notstretchy( 0.0 )
{
}

CLineNode::CLineNode( CLineNode* pLineNode ) 
	: CParentNode( pLineNode )
	, max_cy_notstretchy( 0.0 )
{
}

CLineNode::CLineNode( CLineNode* pLineNode, long nfr, long nto ) 
	: CParentNode( pLineNode, nfr, nto )
	, max_cy_notstretchy( 0.0 )
{
}

//////////////////////////////////////////////////////////////////////

CLineNode::~CLineNode()
{
}

//////////////////////////////////////////////////////////////////////

void CLineNode::Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const
{
	CParentNode::Store( ar, sn, bracketSn );

	// store positon
	qint32 nLong = (sn.GetNode() == this);
	ar << nLong;
	if( nLong )
	{
		nLong = sn.GetPosition();
		ar << nLong;
	}

	ar << max_cy_notstretchy;

	StoreBrackets( ar, bracketSn );
}

void CLineNode::Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad )
{
	CParentNode::Load( ar, sn, bracketSn, isLeftBrLoad );

	// load positon
	qint32 nLong;
	ar >> nLong;
	if( nLong )
	{
		sn.SetNode( this );
		ar >> nLong;
		sn.SetPosition( nLong );
	}

	ar >> max_cy_notstretchy;

	LoadBrackets( ar, bracketSn, isLeftBrLoad );
}

///////////////////////////////////////////////////////////////////////////////

int CLineNode::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc )
{
	if( isHidden() ) return 0;

	if( GetChildCount() )
	{
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

		// draw brackets
		DrawBrackets( fde, tl, rc );
	}
	else if( !fde.IsPrinting() )
	{
		fde.DrawRectangle( tl.x(), tl.y(), GetSize().width(), GetSize().height(), FS_LogPen( QColor(0x40, 0x40, 0x40), Qt::DashLine ) );
	}

	return 1;	
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::RecalculateSize_ReArrangeChildPos( SizeFde &sz )
{
	if( GetChildCount() )
	{
		// 
		long i;
		CNode *pNode;
		SetVerticalCenter( 0 );
		for( i = 0; i < GetChildCount(); i++ )
		{
			pNode = GetChild( i );
			if( pNode->GetVerticalCenter() > GetVerticalCenter() )
				SetVerticalCenter( pNode->GetVerticalCenter() );
		}
		SetVerticalCenter( GetVerticalCenter() + getTopIndent() );

		PointFde pos( getLeftIndent(), 0.0 );
		sz.rwidth() = getLeftIndent();
		sz.rheight() = getTopIndent();
		for( i = 0; i < GetChildCount(); i++ )
		{
			pNode = GetChild( i );
			pos.ry() = getTopIndent() + GetVerticalCenter() - pNode->GetVerticalCenter();

			RealFde prevPosX = pos.x();
			if( pNode->getAlignmentType() == FBtnChildPos::MSPACE_Exact_HeightDepth )
			{
				if( pNode->GetType() == NODE_PLANETEXT )
				{
					RealFde width = pNode->getMMLWidth();
					if( width < 0.0 ) pos.rx() += width;
					if( pos.x() < 0.0 ) pos.rx() = 0.0;
				}
				else if( pNode->GetType() == NODE_FORMULA )
				{
					RealFde newValues[ MML_MPADDED_UNITS_LEN ] = { 0.0, 0.0, 0.0, 0.0 };
					RealFde childValues[ MML_MPADDED_UNITS_LEN ] = { 0.0, 0.0, 0.0, 0.0 };
					int iRet[ MML_MPADDED_UNITS_LEN ] = { -1, -1, -1, -1 };
					((CFormulaNode*) pNode)->getChildDimensions_Vertical( childValues );
					((CFormulaNode*) pNode)->getChildDimensions_Horizontal( childValues );

					pNode->getMathMLAttr().parseMPaddedAttr( MML_MPADDED_UNITS_WIDTH, newValues, childValues, iRet, ::getCurrentFormulatorStyle().getUnit2PX() );
					if( iRet[ MML_MPADDED_UNITS_WIDTH ] == 0 )
					{
						RealFde width = newValues[ MML_MPADDED_UNITS_WIDTH ];
						RealFde delta = width - childValues[ MML_MPADDED_UNITS_WIDTH ];
						if( delta < 0.0 ) pos.rx() += delta;
						if( pos.x() < prevPosX ) pos.rx() = prevPosX;
					}
				}
			}

			pNode->SetPosition( pos );
			pos.rx() += pNode->GetSize().width();
			if( pos.x() > sz.width() ) sz.rwidth() = pos.x();
			sz.rheight() = std::max( sz.height(), pos.y() + pNode->GetSize().height() );
		}
		sz.rwidth() += getRightIndent(); 
		sz.rheight() += getBottomIndent();

		// 
		max_cy_notstretchy = 0.0;
		for( i = 0; i < GetChildCount(); i++ )
		{
			pNode = GetChild( i );
			if( pNode->GetType() != NODE_PLANETEXT || !pNode->isStretchy() )
			{
				RealFde a = pNode->GetVerticalCenter();
				RealFde b = pNode->GetSize().height() - a;
				if( a < b ) a = b;
				a *= 2.0;
				if( max_cy_notstretchy < a )
					max_cy_notstretchy = a;
			}
		}	
	}
	else
	{
		sz = ::getCurrentDefaultSize( GetLevel() );
		sz.rwidth() += getLeftIndent() + getRightIndent();
		sz.rheight() += getTopIndent() + getBottomIndent();
		SetVerticalCenter( sz.height() / 2.0 );
	}
}

void CLineNode::RecalculateSize( int bRecalc )
{
	SizeFde sz;
	CNode *pNode/*, *pRetNode = NULL*/;
	QVector<CNode*> todraw_2nd_time;

	RecalculateSize_ReArrangeChildPos( sz );

	long i;
	for( i = 0; i < GetChildCount(); i++ )
	{
		pNode = GetChild( i );
		if( pNode->GetType() == NODE_PLANETEXT && pNode->isStretchy() )
			todraw_2nd_time.push_back( pNode );
	}	

//	if( GetSize() != sz || todraw_2nd_time.size() )
	{
		SetSize( sz );

		if( todraw_2nd_time.size() )
		{
			long i, count = todraw_2nd_time.size();
			for( i = 0; i < count; i++ )
				todraw_2nd_time[ i ]->RecalculateSize( false );
			RecalculateSize_ReArrangeChildPos( sz );
			SetSize( sz );
		}

		if( bRecalc )
			GetParent()->RecalculateSize( bRecalc );
	}
}

///////////////////////////////////////////////////////////////////////////////

QString CLineNode::GetNodeText()
{
	QString retText("");
	for( long i = 0; i < GetChildCount(); i++ )
	{
		CNode *pNode = GetChild( i );
		if( pNode->GetType() == NODE_PLANETEXT )
			retText += pNode->GetNodeText();
	}
	return retText;
}

///////////////////////////////////////////////////////////////////////////////

long CLineNode::getPosFromPoint( const RectFde &rc, const PointFde &pt )
{
	if( GetChildCount() == 0 )
		return 0;

	PointFde tl = GetAbsolutePosition();
	CNode *pFirstNode, *pLastNode;

	long i = GetVisibleEdge( rc, tl, pFirstNode, pLastNode );
	if( i == -1 )
		return 0;

	long pos = 0;
	RealFde distance, value = FLT_MAX;
  	if( pFirstNode )
	{
		do
		{
			distance = fabs( tl.x() + GetChild( i )->GetPosition().x() - pt.x());
			if( distance < value )
			{
				value = distance;
				pos = i;
			}

			if( pFirstNode == pLastNode )
			{
				distance = fabs( tl.x() + GetChild( i )->GetPosition().x() + GetChild( i )->GetSize().width() - pt.x() );
				if( distance < value )
				{
					value = distance;
					pos = i + 1;
				}
				break;
			}
			pFirstNode = pFirstNode->GetNext();
			i++;
		}
		while ( pFirstNode );
	}
	return pos;
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::PumpLine2Line( CLineNode*& source )
{
	if( source )
	{
		QVector<CNode*> children;
		source->RemoveOrCopyChildren( children, 0, -1, -1 );
		InsertChildren( children, -1 );
	}
}

void CLineNode::PumpLine2LineAtHead( CLineNode*& source )
{
	if( source )
	{
		QVector<CNode*> children;
		source->RemoveOrCopyChildren( children, 0, -1, -1 );
		InsertChildren( children, 0 );
	}
}

///////////////////////////////////////////////////////////////////////////////

void CLineNode::setCMIgnore4Children( void )
{
	CNode *pCurNode = GetFirstChild();
	while( pCurNode )
	{
		pCurNode->setIgnoreContentConversion();
		pCurNode = pCurNode->GetNext();
	}
}

///////////////////////////////////////////////////////////////////////////////

long CLineNode::GetVisibleEdge( const RectFde &rc, const PointFde &tl, CNode *&pFirst, CNode *&pLast )
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

		x = tl.x() + pVisible->GetPosition().x();
		y = x + pVisible->GetSize().width();
		l = rc.left();
		r = rc.right();

		if( y < l )
			t = i;
		else if( x > r )
			b = i;
		else
			break;

		i = (b + t) >> 1;
	}
	while ( i != b && i != t );
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

		y = tl.x() + pNode->GetPosition().x() + pNode->GetSize().width();
		l = rc.left();
	}
	while ( y > l );

	pNode = pVisible;
	do
	{
		pLast = pNode;
		pNode = pNode->GetNext();
		if( pNode == NULL )
			break;

		x = tl.x() + pNode->GetPosition().x();
		r = rc.right();
	}
	while ( x < r );
	return i;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::getContentFormulaChild( long idx )
{
	CNode *node = GetChild( idx );
	if( !node || node->GetType() != NODE_FORMULA || (!node->to_mathml_data.is_content && !node->isIgnoreContentConversion()) ) return 0;
	return (CFormulaNode*)node;
}

CFormulaNode* CLineNode::getLastContentFormulaChild()
{
	CNode *node = GetLastChild();
	if( !node || node->GetType() != NODE_FORMULA || (!node->to_mathml_data.is_content && !node->isIgnoreContentConversion()) ) return 0;
	return (CFormulaNode*)node;
}

CFormulaNode* CLineNode::getFormulaChild( long idx )
{
	CNode *node = GetChild( idx );
	if( !node || node->GetType() != NODE_FORMULA ) return 0;
	return (CFormulaNode*)node;
}

CFormulaNode* CLineNode::getFirstFormulaChild()
{
	CNode *node = GetFirstChild();
	if( !node || node->GetType() != NODE_FORMULA ) return 0;
	return (CFormulaNode*)node;
}

CFormulaNode* CLineNode::getLastFormulaChild()
{
	CNode *node = GetLastChild();
	if( !node || node->GetType() != NODE_FORMULA ) return 0;
	return (CFormulaNode*)node;
}

void CLineNode::cleanSpareAutodetect()
{
	CNode *pNode;
	for( long no = 0; no < GetChildCount(); no++ )
	{
		pNode = GetChild( no );
		if( pNode->GetType() == NODE_FORMULA )
		{
			if( ((CFormulaNode*) pNode)->isAutodetectContent() )
			{
				CFormulaNode *singleChild = (CFormulaNode*) ((CFormulaNode*) pNode)->getSingleFormulaChild();
				if( singleChild && singleChild->isAutodetectContent() )
				{
					CNode *deepChild = singleChild->GetParent()->RemoveChild( 0 );
					Q_ASSERT( deepChild );
					ExchangeChild( pNode, deepChild );
					delete pNode;
					RecalculateSize( 0 );
					--no;
					continue;
				}
			}
			((CFormulaNode*) pNode)->cleanSpareAutodetect();
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::getContentTokenParent_Transparent()
{
	CNode *_n = 0;
	if( GetChildCount() == 1 && GetParent() && (_n = GetParent()->GetParent()) != 0 && 
		_n->GetType() == NODE_FORMULA && !_n->isAutodetectContent() && 
		::mml_is_cm_token( _n->to_mathml_data.name ) )
	{
		return (CFormulaNode*)_n;
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

QString CLineNode::getMathMLNodeDescription()
{
	QString ret("line - "), frmStr;
	CFormulaNode *frmParent = getParentFormulaNode( 0 );
	if( frmParent == 0 )
	{
		ret += getMathMLMarkup2Text();
		frmStr = "\"math\" element";
	}
	else
	{
		ret += frmParent->getMathMLMarkup2Text();
		frmStr = frmParent->getMathMLNodeDescription();
	}
	if( frmStr.length() )
		frmStr.insert(0, "; parent node - ");
	ret += frmStr;
	return ret;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CLineNode::convertDivSup2Nop( CFormulaNode *&opFormula, enum TAG2MML op, int isReverse )
{
	if( !(opFormula && opFormula->GetParent() == this && 
		  opFormula->to_mathml_data.is_content && ::mml_is_cm_apply( opFormula->to_mathml_data.name )) )
		return NULL;

	QString op1, op2 = FBL_TAG_nop;
	switch( op )
	{
	case TAG2MML_divide:
		op1 = FBL_TAG_divide;
		break;
	case TAG2MML_power:
		op1 = FBL_TAG_power;
		break;
	default:
		return NULL;
	}

	long n;
	TXML_Storage* bms = ::CompileMathML( _T(""), opFormula->to_mathml( 2, n ) );
	AST_TXML_Tag* applyXml = bms ? bms->getFirstTag( FBL_TAG_apply ) : NULL;
	AST_TXML_Tag* opXml = applyXml ? applyXml->getTagChild( 0 ) : NULL;
	if( !opXml || opXml->getTagName() != (isReverse ? op2 : op1) )
	{
		if( bms ) delete bms;
		return NULL;
	}
	opXml->setTagName( isReverse ? op1 : op2 );

	CLineNode *newLine = new CLineNode();
	newLine->read_mathml( applyXml );
	delete bms;
	bms = NULL;
	CFormulaNode *newOpFormula = newLine->getContentFormulaChild( 0 );
	if( newLine->GetChildCount() != 1 || !newOpFormula )
	{
		delete newLine;
		return NULL;
	}
	newLine->RemoveChild( 0 );
	delete newLine;
	newLine = NULL;

	ExchangeChild( opFormula, newOpFormula );
	delete opFormula;
	opFormula = NULL;
	return newOpFormula;
}

///////////////////////////////////////////////////////////////////////////////
