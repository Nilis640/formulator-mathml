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
#include "../dictionary/entity_map.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNode::CNode( long nNodeType )
	: CNodeInfo()
	, m_nNodeType( nNodeType )
{
	m_nLevel = 0;
	m_strName = "";
	m_vcenter = 0.0;
	m_left = 0.0;
	m_top = 0.0;
	m_right = 0.0;
	m_bottom = 0.0;
	m_pNext = NULL;
	m_pPrev = NULL;
	m_pParent = NULL;
	m_aPosition[ 0 ] = m_aPosition[ 1 ] = m_aPosition[ 2 ] = m_aPosition[ 3 ] = 0.0;
}

CNode::CNode( CNode* pNode ) 
	: CNodeInfo( pNode )
	, m_nNodeType( pNode->GetType() )
{
	m_nLevel = pNode->GetLevel();
	m_strName = pNode->GetName();
	m_vcenter = pNode->GetVerticalCenter();
	m_left = pNode->getLeftIndent();
	m_top = pNode->getTopIndent();
	m_right = pNode->getRightIndent();
	m_bottom = pNode->getBottomIndent();
	m_pNext = NULL;
	m_pPrev = NULL;
	m_pParent = NULL;

	m_aPosition[ 0 ] = pNode->getPosition(0);
	m_aPosition[ 1 ] = pNode->getPosition(1);
	m_aPosition[ 2 ] = pNode->getPosition(2);
	m_aPosition[ 3 ] = pNode->getPosition(3);
}

CNode::~CNode()
{
}

//////////////////////////////////////////////////////////////////////

void CNode::iterateNodeTree( NODE_TREE_ITERATOR call_back, void *arg )
{
	call_back( this, arg );
}

void CNode::iterateNodeTree_ToWidth( NODE_TREE_ITERATOR call_back, void *arg )
{
	call_back( this, arg );
}

//////////////////////////////////////////////////////////////////////

void CNode::Store( QDataStream& ar, SelectNode& /*sn*/, const std::vector<SelectNode>& /*bracketSn*/ ) const
{
	ar << (const CNodeInfo&)(*this);
	ar << m_strName;
	ar << m_aPosition[ 0 ] << m_aPosition[ 1 ] << m_aPosition[ 2 ] << m_aPosition[ 3 ];
	ar << m_nLevel;
	ar << m_vcenter;
	ar << m_left << m_right << m_top << m_bottom;
}

void CNode::Load( QDataStream& ar, SelectNode& /*sn*/, std::vector<SelectNode>& /*bracketSn*/, int /*isLeftBrLoad*/ )
{
	ar >> (CNodeInfo&)(*this);
	ar >> m_strName;
	ar >> m_aPosition[ 0 ] >> m_aPosition[ 1 ] >> m_aPosition[ 2 ] >> m_aPosition[ 3 ];
	ar >> m_nLevel;
	ar >> m_vcenter;
	ar >> m_left >> m_right >> m_top >> m_bottom;
}

void CNode::StoreBrackets( QDataStream& ar, const std::vector<SelectNode>& bracketSn ) const
{
	qint32 nLong = 0;
	// brackets positon
	long idx;
	std::vector<long> snIdx;
	for( idx = 0; idx < (long) bracketSn.size(); idx++ )
	{
		if( !bracketSn[ idx ].IsEmpty() && bracketSn[ idx ].GetNode() == this )
			snIdx.push_back( idx );
	}
	nLong = snIdx.size();
	ar << nLong;
	for( idx = 0; idx < (long) snIdx.size(); idx++ )
	{
		nLong = snIdx[ idx ];
		ar << nLong;
		nLong = bracketSn[ snIdx[ idx ] ].GetPosition();
		ar << nLong;
	}
}

