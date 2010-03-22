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

#include "HUtils/iustring.h"
#include "../dictionary/op_iddefs.h"

#include "node.h"
#include "dump/dumptags.h"

///////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
///////////////////////////////////////////////////////////////////////////////

CFormulaNode::CFormulaNode()
	: CParentNode( NODE_FORMULA )
	, CNodeExInfo()
	, CFormulaExCalc()
{
}

CFormulaNode::CFormulaNode( CFormulaNode* pFormulaNode )
	: CParentNode( pFormulaNode )
	, CNodeExInfo( pFormulaNode )
	, CFormulaExCalc( pFormulaNode )
	, m_frame_corrector( pFormulaNode->m_frame_corrector )
{
}

CFormulaNode::~CFormulaNode()
{
}

///////////////////////////////////////////////////////////////////////////////

void CFormulaNode::Store( QDataStream& ar, SelectNode& sn, const std::vector<SelectNode>& bracketSn ) const
{
	CParentNode::Store( ar, sn, bracketSn );
	ar << (const CNodeExInfo&)(*this);
	CFormulaExCalc::Store( ar );

	ar << m_frame_corrector;
}

void CFormulaNode::Load( QDataStream& ar, SelectNode& sn, std::vector<SelectNode>& bracketSn, int isLeftBrLoad )
{
	CParentNode::Load( ar, sn, bracketSn, isLeftBrLoad );
	ar >> (CNodeExInfo&)(*this);
	CFormulaExCalc::Load( ar );

	ar >> m_frame_corrector;
}

///////////////////////////////////////////////////////////////////////////////

