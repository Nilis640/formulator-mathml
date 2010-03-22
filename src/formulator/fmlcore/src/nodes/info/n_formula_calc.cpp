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

#include <QtGlobal>

#include "../node.h"
#include "n_formula_calc.h"

//////////////////////////////////////////////////////////////////////

CFormulaExCalc::CFormulaExCalc()
	: m_slots()
	, m_defSize()
	, m_psx()
	, m_psy()
{
}

CFormulaExCalc::CFormulaExCalc( CFormulaExCalc* pFormulaEx )
	: m_slots( pFormulaEx->m_slots )
	, m_defSize( pFormulaEx->m_defSize )
	, m_psx( pFormulaEx->m_psx )
	, m_psy( pFormulaEx->m_psy )
{
}

CFormulaExCalc::~CFormulaExCalc()
{
}

//////////////////////////////////////////////////////////////////////

void CFormulaExCalc::Store( QDataStream& ar ) const
{
	ar << m_slots;
}

void CFormulaExCalc::Load( QDataStream& ar )
{
	ar >> m_slots;
}

//////////////////////////////////////////////////////////////////////

SizeFde CFormulaExCalc::RecalculateSize( CParentNode *pParentNode )
{
	CNode *pNode;
	RectFde position;
	SizeFde retSize, defSize;
	long i, j, k, idx, type;

	m_defSize = CalculateDefaultSize( pParentNode );
	retSize = CalculateFormulaSize( m_defSize, pParentNode );

	int isTable = ( pParentNode->GetChildCount() > 1 && 
					pParentNode->GetChildCount() == pParentNode->GetNumLine() * pParentNode->GetNumColumn() );

	int isRecalculate = 0;
	if( isTable )
	{
		RealFde maxVC, posY, correctY;
		CNode *pTblNode = 0, *pLineChild = 0;
		QVector<RealFde> nodeVC;
		for( i = 0; i < pParentNode->GetNumLine(); i++ )
		{
			maxVC = 0.0;
			nodeVC.clear();
			nodeVC.fill( RealFde(0.0), pParentNode->GetNumColumn() );
			for( j = 0; j < pParentNode->GetNumColumn(); j++ )
			{
				idx = i * pParentNode->GetNumColumn() + j;
				if( (pTblNode = pParentNode->GetChild( idx )) == 0 || 
					(pTblNode->GetType() != NODE_FRAME) ||
					(pLineChild = ((CFrameNode*) pTblNode)->GetLastChild()) == 0 )
				{
					continue;
				}
				nodeVC[ j ] = pLineChild->GetPosition().y() + pLineChild->GetVerticalCenter() - pTblNode->getTopIndent();
				Q_ASSERT( nodeVC[ j ] >= 0 );
				if( maxVC < nodeVC[ j ] ) 
					maxVC = nodeVC[ j ];
			}

			for( j = 0; j < pParentNode->GetNumColumn(); j++ )
			{
				idx = i * pParentNode->GetNumColumn() + j;
				if( (pTblNode = pParentNode->GetChild( idx )) == 0 ) continue;

				position = GetSlot( idx ).GetPecentPosition();
				position = GetRealPosition( position );

				type = GetSlot( idx ).GetAlignType();
				if( (type & VERTICAL_MASK) == TO_BASELINE )
				{
					posY = maxVC - nodeVC[ j ];
					Q_ASSERT( posY >= 0 );
					correctY = posY - pTblNode->getTopIndent();
					pTblNode->setTopIndent( posY );

					CFrameNode *frameNode = (CFrameNode*) pTblNode;
					for( k = 0; k < frameNode->GetChildCount(); k++ )
					{
						if( (pLineChild = frameNode->GetChild( k )) == 0 ) continue;
						PointFde pnt = pLineChild->GetPosition();
						pnt.ry() += correctY;
						Q_ASSERT( pnt.y() >= 0 );
						pLineChild->SetPosition( pnt );
					}

					SizeFde sz( pTblNode->GetSize() );
					sz.rheight() += correctY;
					if( sz.height() < 0 )
						sz.rheight() = 0;
					pTblNode->SetSize( sz );

					isRecalculate = 1;
				}
			}
		}
	}

	if( isRecalculate )
		retSize = CalculateFormulaSize( m_defSize, pParentNode );

	for( i = 0; i < pParentNode->GetChildCount(); i++ )
	{
		position = GetSlot( i ).GetPecentPosition();
		position = GetRealPosition( position );

		pNode = pParentNode->GetChild( i );
		pNode->SetAlignmentPosition( position, GetSlot( i ).GetAlignType(), pNode->GetVerticalCenter() );
	}

	return retSize;
}

//////////////////////////////////////////////////////////////////////
//

RectFde CFormulaExCalc::GetRealPosition( RectFde rc )
{
	if( rc.left() == rc.right() )
	{
		rc.setLeft( GetMapValue( rc.left(), m_psx ) );
		rc.setRight( rc.left() );
	}
	else
	{
		rc.setLeft( GetMapValue( rc.left(), m_psx ) );
		rc.setRight( GetMapValue( rc.right(), m_psx ) );
	}

	if( rc.top() == rc.bottom() )
	{
		rc.setTop( GetMapValue( rc.top(), m_psy ) );
		rc.setBottom( rc.top() );
	}
	else
	{
		rc.setTop( GetMapValue( rc.top(), m_psy ) );
		rc.setBottom( GetMapValue( rc.bottom(), m_psy ) );
	}
	return rc;
}

/////////////////////////////////////////////////////////////////////
//