void CNode::LoadBrackets( QDataStream& ar, std::vector<SelectNode>& bracketSn, int isLeftBrLoad )
{
	qint32 nLong = 0;
	// brackets positon
	long nBracketsCount, maxIdx = -1;
	ar >> nLong;
	nBracketsCount = nLong;
	if( nBracketsCount > 0 )
	{
		static const SelectNode emptySn( NULL, 0 );
		long idx = 0;
		std::vector<long> positionVec, idxVec;
		for( idx = 0; idx < nBracketsCount; idx++ )
		{
			ar >> nLong;
			idxVec.push_back( nLong );
			if( nLong > maxIdx )
				maxIdx = nLong;
			ar >> nLong;
			positionVec.push_back( nLong );
		}
		if( maxIdx >= 0 && maxIdx >= (long) bracketSn.size() )
			bracketSn.resize( maxIdx + 1, emptySn );
		for( idx = 0; idx < nBracketsCount; idx++ )
		{
			SelectNode bsn( this, positionVec[ idx ] );
			bracketSn[ idxVec[ idx ] ] = bsn;
			notifyNodeOnCreateBracket( positionVec[ idx ], isLeftBrLoad );
		}
	}
}

int CNode::CopyNodeInfo( CNode* pNode )
{
	setNodeInfo( *pNode );
	return 1;
}

CNode* CNode::GetNodeFromType( long nNodeType, long *pnPos ) const
{
	Q_ASSERT( nNodeType == NODE_LINE || nNodeType == NODE_FORMULA || nNodeType == NODE_FRAME );
	CNode *pChld = NULL;
	CNode *pNode = (CNode*) this;
	do
	{
		if( pNode->GetType() == nNodeType )
		{
			if( pnPos )
				*pnPos = (pChld ? ((CParentNode*) pNode)->GetChildIndex( pChld ) : -1);
			return pNode;
		}
		pChld = pNode;
	}
	while ( (pNode = pNode->GetParent()) != NULL );
	return pNode;
}

CParentNode* CNode::GetRootParent() const
{
	CParentNode *currentParent = 0;
	if( (currentParent = GetParent()) == 0 ) return 0;
	CParentNode *retParent = 0;
	do
	{
		retParent = currentParent;
	}
	while( (currentParent = currentParent->GetParent()) != 0 );

	return retParent;
}

long CNode::GetParentFrameCount() const
{
	long nCount = 0;
	CNode *pNode = GetParent();
	while( pNode )
	{
		if( pNode->GetType() == NODE_FRAME )
			nCount++;
		pNode = pNode->GetParent();
	}
	return nCount;
}

RealFde CNode::LengthPttoNode( PointFde& pt, RealFde kX )
{
	RectFde rc = RectFde( GetAbsolutePosition(), GetSize() );
	RealFde width = (pt.x() >= rc.left() && pt.x() <= rc.right()) ? 0.0 : std::min( fabs( pt.x() - rc.left() ), fabs( pt.x() - rc.right() ) );
	RealFde height = (pt.y() >= rc.top() && pt.y() <= rc.bottom()) ? 0.0 : std::min( fabs( pt.y() - rc.top() ), fabs( pt.y() - rc.bottom() ) );
	return sqrt( (kX * width * width + height * height) );
}

PointFde CNode::GetAbsolutePosition( void )
{
	PointFde retPoint = GetPosition();
	CNode *lpNode = this;
	while( lpNode->GetParent() != NULL )
	{
		lpNode = lpNode->GetParent();
		retPoint += lpNode->GetPosition();
	}
	return retPoint;
}

