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

#include "baseratio.h"
#include "../nodes/dump/dumptags.h"

/////////////////////////////////////////////////////////////////////////////
// constructor / destructor
/////////////////////////////////////////////////////////////////////////////

CGraphBSplineEx::CGraphBSplineEx() 
	: CGraphBSpline()
	, m_offset_direction_x( 0.0 )
	, m_offset_direction_y( 0.0 )
{
}

CGraphBSplineEx::CGraphBSplineEx( const CGraphBSplineEx& a ) 
	: CGraphBSpline( a )
	, m_offset_direction_x( a.m_offset_direction_x )
	, m_offset_direction_y( a.m_offset_direction_y )
{
}

CGraphBSplineEx::~CGraphBSplineEx()
{
}

int CGraphBSplineEx::Create( const CGraphPrimFormula& templatePrim  )
{
	if ( !CGraphBSpline::Create( templatePrim ) )
		return 0;

	m_offset_direction_x = templatePrim.m_offset_direction_x;
	m_offset_direction_y = templatePrim.m_offset_direction_y;

	return 1;
}

/////////////////////////////////////////////////////////////////////////////

int CGraphBSplineEx::GetPoints( const RectFde &rc, QVector<PointFde> &xy, QVector<qint8> &tp )
{
	CGraphBSpline::GetPoints( rc, xy, tp );

	// b-spline-ex part
	long i, count = xy.size();
	if( count )
	{
		PointFde cp;
		qint8 vl = spln_point_type_lineto;

		tp[ count - 1 ] &= ~spln_point_type_clsfig;
		for( i = count; i > 0; i-- )
		{
			cp = xy[ i - 1 ];
			cp.rx() += m_offset_direction_x;
			cp.ry() += m_offset_direction_y;
			xy.push_back( cp );
			tp.push_back( vl );
			vl = tp[ i - 1 ];
		}
		tp[ xy.size() - 1 ] |= spln_point_type_clsfig;
	}

	return 0;
}

QDataStream& operator << ( QDataStream& ar, const CGraphBSplineEx& lp )
{
	return ar << (const CGraphPrim&) lp << lp.m_offset_direction_x << lp.m_offset_direction_y;
}

QDataStream& operator >> ( QDataStream& ar, CGraphBSplineEx& lp )
{
	return ar >> (CGraphPrim&) lp >> lp.m_offset_direction_x >> lp.m_offset_direction_y;
}

//////////////////////////////////////////////////////////////////////
