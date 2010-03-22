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
#include "../style/style.h"
#include "../settings/options.h"
#include "../nodes/dump/dumptags.h"

/////////////////////////////////////////////////////////////////////////////

CGraphBSpline::CGraphBSpline() 
	: CGraphPrim()
	, m_point_type()
	, m_point_xy()
{
}

CGraphBSpline::CGraphBSpline( const CGraphBSpline& a ) 
	: CGraphPrim( a )
	, m_point_type( a.m_point_type )
	, m_point_xy( a.m_point_xy )
{
}

CGraphBSpline::~CGraphBSpline()
{
}

/////////////////////////////////////////////////////////////////////////////

int CGraphBSpline::Create( const CGraphPrimFormula& pf  )
{
	if( !CGraphPrim::Create( pf ) )
		return 0;

	m_point_type = pf.m_point_type;
	m_point_xy = pf.m_point_xy;

	return 1;
}

/////////////////////////////////////////////////////////////////////////////

int CGraphBSpline::Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode* )
{
	RectFde rc = GetPositionRect();
	rc.translate( tl.x(), tl.y() );

	QVector<qint8> tp;
	QVector<PointFde> xy;
	GetPoints( rc, xy, tp );

	QColor color = (GetColor() == DEFAULT_GRAPH_COLOR ? ::getCurrentFormulatorStyle().getLogPen().m_color : GetColor());

	fde.DrawFillPath( xy, tp, FS_LogPen( color, (Qt::PenStyle) GetStyle(), GetWidth() ), FS_LogBrush(GetColor(), Qt::SolidPattern) );

	return 1;
}

/////////////////////////////////////////////////////////////////////////////

int CGraphBSpline::GetPoints( const RectFde &rc, QVector<PointFde>& xy, QVector<qint8>& tp )
{
	// scale factor
	RealFde w = rc.width();
	RealFde h = rc.height();

	PointFde point;
	long i, count = m_point_xy.size();
	for( i = 0; i < count; i++ )
	{
		point.rx() = rc.left() + m_point_xy[ i ].x() * w / DRAWENGINE_BSPLINE_SCALE_X;
		point.ry() = rc.top()  + m_point_xy[ i ].y() * h / DRAWENGINE_BSPLINE_SCALE_Y;
		xy.push_back( point );
	}

	tp = m_point_type;
	return 0;
}

//////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const CGraphBSpline& lp )
{
	return ar << (const CGraphPrim&) lp << lp.m_point_type << lp.m_point_xy;
}

QDataStream& operator >> ( QDataStream& ar, CGraphBSpline& lp )
{
	return ar >> (CGraphPrim&) lp >> lp.m_point_type >> lp.m_point_xy;
}

//////////////////////////////////////////////////////////////////////