void CNode::SetAlignmentPosition( const RectFde &rect, unsigned int type, RealFde /*maxVC*/ )
{
	switch( type & HORISONTAL_MASK )
	{
	case TO_LEFT:
		m_aPosition[ SVG_NodeTemplate_Parameter_X ] = rect.left();
		break;
	case TO_RIGHT:
		m_aPosition[ SVG_NodeTemplate_Parameter_X ] = rect.right() - m_aPosition[ SVG_NodeTemplate_Parameter_CX ];
		break;
	case HORISONTAL_CENTER:
		m_aPosition[ SVG_NodeTemplate_Parameter_X ] = (rect.left() + rect.right() - m_aPosition[ SVG_NodeTemplate_Parameter_CX ]) / 2.0;
		break;
	default:
		Q_ASSERT( 0 );
	}

	switch ( type & VERTICAL_MASK )
	{
	case TO_TOP:
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] = rect.top();
		break;
	case TO_BOTTOM:
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] = rect.bottom() - m_aPosition[ SVG_NodeTemplate_Parameter_CY ];
		break;
	case TO_BASELINE:
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] = rect.top();
		break;
	case VERTICAL_CENTER:
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] = (rect.top() + rect.bottom() - m_aPosition[ SVG_NodeTemplate_Parameter_CY ]) / 2.0;
		break;
	default:
		Q_ASSERT( 0 );
	}

 	if( m_aPosition[ SVG_NodeTemplate_Parameter_X ] < 0.0 ) 
		m_aPosition[ SVG_NodeTemplate_Parameter_X ] = 0.0;
	if( m_aPosition[ SVG_NodeTemplate_Parameter_Y ] < 0.0 ) 
		m_aPosition[ SVG_NodeTemplate_Parameter_Y ] = 0.0;
}

/////////////////////////////////////////////////////////////////////////////
// NODE TREE ITERATORS
/////////////////////////////////////////////////////////////////////////////

void write2log_MissButton( CNode* node, void* arg )
{
	if( arg && node && node->isMissButton() )
	{
		HKMap<QString,int>& collection = *(HKMap<QString,int>*) arg;
		if( node->GetType() == NODE_FORMULA )
			collection[ ((CFormulaNode*) node)->getCalcButtonID() ] = 1;
		else if( node->GetType() == NODE_PLANETEXT )
			collection[ ((CPlaneText*) node)->getCalcButtonID() ] = 1;
	}
}

void search4ContentChildren( CNode* node, void* /*arg*/ )
{
	if( !node ) return;

	if( node->to_mathml_data.is_content || node->isIgnoreContentConversion() || node->isAutodetectContent() )
		throw 1;
}