SizeFde CFormulaExCalc::CalculateDefaultSize( CParentNode *pParentNode )
{
	QVector<RectFde> rects;
	QVector<SizeFde> sizes;

	long i, count = GetSlotCount()/*, level = 0*/;
	for( i = 0; i < count; i++ )
	{
		CNode* pNode = pParentNode->GetChild( i );
		if( pNode == NULL ) continue;

		RectFde rc = GetSlot( i ).GetPecentPosition();
		SizeFde sz = ::getCurrentDefaultSize( pNode->GetLevel() );

		rects.push_back( rc );
		sizes.push_back( sz );
	}

	return CalculateSizeByPercents( rects, sizes );
}

//////////////////////////////////////////////////////////////////////
//

SizeFde CFormulaExCalc::CalculateFormulaSize( const SizeFde& defSize, CParentNode *pParentNode )
{
	QVector<RectFde> rects;
	QVector<SizeFde> sizes;

	rects.push_back( RectFde( PointFde(0.0, 0.0), PointFde(HF_BINBUTTON_HORIZONTAL_SCALE, HF_BINBUTTON_VERTICAL_SCALE) ) );
	sizes.push_back( defSize );

	long i, count = GetSlotCount();
	for( i = 0; i < count; i++ )
	{
		CNode* pNode = pParentNode->GetChild( i );
		if( pNode == NULL ) continue;

		RectFde rc = GetSlot( i ).GetPecentPosition();
		SizeFde sz = pNode->GetSize();

		rects.push_back( rc );
		sizes.push_back( sz );
	}

	return CalculateSizeByPercents( rects, sizes );
}

//////////////////////////////////////////////////////////////////////
//

SizeFde CFormulaExCalc::CalculateSizeByPercents( QVector<RectFde>& rects, QVector<SizeFde>& sizes )
{
	Q_ASSERT( rects.size() == sizes.size() );
	if( rects.size() != sizes.size() )
	{
		qWarning("CFormulaExCalc::CalculateSizeByPercents assert");
		return SizeFde(0, 0);
	}

	m_psx.clear();
	m_psy.clear();

	RectFde rc;
	SizeFde sz;

	long i, count = rects.size();
	for( i = 0; i < count; i++ )
	{
		rc = rects[ i ];

		m_psx[ rc.left() ] = m_psx[ rc.right() ] = 0.0;
		m_psy[ rc.top() ] = m_psy[ rc.bottom() ] = 0.0;
	}

	CExCalcMap::iterator itl, itr, it;
	for( i = 0; i < count; i++ )
	{
		rc = rects[ i ];
		sz = sizes[ i ];

		sz.rwidth() /= rc.width();
		sz.rheight() /= rc.height();

#ifdef Q_WS_WIN
		// в_сь х
		itl = m_psx.find( rc.left() );
		itr = m_psx.find( rc.right() );
		while( itl != m_psx.end() && itl != itr )
		{
			itl++;
			if( itl == m_psx.end() )
				break;
			if( itl->second < sz.width() )
				itl->second = sz.width();
		}

		// в_сь у
		itl = m_psy.find( rc.top() );
		itr = m_psy.find( rc.bottom() );
		while( itl != m_psy.end() && itl != itr )
		{
			itl++;
			if( itl == m_psy.end() )
				break;
			if( itl->second < sz.height() )
				itl->second = sz.height();
		}
#else
		if(rc.left() < rc.right())
		{
			for(it = m_psx.begin(); it != m_psx.end(); it++)
			{
				if(it->first > rc.left() && it->first <= rc.right())
				{
					if( it->second < sz.width() )
						it->second = sz.width();
				}
			}
		}
		if(rc.top() < rc.bottom())
		{
			for(it = m_psy.begin(); it != m_psy.end(); it++)
			{
				if(it->first > rc.top() && it->first <= rc.bottom())
				{
					if( it->second < sz.height() )
						it->second = sz.height();
				}
			}
		}
#endif
	}

	sz.rwidth() = 0.0;
	sz.rheight() = 0.0;

	itr = m_psx.end();
	it = itl = m_psx.begin();
	if( itl != m_psx.end() )
		itl++;
	for( ; itl != m_psx.end() && it != m_psx.end() && itl != itr; it = itl, itl++ )
	{
		sz.rwidth() += (itl->first - it->first) * itl->second;
		itl->second = sz.width();
	}

	itr = m_psy.end();
	it = itl = m_psy.begin();
	if( itl != m_psy.end() )
		itl++;
	for( ; itl != m_psy.end() && it != m_psy.end() && itl != itr; it = itl, itl++ )
	{
		sz.rheight() += (itl->first - it->first) * itl->second;
		itl->second = sz.height();
	}

	return sz;
}

//////////////////////////////////////////////////////////////////////
//

RealFde CFormulaExCalc::GetMapValue( RealFde pos, CExCalcMap& ps )
{
        CExCalcMap::iterator it = ps.find( pos );
	if( it != ps.end() )
		return it->second;

        CExCalcMap::iterator itr, end = ps.end();
	for( itr = ps.begin(); itr != end; itr++ )
	{
		if( pos < itr->first )
			break;
		it = itr;
	}
	Q_ASSERT( it != end );
	Q_ASSERT( itr != end );
	if( it == end || itr == end )
	{
		qWarning("CFormulaExCalc::GetMapValue assert");
		return 0.0;
	}

	RealFde value = it->second + 
		(itr->second - it->second) * (pos - it->first) / (itr->first - it->first);
	ps[ pos ] = value;
	return value;
}

//////////////////////////////////////////////////////////////////////