int CFormulaNode::CopyNodeInfo( CNode* pNode )
{
	CParentNode::CopyNodeInfo( pNode );
	if( pNode->GetType() == NODE_FORMULA )
	{
		CFormulaNode *pFormulaNode = (CFormulaNode*) pNode;
		setNodeExInfo( *pFormulaNode );
		m_frame_corrector = pFormulaNode->m_frame_corrector;
	}
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

int CFormulaNode::Draw( CFmlDrawEngine& fde, const PointFde& tl, const RectFde& rc )
{
	QString _mathbackground = getMathMLAttr().getFilteredAttr( FBL_ATTR_mathbackground );
	if( isHidden() || to_mathml_data.name == FBL_TAG_mstyle )
	{
		if( _mathbackground.length() )
		{
			QColor dest_bkcolor;
			QString backcolor_str = _mathbackground;
			int rr = ::mml_mathcolor2color( backcolor_str, dest_bkcolor, QColor(0, 0, 0) );
			if( rr != -1 )
			{
				// Paint the background
				fde.FillRectangle( tl.x(), tl.y(), GetSize().width(), GetSize().height(), FS_LogBrush(dest_bkcolor, Qt::SolidPattern) ); 
			}
		}
		if( isHidden() )
			return 0;
	}

	if( !isFrameHidden() )
	{
		// draw child items
		RectFde rcItem;
		CNode *pNode = NULL;
		for( long i = 0; i < GetChildCount(); i++ )
		{
			pNode = GetChild( i );
			PointFde res = tl + pNode->GetPosition();
			rcItem.setLeft( res.x() );
			rcItem.setRight( rcItem.left() + pNode->GetSize().width() );
			rcItem.setTop( res.y() );
			rcItem.setBottom( rcItem.top() + pNode->GetSize().height() );

			if( rcItem.left() > rc.right() || rcItem.top() > rc.bottom() || rcItem.right() < rc.left() || rcItem.bottom() < rc.top() )
				continue;

			pNode->Draw( fde, PointFde( rcItem.left(), rcItem.top() ), rc );
		}
	}

	// draw graph primitives
	CNodeExInfo_GraphPrimitive::Draw( fde, tl, this );

	return 1;
}

///////////////////////////////////////////////////////////////////////////////

int CFormulaNode::CreateGraphPrimIndirect( int nCount, CGraphPrimFormula *grpr )
{
	CNodeExInfo_GraphPrimitive::CreateGraphPrimIndirect( grpr, nCount );
	m_frame_corrector.clear();
	m_frame_corrector.fill( -1, GetChildCount() );
	CNodeExInfo_GraphPrimitive::CreateFrameCorrector( m_frame_corrector );
	return 1;
}

int CFormulaNode::CreateGraphPrimIndirect( const QVector<CGraphPrimFormula>& srcGraphics )
{
	CNodeExInfo_GraphPrimitive::CreateGraphPrimIndirect( srcGraphics );
	m_frame_corrector.clear();
	m_frame_corrector.fill( -1, GetChildCount() );
	CNodeExInfo_GraphPrimitive::CreateFrameCorrector( m_frame_corrector );
	return 1;
}

///////////////////////////////////////////////////////////////////////////////

void CFormulaNode::calculatePadded_Vertical( RealFde *destDim )
{
	RealFde newValues[ MML_MPADDED_UNITS_LEN ] = { 0.0, 0.0, 0.0, 0.0 };
	RealFde childValues[ MML_MPADDED_UNITS_LEN ] = { 0.0, 0.0, 0.0, 0.0 };
	getChildDimensions_Horizontal( childValues );
	getChildDimensions_Vertical( childValues );

	int iRet[ 4 ] = { -1, -1, -1, -1 };
	getMathMLAttr().parseMPaddedAttr( MML_MPADDED_UNITS_HEIGHT, newValues, childValues, iRet, ::getCurrentFormulatorStyle().getUnit2PX() );
	getMathMLAttr().parseMPaddedAttr( MML_MPADDED_UNITS_DEPTH, newValues, childValues, iRet, ::getCurrentFormulatorStyle().getUnit2PX() );

	destDim[ MML_MPADDED_UNITS_HEIGHT ] = (iRet[ MML_MPADDED_UNITS_HEIGHT ] == 0) ? 
		newValues[ MML_MPADDED_UNITS_HEIGHT ] : childValues[ MML_MPADDED_UNITS_HEIGHT ];
	destDim[ MML_MPADDED_UNITS_DEPTH ] = (iRet[ MML_MPADDED_UNITS_DEPTH ] == 0) ? 
		newValues[ MML_MPADDED_UNITS_DEPTH ] : childValues[ MML_MPADDED_UNITS_DEPTH ];
}

void CFormulaNode::getChildDimensions_Vertical( RealFde *destDim )
{
	CNode *padded = getSingleChild();
	if( padded && padded->getAlignmentType() == FBtnChildPos::MSPACE_Exact_HeightDepth )
	{
		if( padded->GetType() == NODE_FORMULA )
		{
			((CFormulaNode*)padded)->calculatePadded_Vertical( destDim );
			return;
		}
		else if( padded->GetType() == NODE_PLANETEXT )
		{
			destDim[ MML_MPADDED_UNITS_HEIGHT ] = padded->getMMLHeight();
			destDim[ MML_MPADDED_UNITS_DEPTH ]  = padded->getMMLDepth();
			return;
		}
	}

	RealFde ascent = ::calculateCurrentTextAscent( GetLevel() );
	destDim[ MML_MPADDED_UNITS_DEPTH ] = ::calculateCurrentDepth( GetLevel(), ascent );
	destDim[ MML_MPADDED_UNITS_HEIGHT ] = ::calculateCurrentHeight( GetChild( 0 ), 1, GetLevel(), ascent );
}

void CFormulaNode::getChildDimensions_Horizontal( RealFde *destDim )
{
	CNode *n1st = GetChild( 0 );
	CNode *n1stChild = getFormulaDeepChild( 0, 0 );

	RealFde width = n1st ? (n1st->GetSize().width() - n1st->getLeftIndent() - n1st->getRightIndent()) : 0.0;
	destDim[ MML_MPADDED_UNITS_WIDTH ] = (n1st ? width : 0.0);
	destDim[ MML_MPADDED_UNITS_LSPACE ] = (n1stChild ? n1stChild->getLSpace() : 0.0);
}

///////////////////////////////////////////////////////////////////////////////
//

RealFde CFormulaNode::getAscent( const SizeFde &sz )
{
	RealFde vc = sz.height() / 2.0;

	switch( getAlignmentType() )
	{
	case FBtnChildPos::TableCenter2Baseline:
		if( (getAlignmentValue() - 1) >= 0 && (getAlignmentValue() - 1) < GetChildCount() )
		{
			CNode *pNode = GetChild( getAlignmentValue() - 1 );
			if( pNode != NULL )
				vc = pNode->GetPosition().y() + pNode->GetSize().height() / 2.0;
		}
		break;

	case FBtnChildPos::TableTop2Baseline:
		if( (getAlignmentValue() - 1) >= 0 && (getAlignmentValue() - 1) < GetChildCount() )
		{
			CNode *pNode = GetChild( getAlignmentValue() - 1 );
			if( pNode != NULL )
				vc = pNode->GetPosition().y();
			else
				vc = 0.0;
		}
		else
			vc = 0.0;
		break;

	case FBtnChildPos::TableBottom2Baseline:
		if( (getAlignmentValue() - 1) >= 0 && (getAlignmentValue() - 1) < GetChildCount() )
		{
			CNode *pNode = GetChild( getAlignmentValue() - 1 );
			if( pNode != NULL )
				vc = pNode->GetPosition().y() + pNode->GetSize().height();
			else
				vc = sz.height();
		}
		else
			vc = sz.height();
		break;
	default:
		vc = sz.height() - ::calculateCurrentTextDescent( GetLevel() );
		break;
	}

	return vc;
}

///////////////////////////////////////////////////////////////////////////////
//

void CFormulaNode::Recalculate_VerticalCenter( const SizeFde &sz )
{
	RealFde vc = sz.height() / 2.0;

	switch( getAlignmentType() )
	{
	case FBtnChildPos::Child:
		if( getAlignmentValue() >= 1 && (getAlignmentValue() - 1) < getGraphPrimNumber() )
		{
			const RectFde& _t = getGraphPrimitivePositionRect( getAlignmentValue() - 1 );
			vc = (_t.top() + _t.bottom()) / 2.0;
		}
		break;
	case FBtnChildPos::Child_Average:
		if( getAlignmentValue() >= 1 && (getAlignmentValue() - 1) < getGraphPrimNumber() )
		{
			vc = 0.0;
			for( long i = 0; i <= getAlignmentValue() - 1; i++ )
			{
				const RectFde& _t = getGraphPrimitivePositionRect( i );
				vc += _t.top() + _t.bottom();
			}
			vc = vc / (getAlignmentValue() * 2.0);
		}
		break;
	case FBtnChildPos::MSPACE_Exact_HeightDepth:
	case FBtnChildPos::Frame:
		if( (getAlignmentValue() - 1) >= 0 && (getAlignmentValue() - 1) < GetChildCount() )
		{
			CNode *pNode = GetChild( getAlignmentValue() - 1 );
			if( pNode )
				vc = pNode->GetPosition().y() + pNode->GetVerticalCenter();
		}
		break;
	case FBtnChildPos::Frame_Blank_Average:
		{
			CNode *pNodeFirst = GetFirstChild();
			CNode *pNodeLast  = GetLastChild();
			if( pNodeFirst != NULL && pNodeLast != NULL )
			{
				vc = (pNodeFirst->GetPosition().y() + pNodeFirst->GetSize().height() + pNodeLast->GetPosition().y()) / 2.0;
			}
		}
		break;
	case FBtnChildPos::Half:
		break;

	case FBtnChildPos::TableAxis:
		if( (getAlignmentValue() - 1) >= 0 && (getAlignmentValue() - 1) < GetChildCount() )
		{
			CNode *pNode = GetChild( getAlignmentValue() - 1 );
			if( pNode )
				vc = pNode->GetPosition().y() + pNode->GetSize().height() / 2.0;
		}
		break;

	case FBtnChildPos::TableCenter2Baseline:
	case FBtnChildPos::TableTop2Baseline:
	case FBtnChildPos::TableBottom2Baseline:
		{
			RealFde asc = ::getCurrentDefaultAscent( GetLevel() );
			RealFde h = ::getCurrentDefaultSize( GetLevel() ).height();
			vc = getAscent( sz ) - asc + h - asc / 2.0;
		}
		break;

	default:
		break;
	}

	SetVerticalCenter( vc );
}

///////////////////////////////////////////////////////////////////////////////
//

void CFormulaNode::RecalculateSize( int bRecalc )
{
	SizeFde sz( 0, 0 );
	int i = 0;

	sz = CFormulaExCalc::RecalculateSize( this );
	if( (i = RecalculateGrph( this, regetActiveCalc() )) != 0 )
	{
		sz = CFormulaExCalc::RecalculateSize( this );
		i = RecalculateGrph( this, regetActiveCalc() );
	}
	if( isHiddenNDisabled() )
	{
		sz.rwidth() = 0;
		if( GetParent() && GetParent()->GetSize().height() > 0 )
			sz.rheight() = GetParent()->GetSize().height();
		else
			sz.rheight() = GetDefaultSize().height();
	}
	Recalculate_VerticalCenter( sz );

	//=== 'mpadded' MathML element
	if( !isHiddenNDisabled() &&
		getAlignmentType() == FBtnChildPos::MSPACE_Exact_HeightDepth )
	{
		RealFde newValues[ MML_MPADDED_UNITS_LEN ] = { 0.0, 0.0, 0.0, 0.0 };
		RealFde childValues[ MML_MPADDED_UNITS_LEN ] = { 0.0, 0.0, 0.0, 0.0 };
		int iRet[ MML_MPADDED_UNITS_LEN ] = { -1, -1, -1, -1 };
		getChildDimensions_Vertical( childValues );
		getChildDimensions_Horizontal( childValues );

		getMathMLAttr().parseMPaddedAttr( newValues, childValues, iRet, ::getCurrentFormulatorStyle().getUnit2PX() );

		RealFde paddedLSpace = 0.0, paddedRSpace = 0.0;
		if( iRet[ MML_MPADDED_UNITS_LSPACE ] == 0 )
		{
			paddedLSpace = newValues[ MML_MPADDED_UNITS_LSPACE ];
		}
		if( iRet[ MML_MPADDED_UNITS_WIDTH ] == 0 )
		{
			RealFde width = newValues[ MML_MPADDED_UNITS_WIDTH ];
			RealFde delta = width - (paddedLSpace + childValues[ MML_MPADDED_UNITS_WIDTH ]);
			if( delta >= 0.0 )
				paddedRSpace = delta;
			else
			{
				paddedLSpace = width - childValues[ MML_MPADDED_UNITS_WIDTH ];
				if( paddedLSpace < 0.0 )
				{
					sz.rwidth() = width;
					paddedLSpace = 0.0;
				}
			}
		}

		RealFde paddedTSpace = 0.0, paddedBSpace = 0.0;
		if( iRet[ MML_MPADDED_UNITS_HEIGHT ] == 0 )
		{
			RealFde height = newValues[ MML_MPADDED_UNITS_HEIGHT ];
			RealFde delta = height - childValues[ MML_MPADDED_UNITS_HEIGHT ];
			if( delta >= 0 )
				paddedTSpace = delta;
			else
			{
				// not feasible 
			}
		}
		if( iRet[ MML_MPADDED_UNITS_DEPTH ] == 0 )
		{
			RealFde delta = newValues[ MML_MPADDED_UNITS_DEPTH ] - childValues[ MML_MPADDED_UNITS_DEPTH ];
			if( delta >= 0 )
				paddedBSpace = delta;
			else
			{
				// not feasible 
			}
		}

		if( paddedLSpace || paddedRSpace || paddedTSpace || paddedBSpace )
		{
			CNode *pNode = GetFirstChild();
			if( pNode && pNode->GetType() == NODE_FRAME )
			{
				CFrameNode *n1st = (CFrameNode*)pNode;
				n1st->setLeftIndent( paddedLSpace );
				n1st->setRightIndent( paddedRSpace );
				n1st->setTopIndent( paddedTSpace );
				n1st->setBottomIndent( paddedBSpace );
			}
		}
	}

	if( sz != GetSize() || i )
		SetSize( sz );

	if( bRecalc )
		GetParent()->RecalculateSize( bRecalc );
}

///////////////////////////////////////////////////////////////////////////////

void CFormulaNode::SetLevel( long nLevel )
{
	CNode::SetLevel( nLevel );

	long i, count = GetSlotCount();
	for( i = 0; i < count; i++ )
	{
		if( GetChild( i ) == NULL ) continue;
		GetChild( i )->SetLevel( nLevel + GetSlot( i ).GetLevel() );
	}

	CNodeExInfo_GraphPrimitive::SetGraphPrimitiveLevel( nLevel );
}

///////////////////////////////////////////////////////////////////////////////

int CFormulaNode::RunCalc()
{
	if( !getCalc() )
	{
		if( getCalcButtonID().length() )
			setMissButton();
		return 0;
	}
	if( !getCalc()->isPositionRunExist() ) return 0;

	QVector<RealFde> frames_ltrb;
	frames_ltrb.fill( 0.0, 4 * GetChildCount() );
	QVector<RealFde> frames_margin_ltrb;
	frames_margin_ltrb.fill( 0.0, 4 * GetChildCount() );
	QVector<RealFde> primitive_ltrb;
	primitive_ltrb.fill( 0.0, 4 * GetChildCount() );

	long i;
	CNode *pCurNode = GetFirstChild();
	if( pCurNode->GetType() != NODE_FRAME ) return 0;
	for( i = 0; i < GetChildCount() && pCurNode; pCurNode = pCurNode->GetNext(), i++ )
	{
		frames_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Left ] = pCurNode->GetPosition().x();
		frames_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Top ] = pCurNode->GetPosition().y();
		frames_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Right ] = pCurNode->GetPosition().x() + pCurNode->GetSize().width();
		frames_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Bottom ] = pCurNode->GetPosition().y() + pCurNode->GetSize().height();

		frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Left ] = ((CFrameNode*) pCurNode)->getLeftIndent();
		frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Top ] = ((CFrameNode*) pCurNode)->getTopIndent();
		frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Right ] = ((CFrameNode*) pCurNode)->getRightIndent();
		frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Bottom ] = ((CFrameNode*) pCurNode)->getBottomIndent();
	}

	for( i = 0; i < getGraphPrimNumber(); i++ )
	{
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 0 ) );
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 1 ) );
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 2 ) );
		primitive_ltrb.push_back( getGraphPrimitivePositionCoord( i, 3 ) );
	}

	UseBtnCalc_InitByThisSize( *getCalc() );
	int ret = getCalc()->PositionRun( frames_ltrb, frames_margin_ltrb, primitive_ltrb );

	// reset parent margins
	UseBtnCalc_ChangeThisMargin( *getCalc() );

	for( i = 0; i < getGraphPrimNumber(); i++ )
	{
		if( getCalc()->isFormulaTypeGraphics( i ) )
		{
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Left,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Left ] );
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Top,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Top ] );
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Right,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Right ] );
			setGraphPrimitivePositionCoord( 
				i, SVG_NodeTemplate_Parameter_Bottom,
				primitive_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Bottom ] );
		}
	}

	if( ret )
	{
		pCurNode = GetFirstChild();
		i = 0;
		while( pCurNode )
		{
			((CFrameNode*) pCurNode)->setLeftIndent( frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Left ] );
			((CFrameNode*) pCurNode)->setTopIndent( frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Top ] );
			((CFrameNode*) pCurNode)->setRightIndent( frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Right ] );
			((CFrameNode*) pCurNode)->setBottomIndent( frames_margin_ltrb[ i * 4 + SVG_NodeTemplate_Parameter_Bottom ] );
			pCurNode = pCurNode->GetNext();
			i++;
		}
		RecalculateSizeChild();
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////