void accountLinebreaks( CNode* node, void* /*arg*/ )
{
	if( node && node->GetType() == NODE_PLANETEXT && 
		node->to_mathml_data.mathml_template_string.length() > 0 &&
		node->getAlignmentType() == FBtnChildPos::MSPACE_Exact_HeightDepth )
	{
		QString value = _T("");
		int ret = node->getMathMLAttr().getFilteredAttr( QString(FBL_ATTR_linebreak), value );
		if( ret == -1 || value.length() == 0 ) return;

		if( value == FBL_VAL_newline || value == FBL_VAL_indentingnewline || value == FBL_VAL_goodbreak )
		{
			if( node->GetParent() && node->GetParent()->GetType() == NODE_LINE )
			{
				CLineNode *srcLine = (CLineNode*)(node->GetParent());
				long nFrom = srcLine->GetChildIndex( node ) + 1;
				if( nFrom == -1 || nFrom >= srcLine->GetChildCount() ) /* we have nothing to break */
					return;
				if( srcLine->GetParent() == 0 ) return;

				CLineNode *newLine = new CLineNode();
				QVector<CNode*> children;
				srcLine->RemoveOrCopyChildren( children, nFrom, -1, -1 );
				newLine->InsertChildren( children, -1 );
				newLine->SetLevel( srcLine->GetLevel() );
				srcLine->GetParent()->AddChildAfter( newLine, srcLine );
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////

QString CNode::getMathMLMarkup2Text()
{
	QString ret = isIgnoreContentConversion() || isAutodetectContent() || to_mathml_data.is_content ?
		"content MathML" : "presentation MathML";
	return ret;
}

QString CNode::getMathMLNodeDescription()
{
	return _T("");
}

/////////////////////////////////////////////////////////////////////////////
//

CLineNode* CNode::getParentLineNode( int useReadonly, long *childIndexOfRequest )
{
	for( CNode *curNode = GetParent(), *oldNode = this;
			curNode != 0 && oldNode != 0;
			oldNode = curNode, curNode = curNode->GetParent() )
	{
		if( curNode->GetType() == NODE_LINE && 
			(useReadonly || (!curNode->isCannotBeEdited() && !curNode->isCannotBeMoved())) )
		{
			if( childIndexOfRequest != 0 )
				*childIndexOfRequest = ((CLineNode*) curNode)->GetChildIndex( oldNode );
			return (CLineNode*) curNode;
		}
	}
	return 0;
}

CFormulaNode* CNode::getParentFormulaNode( int useIgnoreNode, long *childIndexOfRequest )
{
	for( CNode *curNode = this, *oldNode = this;
			curNode != 0 && oldNode != 0;
			oldNode = curNode, curNode = curNode->GetParent() )
	{
		if( curNode->GetType() == NODE_FORMULA && 
			(useIgnoreNode || !curNode->isIgnoreContentConversion()) )
		{
			if( childIndexOfRequest != 0 )
				*childIndexOfRequest = ((CFormulaNode*) curNode)->GetChildIndex( oldNode );
			return (CFormulaNode*) curNode;
		}
	}
	return 0;
}

CFormulaNode* CNode::getParentContentFormulaNode( int useIgnoreNode, long *childIndexOfRequest )
{
	for( CNode *curNode = this, *oldNode = this;
			curNode != 0 && oldNode != 0;
			oldNode = curNode, curNode = curNode->GetParent() )
	{
		if( (curNode->GetType() == NODE_FORMULA) && 
			(useIgnoreNode || !curNode->isIgnoreContentConversion()) &&
			(curNode->isAutodetectContent() || curNode->to_mathml_data.is_content != 0) )
		{
			if( childIndexOfRequest != 0 )
				*childIndexOfRequest = ((CFormulaNode*) curNode)->GetChildIndex( oldNode );
			return (CFormulaNode*) curNode;
		}
	}
	return 0;
}

void CNode::cleanSpareAutodetect()
{
}

RealFde CNode::getAscent()
{
	return ::calculateCurrentTextAscent( GetLevel() );
}

RealFde CNode::getDescent()
{
	return ::calculateCurrentTextDescent( GetLevel() );
}

/////////////////////////////////////////////////////////////////////

RealFde calculateCurrentHeight( CNode *pNode, int ignoreIndent, long level, RealFde ascent )
{
	RealFde height = 0.0;
	if( pNode != NULL )
	{
		height = pNode->GetSize().height();
		if( ignoreIndent )
			height -= pNode->getTopIndent() + pNode->getBottomIndent();
	}
	return height - ::calculateCurrentDepth( level, ascent );
}

RealFde calculateCurrentDepth( long level, RealFde ascent )
{
	return ::getCurrentDefaultSize( level ).height() - ascent;
}

RealFde calculateCurrentTextAscent( long level )
{
	CPlaneText textNode;
	textNode.SetLevel( level );
	return textNode.getAscent();
}

RealFde calculateCurrentTextDescent( long level )
{
	CPlaneText textNode;
	textNode.SetLevel( level );
	return textNode.getDescent();
}

//////////////////////////////////////////////////////////////////////////////

void mmlTreeIterator__mapUnicodeValue2EntityName( AST_TXML_Node *node, void * data )
{
	if( !node ) return;

	if( node->getNodeType() == ASTT_TXML_Value )
	{
		QString dest_str;
		QString src_str = ((AST_TXML_Value*)node)->getText();
		::mapUnicodeValue2EntityName( dest_str, src_str );
		((AST_TXML_Value*)node)->getText() = dest_str;
	}
	else if( data && *(int*)data && node->getNodeType() == ASTT_TXML_Tag )
	{
		AST_TXML_Compound *_attrlist = ((AST_TXML_Tag*)node)->getAttributes();
		if( !_attrlist || _attrlist->getChildNumber() == 0 ) return;
		AST_TXML_Node *attrnode = _attrlist->getChild( 0 );
		if( attrnode->getNodeType() != ASTT_TXML_Attribute || 
			((AST_TXML_Attribute*)attrnode)->getAttributeName() != "__flag" ) return;

		QString str = ((AST_TXML_Attribute*)attrnode)->getStringValue();
		_attrlist->Erase( 0 );

		bool ok;
		long value = str.toLong(&ok, 16);
		if( !ok ) return;
		((AST_TXML_Tag*)node)->getFlags().setFlag( value );
	}
}

/////////////////////////////////////////////////////////////////////

int CNode::DrawBrackets( CFmlDrawEngine& fde, const PointFde& /*tl*/, const RectFde& /*rc*/ )
{
	int isLeft = 0;
	CNode *pNode = 0;

	long position = 0;
	unsigned char value = 0;
	
	std::vector<long> key2erase;
	HKMap<long, unsigned char>::iterator it, end = m_brackets.end();
	for( it = m_brackets.begin(); it != end; it++ )
	{
		position = it->first;
		value = it->second;

		isLeft = value & 0x80;
		SizeFde sz;
		PointFde pt;
		switch( GetType() )
		{
		case NODE_PLANETEXT:
			if( position > 0 && position < ((CPlaneText*) this)->GetStringLength() )
			{
				sz = ((CPlaneText*) this)->getDrawSize( position ).size();
				pt = GetAbsolutePosition();
				pt.ry() = GetParent() ? GetParent()->GetAbsolutePosition().y() : 0;
				pt.rx() += sz.width();
				sz.rwidth() = 1.0;
				sz.rheight() = GetParent() ? GetParent()->GetSize().height() : 0;
			}
			else
			{
				key2erase.push_back( position );
				value = 0;
			}
			break;
		case NODE_LINE:
			sz = GetSize();
			sz.rwidth() = 1.0;
			pt = GetAbsolutePosition();
			if( position > 0 )
			{
				pNode = ((CLineNode*) this)->GetChild( position - 1 );
				if( pNode )
				{
					pt.rx() += pNode->GetPosition().x();
					pt.rx() += pNode->GetSize().width();
				}
				else
				{
					key2erase.push_back( position );
					value = 0;
				}
			}
			break;
		default:
			break;
		}
		{
			static qint8 _tps[] =
			{ 
				spln_point_type_moveto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 

				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 

				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto, 
				spln_point_type_splnto | spln_point_type_clsfig
			};
			static PointFde _pts_l[] = 
			{
				PointFde(1.074074074,	0.005050505 ), 
				PointFde(0.777777778,	0.075757576 ), 
				PointFde(0.37037037,	0.171717172 ), 
				PointFde(0.22222222,	0.297979798 ), 
				PointFde(0.0,			0.439393939 ), 
						
				PointFde(0.074074074,	0.616161616 ), 
				PointFde(0.259259259,	0.702020202 ), 
				PointFde(0.407407407,	0.813131313 ), 
				PointFde(0.777777778,	0.929292929 ), 
				PointFde(1.074074074,	0.994949495 ), 
				PointFde(1.111111111,	1.005050505 ), 
				PointFde(1.222222222,	1.0 ), 
				PointFde(1.185185185,	0.984848485 ), 
				PointFde(0.888888889,	0.904040404 ), 
						
				PointFde(0.666666667,	0.797979798 ), 
				PointFde(0.555555556,	0.702020202 ), 
				PointFde(0.407407407,	0.535353535 ), 
				PointFde(0.407407407,	0.449494949 ), 
				PointFde(0.518518519,	0.303030303 ), 
				PointFde(0.62962963,	0.207070707 ), 
				PointFde(0.888888889,	0.085858586 ), 
				PointFde(1.148148148,	0.015151515 ), 
				PointFde(1.222222222,	0.0 ), 
				PointFde(1.148148148,	-0.005050505 ), 
				PointFde(1.074074074,	0.005050505 )


			};
			static PointFde _pts_r[] = 
			{
				PointFde(-0.074074074,		0.005050505 ), 
				PointFde(0.222222222,		0.075757576 ), 
				PointFde(0.62962962962963,	0.171717172 ), 
				PointFde(0.777777777777778,	0.297979798 ), 
				PointFde(1.0,				0.439393939 ), 
						
				PointFde(0.925925925925926,	0.616161616 ), 
				PointFde(0.740740740740741,	0.702020202 ), 
				PointFde(0.592592592592593,	0.813131313 ), 
				PointFde(0.222222222222222,	0.929292929 ), 
				PointFde(-0.074074074,		0.994949495 ), 
				PointFde(-0.111111111,		1.005050505 ), 
				PointFde(-0.222222222,		1.0 ), 
				PointFde(-0.185185185,		0.984848485 ), 
				PointFde(0.111111111111111,	0.904040404 ), 
						
				PointFde(0.333333333333333,	0.797979798 ), 
				PointFde(0.444444444444445,	0.702020202 ), 
				PointFde(0.592592592592593,	0.535353535 ), 
				PointFde(0.592592592592593,	0.449494949 ), 
				PointFde(0.481481481481482,	0.303030303 ), 
				PointFde(0.37037037037037,	0.207070707 ), 
				PointFde(0.111111111111111,	0.085858586 ), 
				PointFde(-0.148148148148148,0.015151515 ), 
				PointFde(-0.222222222,		0.0 ), 
				PointFde(-0.148148148,		-0.005050505 ), 
				PointFde(-0.074074074,		0.005050505 )
			};

			sz.rwidth() = 3;
			if( !isLeft ) pt.rx() -= 3;

			QVector<qint8> tps( sizeof( _tps ) );
			QVector<PointFde> pts( tps.size() );
			for( long i = 0; i < tps.size(); i++ )
			{
				tps[ i ] = _tps[ i ];
				pts[ i ] = (isLeft ? _pts_l[ i ] : _pts_r[ i ]);
				pts[ i ].rx() *= sz.width();
				pts[ i ].ry() *= sz.height();
				pts[ i ].rx() += pt.x();
				pts[ i ].ry() += pt.y();
			}

			QColor grayColor(0x3F, 0x3F, 0x3F), grayTransparentColor(0x7F, 0x7F, 0x7F, 0x5F);
			fde.DrawFillPath( pts, tps, FS_LogPen(grayTransparentColor), FS_LogBrush(grayTransparentColor, Qt::SolidPattern) );
			//fde.FillPath( pts, tps, FS_LogBrush(QColor(0, 0, 0, 0xFF), Qt::SolidPattern) );
		}
	}

	for( long i = 0; i < (long) key2erase.size(); i++ )
	{
		m_brackets.erase( key2erase[ i ] );
	}

	return 1;
}

int CNode::hasBracket( long bracketPosition, int isLeft )
{
	unsigned char *brInfo = m_brackets.is_find( bracketPosition );
	return (brInfo && isLeft == ((*brInfo >> 7) & 0x01)) ? (*brInfo & 0x7F) : 0;
}

void CNode::notifyNodeOnRemoveBracket( long bracketPosition )
{
	unsigned char *brInfo = m_brackets.is_find( bracketPosition );
	if( brInfo )
	{
		unsigned char cb = *brInfo & 0x7F;
		if( cb <= 1 )
			m_brackets.erase( bracketPosition );
		else
		{
			cb--;
			m_brackets[ bracketPosition ] = (*brInfo & 0x80) | cb;
		}
	}
}

void CNode::notifyNodeOnCreateBracket( long bracketPosition, int isLeft )
{
	unsigned char *brInfo = m_brackets.is_find( bracketPosition );
	if( brInfo )
	{
		unsigned char cb = *brInfo & 0x7F;
		if( cb < 0x7F ) cb++;
		if( isLeft == ((*brInfo >> 7) & 0x01) )
			m_brackets[ bracketPosition ] = (*brInfo & 0x80) | cb;
	}
	else
	{
		if( isLeft == 1 )
			m_brackets[ bracketPosition ] = 0x81;
		else if( isLeft == 0 )
			m_brackets[ bracketPosition ] = 0x01;
	}
}

/////////////////////////////////////////////////////////////////////