QString CFormulaNode::getMathMLNodeDescription_IterateParent()
{
	QString res = _T("");
	CFormulaNode *frmParent = this;
	CNode *pParentNode = 0;
	while( (pParentNode = frmParent->GetParent()) != 0 )
	{
		frmParent = pParentNode->getParentContentFormulaNode( 0 );
		if( !frmParent ) break;
		res = frmParent->getMathMLNodeDescription();
		if( res.length() ) return res;
	}
	return res;
}

QString CFormulaNode::getMathMLNodeDescription()
{
	QString res = _T("");
	if( isAutodetectContent() )
	{
		QString res = getMathMLNodeDescription_IterateParent();
		if( res.length() ) return res;
		res = _T("autodetected by content");
	}
	else if( !isRWMMLSupport() )
		res = _T("unsupported in MathML");
	else if( isIgnoreContentConversion() )
		res = _T("auxiliary");
	else if( to_mathml_data.type.length() && 
			 !xml_strcmp(to_mathml_data.type, "table") &&
			 to_mathml_data.name.length() )
		res = _T("<mtable>");
	else if( to_mathml_data.type.length() && !xml_strcmp(to_mathml_data.type, "bracket") )
		res = _T("<mfence>");
	else if( to_mathml_data.name.length() )
	{
		QString op = _T("");
		if( to_mathml_data.name == FBL_TAG_apply )
			op = getApplyOp();
		res = QString( _T("\"") ) + to_mathml_data.name + QString( _T("\"") );
		if( op.length() )
			res += QString( _T(" (") ) + op + QString( _T(")") );
	}
	else if( to_mathml_data.value.length() && to_mathml_data.type.length() )
		res = QString( _T("\"") ) + to_mathml_data.type + QString( _T("\"") );
	else if( to_mathml_data.mathml_template_string.length() && !isReferenced() )
	{
		res = _T("");
		for( long i = 0; i < to_mathml_data.mathml_template_string.length() && 
								  to_mathml_data.mathml_template_string.at(i) != QChar('\n'); i++ )
		{
			res.append( to_mathml_data.mathml_template_string.at(i) );
		}
	}

	if( res.length() == 0 )
		return getMathMLNodeDescription_IterateParent();

	res += _T(" element");
	return res;
}


///////////////////////////////////////////////////////////////////////////////

void CFormulaNode::setContentTokenParent_Transparent()
{
	if( GetParent() && GetParent()->GetType() == NODE_LINE )
	{
		CFormulaNode *frmNode = ((CLineNode*)GetParent())->getContentTokenParent_Transparent();
		if( frmNode ) frmNode->setAutodetectContent();
	}
}

CFormulaNode* CFormulaNode::getSemanticsOrCTokenNodeWrapped()
{
	for( CNode *curNode = this; curNode != 0; curNode = curNode->GetParent() )
	{
		if( curNode->GetType() == NODE_FORMULA && curNode->to_mathml_data.is_content != 0 )
		{
			if( !xml_strcmp( curNode->to_mathml_data.name, FBL_TAG_semantics ) || 
				!xml_strcmp( curNode->to_mathml_data.name, FBL_TAG_annotation ) || 
				::mml_is_cm_token( curNode->to_mathml_data.name ) )
				return (CFormulaNode*)curNode;
			else if( curNode->to_mathml_data.name == FBL_TAG_annotation_xml )
			{
				QString attrValue = _T("");
				struct HMathMLAttr mml_attr_by_this_node( getMathMLAttr() );
				if( mml_attr_by_this_node.getFilteredAttr( FBL_ATTR_encoding, attrValue ) == 0 &&
					attrValue == FBL_VAL_MathML_Content )
				{
					continue;
				}
				return (CFormulaNode*)curNode;
			}
		}
	}
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

CNode* CFormulaNode::getFormulaDeepChild( long frame_idx, long child_idx )
{
	CNode* pFrame = GetChild( frame_idx );

	if( !pFrame || pFrame->GetType() != NODE_FRAME )
		return NULL;

	if( !((CFrameNode*) pFrame)->GetFirstChild() ||
		((CFrameNode*) pFrame)->GetFirstChild()->GetType() != NODE_LINE )
		return NULL;

	CLineNode* pLine = (CLineNode*) ((CFrameNode*) pFrame)->GetFirstChild();

	if( child_idx == -1 )
		return pLine->GetLastChild();
	else if( child_idx == 0 )
		return pLine->GetFirstChild();
	return pLine->GetChild( child_idx );
}

CFormulaNode* CFormulaNode::getFormulaDeepFormulaChild( long frame_idx, long child_idx )
{
	CNode *ret = getFormulaDeepChild( frame_idx, child_idx );
	if( !ret || ret->GetType() != NODE_FORMULA )
		return NULL;
	return (CFormulaNode*) ret;
}

CLineNode* CFormulaNode::getFormulaLineChild( long frame_idx, long child_idx )
{
	CNode* pFrame = GetChild( frame_idx );

	if( !pFrame || pFrame->GetType() != NODE_FRAME )
		return NULL;

	CNode* fch = ((CFrameNode*) pFrame)->GetChild( child_idx );
	if( !fch || fch->GetType() != NODE_LINE)
		return NULL;

	return (CLineNode*) fch;
}

CLineNode* CFormulaNode::getContentFormulaSingleLineChild()
{
	if( (to_mathml_data.is_content == 0 && !isAutodetectContent()) || 
		GetChildCount() != 1 || GetFirstChild() == 0 )
		return NULL;

	Q_ASSERT( GetFirstChild()->GetType() == NODE_FRAME );
	CFrameNode *frame = (CFrameNode*) GetFirstChild();
	if( frame->GetChildCount() != 1 || frame->GetFirstChild() == 0 )
		return NULL;	// don't know how to wrap into brackets a multiline frame

	Q_ASSERT( frame->GetFirstChild()->GetType() == NODE_LINE );
	return (CLineNode*) frame->GetFirstChild();
}

CNode* CFormulaNode::getSingleChild()
{
	CNode *node = 0;

	if( GetChildCount() != 1 ) return 0;
	node = GetFirstChild();
	if( !node || node->GetType() != NODE_FRAME ) return 0;
	CFrameNode *frame = (CFrameNode*)node;

	if( frame->GetChildCount() != 1 ) return 0;
	node = frame->GetFirstChild();
	if( !node || node->GetType() != NODE_LINE ) return 0;
	CLineNode *line = (CLineNode*)node;

	if( line->GetChildCount() != 1 ) return 0;
	node = line->GetFirstChild();
	if( !node ) return 0;

	return node;
}

CNode* CFormulaNode::getSingleChild( long frame_idx )
{
	CNode *node = GetChild( frame_idx );
	if( !node || node->GetType() != NODE_FRAME ) return 0;
	CFrameNode *frame = (CFrameNode*)node;

	if( frame->GetChildCount() != 1 ) return 0;
	node = frame->GetFirstChild();
	if( !node || node->GetType() != NODE_LINE ) return 0;
	CLineNode *line = (CLineNode*)node;

	if( line->GetChildCount() != 1 ) return 0;
	node = line->GetFirstChild();
	if( !node ) return 0;

	return node;
}

CNode* CFormulaNode::getSingleFormulaChild()
{
	CNode *node = getSingleChild();
	if( node && node->GetType() == NODE_FORMULA ) return node;
	return 0;
}

CPlaneText* CFormulaNode::getSingleTextChild()
{
	CNode *node = getSingleChild();
	if( node && node->GetType() == NODE_PLANETEXT ) return (CPlaneText*) node;
	return 0;
}

CNode* CFormulaNode::getSingleFormulaChild( long frame_idx )
{
	CNode *node = getSingleChild( frame_idx );
	if( node && node->GetType() == NODE_FORMULA ) return node;
	return 0;
}

CFormulaNode* CFormulaNode::getSingleFormulaChild_ContentEmptyFrame( long frame_idx )
{
	CNode *node = getSingleChild( frame_idx );
	if( !node || node->GetType() != NODE_FORMULA ) return 0;
	if( (node->to_mathml_data.is_content || node->isAutodetectContent()) &&
		((CFormulaNode*) node)->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
	{
		return (CFormulaNode*) node;
	}
	return 0;
}

CPlaneText* CFormulaNode::getSingleFormulaChild_ContentText( long frame_idx )
{
	CNode *node = getSingleChild( frame_idx );
	if( !node || node->GetType() != NODE_FORMULA ||
		((CFormulaNode*) node)->getCalcButtonID() != HF_BTNID_EMPTY_FRAME )
		return 0;
	if( node->to_mathml_data.is_content || node->isAutodetectContent() )
	{
		return ((CFormulaNode*) node)->getSingleTextChild();
	}
	return 0;
}

CNode* CFormulaNode::getSingleContentFormulaChild()
{
	CNode *node = getSingleFormulaChild();
	if( node && node->to_mathml_data.is_content ) /*node->isIgnoreContentConversion()*/
		return node;
	return 0;
}

long CFormulaNode::getUpperFormulaChildrenCount4Content()
{
	long lRet = 0;
	CLineNode *_line = getContentFormulaSingleLineChild();
	if( _line )
	{
		CNode *pNode;
		for( long i = 0; i < _line->GetChildCount(); i++ )
		{
			pNode = _line->GetChild( i );
			if( pNode->GetType() == NODE_FORMULA )
				lRet++;
		}
	}
	return lRet;
}

///////////////////////////////////////////////////////////////////////////////

CFormulaNode* CFormulaNode::getRecursiveApplyOperand( bool lastChild, QString exceptId )
{
	CFormulaNode *tryFormulaNode = this;
	for(;;)
	{
		CNode *contentNode = 0;
		if( tryFormulaNode->isAutodetectContent() && 
			tryFormulaNode->getCalcButtonID() == HF_BTNID_EMPTY_FRAME )
		{
			contentNode = tryFormulaNode->getSingleChild( 0 );
		}
		else if( tryFormulaNode->to_mathml_data.is_content &&
				::mml_is_cm_apply( tryFormulaNode->to_mathml_data.name ) && 
				tryFormulaNode->getCalcButtonID() != exceptId )
		{
			contentNode = lastChild ? tryFormulaNode->getFormulaDeepChild( 0, -1 ) : tryFormulaNode->getFormulaDeepChild( 0, 0 );
		}
		if( contentNode && contentNode->GetType() == NODE_FORMULA && (contentNode->to_mathml_data.is_content || contentNode->isAutodetectContent()) )
			tryFormulaNode = (CFormulaNode*) contentNode;
		else
			break;
	}
	return tryFormulaNode;
}

CFormulaNode* CFormulaNode::passAutodetectParent( CFormulaNode *upperLimit )
{
	CFormulaNode *pslot = this;
	Q_ASSERT( pslot->GetParent() );

	for( CFormulaNode *frmNode = pslot->GetParent()->getParentFormulaNode( 0 );
			frmNode && frmNode->GetParent() && frmNode != upperLimit;
			frmNode = pslot->GetParent()->getParentFormulaNode( 0 ) )
	{
		if( ::mml_is_cm_token( frmNode->to_mathml_data.name ) )
		{
			frmNode->setAutodetectContent();
			pslot = frmNode;
		}
		else if( frmNode->isAutodetectContent() )
			pslot = frmNode;
		else
			break;
	}

	return pslot;
}

CFormulaNode* CFormulaNode::passAutodetectParent_SingleFormulaChild( CFormulaNode *upperLimit )
{
	CFormulaNode *pslot = this;
	Q_ASSERT( pslot->GetParent() );

	for( CFormulaNode *frmNode = pslot->GetParent()->getParentFormulaNode( 0 );
			frmNode && frmNode->GetParent() && frmNode != upperLimit && ((CFormulaNode*) frmNode->getSingleFormulaChild() == pslot);
			frmNode = pslot->GetParent()->getParentFormulaNode( 0 ) )
	{
		if( ::mml_is_cm_token( frmNode->to_mathml_data.name ) )
		{
			frmNode->setAutodetectContent();
			pslot = frmNode;
		}
		else if( frmNode->isAutodetectContent() )
			pslot = frmNode;
		else
			break;
	}

	return pslot;
}

CFormulaNode* CFormulaNode::passAutodetectSingleFormulaChild()
{
	CFormulaNode *singleChild = 0;
	if( isAutodetectContent() && (singleChild = (CFormulaNode*) getSingleFormulaChild()) != NULL )
		return singleChild;
	return this;
}

CFormulaNode* CFormulaNode::passAutodetectSingleFormulaChildren()
{
	CFormulaNode *singleChild = 0;
	if( isAutodetectContent() && (singleChild = (CFormulaNode*) getSingleFormulaChild()) != NULL )
		return singleChild->passAutodetectSingleFormulaChildren();
	return this;
}

CFormulaNode* CFormulaNode::passParentApplyBrackets( CFormulaNode *upperLimit )
{
	CFormulaNode *pslot = this;

	if( !pslot->GetParent() ) return pslot;
	pslot = this->passAutodetectParent_SingleFormulaChild( upperLimit );
	if( pslot->to_mathml_data.is_content )
	{
		CFormulaNode *frmNode = 0;
		if( pslot->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR )
		{
			frmNode = pslot->GetParent()->getParentFormulaNode( 0 );
			if( frmNode && frmNode->GetParent() && frmNode != upperLimit && 
				frmNode->to_mathml_data.is_content && ::mml_is_cm_apply( frmNode->to_mathml_data.name ) &&
				(CFormulaNode*) frmNode->getSingleFormulaChild() == pslot )
			{
				return frmNode->passAutodetectParent_SingleFormulaChild( upperLimit );
			}
		}
		else if( ::mml_is_cm_apply( pslot->to_mathml_data.name ) )
		{
			frmNode = pslot->GetParent()->getParentFormulaNode( 0 );
			if( frmNode && frmNode->GetParent() && frmNode != upperLimit && 
				frmNode->to_mathml_data.is_content && frmNode->getCalcButtonID() == HF_BTNID_DEFAULT_BRACKET_LR &&
				(CFormulaNode*) frmNode->getSingleFormulaChild() == pslot )
			{
				return frmNode->passAutodetectParent_SingleFormulaChild( upperLimit );
			}
		}
	}
	return pslot;
}

//////////////////////////////////////////////////////////////////////

void CFormulaNode::adjustReadOnlyAttr( CLineNode *templateLine )
{
	if( !templateLine ) templateLine = (CLineNode*) GetParent();
	if( !templateLine ) return;

	if( templateLine->isCannotBeMoved() )
		setCannotBeMoved();
	else
		clearCannotBeMoved();

	if( templateLine->isCannotBeEdited() )
		setCannotBeEdited();
	else
		clearCannotBeEdited();
}

void CFormulaNode::adjustReadOnlyAttr4Parent()
{
	if( isCannotBeMoved() )
		GetParent()->setCannotBeMoved();
	else
		GetParent()->clearCannotBeMoved();

	if( isCannotBeEdited() )
		GetParent()->setCannotBeEdited();
	else
		GetParent()->clearCannotBeEdited();
}

//////////////////////////////////////////////////////////////////////

CFormulaNode* CFormulaNode::reorderFormulaBracketsTree( CFormulaNode *slot, CFormulaNode *pslot, CFormulaNode *sharedFormula, int shouldExchangeReadOnlyAttr )
{
	CFormulaNode *pDummyNode = new CFormulaNode();
	((CLineNode*) sharedFormula->GetParent())->ExchangeChild( sharedFormula, pDummyNode );
	((CLineNode*) slot->GetParent())->ExchangeChild( slot, sharedFormula );
	((CLineNode*) pslot->GetParent())->ExchangeChild( pslot, slot );
	((CLineNode*) pDummyNode->GetParent())->ExchangeChild( pDummyNode, pslot );
	delete pDummyNode;

	if( shouldExchangeReadOnlyAttr )
	{
		int pslot_isCannotBeMoved = pslot->isCannotBeMoved(), pslot_isCannotBeEdited = pslot->isCannotBeEdited();
		sharedFormula->isCannotBeMoved()  ? pslot->setCannotBeMoved() : pslot->clearCannotBeMoved();
		sharedFormula->isCannotBeEdited() ? pslot->setCannotBeEdited() : pslot->clearCannotBeEdited();
		pslot_isCannotBeMoved  ? sharedFormula->setCannotBeMoved() : sharedFormula->clearCannotBeMoved();
		pslot_isCannotBeEdited ? sharedFormula->setCannotBeEdited() : sharedFormula->clearCannotBeEdited();
	}

	return pslot;
}

//////////////////////////////////////////////////////////////////////
