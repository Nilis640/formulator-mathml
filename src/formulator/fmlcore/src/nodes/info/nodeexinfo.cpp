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

//#include "../../button/btn_graphics.h"
#include "../node.h"
#include "../node.h"
#include "nodeexinfo.h"

///////////////////////////////////////////////////////////////////////////////

CNodeExInfo_GraphPrimitive::~CNodeExInfo_GraphPrimitive()
{
	for( long i = 0; i < m_GraphPrim.size(); i++ )
	{
		if( m_GraphPrim[ i ] )
		{
			delete m_GraphPrim[ i ];
			m_GraphPrim[ i ] = NULL;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////

QDataStream& operator << ( QDataStream& ar, const CNodeExInfo_GraphPrimitive& lp )
{
	qint32 count = lp.m_GraphPrim.size();
	ar << count;
	for( long i = 0; i < lp.m_GraphPrim.size(); i++ )
	{
		count = lp.m_GraphPrim[ i ]->GetType();
		ar << count;
		switch( count )
		{
		case FBtnGraphics::GRAPH_PRIM_LINE:
			ar << *((CGraphLine*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_RECT:
			ar << *((CGraphRect*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_TEXT:
			ar << *((CGraphText*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_SPLINE:
			ar << *((CGraphBSpline*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_SPLINE_EX:
			ar << *((CGraphBSplineEx*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_QUATER_ARC:
			ar << *((CGraphQuaterArc*) (lp.m_GraphPrim[ i ]));
			break;
		default:
			Q_ASSERT( 0 );
		}
	}
	return ar << lp.m_GraphPrimFormula << lp.m_altGroup << lp.m_ActiveAltGroup;
}

QDataStream& operator >> ( QDataStream& ar, CNodeExInfo_GraphPrimitive& lp )
{
	qint32 count;
	ar >> count;
	lp.m_GraphPrim.fill( NULL, count );
	for( long i = 0; i < lp.m_GraphPrim.size(); i++ )
	{
		ar >> count;
		switch( count )
		{
		case FBtnGraphics::GRAPH_PRIM_LINE:
			lp.m_GraphPrim[ i ] = new CGraphLine();
			ar >> *((CGraphLine*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_RECT:
			lp.m_GraphPrim[ i ] = new CGraphRect();
			ar >> *((CGraphRect*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_TEXT:
			lp.m_GraphPrim[ i ] = new CGraphText();
			ar >> *((CGraphText*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_SPLINE:
			lp.m_GraphPrim[ i ] = new CGraphBSpline();
			ar >> *((CGraphBSpline*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_SPLINE_EX:
			lp.m_GraphPrim[ i ] = new CGraphBSplineEx();
			ar >> *((CGraphBSplineEx*) (lp.m_GraphPrim[ i ]));
			break;
		case FBtnGraphics::GRAPH_PRIM_QUATER_ARC:
			lp.m_GraphPrim[ i ] = new CGraphQuaterArc();
			ar >> *((CGraphQuaterArc*) (lp.m_GraphPrim[ i ]));
			break;
		default:
			Q_ASSERT( 0 );
		}
	}
	return ar >> lp.m_GraphPrimFormula >> lp.m_altGroup >> lp.m_ActiveAltGroup;
}

///////////////////////////////////////////////////////////////////////////////

long CNodeExInfo_GraphPrimitive::regetActiveAltGroup( CNode *master )
{
	if( master->GetType() == NODE_FORMULA )
	{
		for( long i = 0; i < getGraphPrimNumber(); i++ )
			if( m_GraphPrim[ i ]->canDraw( (CFormulaNode*) master ) )
				return GetAltGroupNo( i );
	}
	return 0;
}

void CNodeExInfo_GraphPrimitive::CopyGraphPrimitive( const CNodeExInfo_GraphPrimitive& a )
{
	m_altGroup = a.m_altGroup;
	m_GraphPrimFormula = a.m_GraphPrimFormula;
	m_GraphPrim.clear();
	for( long i = 0; i < a.m_GraphPrim.size(); i++ )
		AddGraphPrim( a.m_GraphPrim[ i ] );
}

void CNodeExInfo_GraphPrimitive::SetGraphPrimitiveLevel( long _level )
{
	for( long i = 0; i < getGraphPrimNumber(); i++ )
		m_GraphPrim[ i ]->SetLevel( _level );
}


int CNodeExInfo_GraphPrimitive::isEnabledCalc( CNode *master )
{
	setActiveAltGroup( master );

	long i;
	int c_enabled = master->isCannotDrawText() ? 1 : 0;
	for( i = 0; i < getGraphPrimNumber(); i++ )
		if( GetAltGroupNo( i ) == getActiveAltGroup() ) break;
	for( ; i < getGraphPrimNumber() && GetAltGroupNo( i ) == getActiveAltGroup(); i++ )
	{
		c_enabled = c_enabled ||
			m_GraphPrim[ i ]->isFormulaType() ||
		  	m_GraphPrim[ i ]->isCalcEnabled();
	}
	return c_enabled;
}

void CNodeExInfo_GraphPrimitive::AddGraphPrim( const CGraphPrim *tmplt )
{
	if( !tmplt ) return;
	CGraphPrim *newPrim;
	switch( tmplt->GetType() )
	{
	case FBtnGraphics::GRAPH_PRIM_LINE:
		newPrim = new CGraphLine( *(CGraphLine*)tmplt );
		break;
	case FBtnGraphics::GRAPH_PRIM_RECT:
		newPrim = new CGraphRect( *(CGraphRect*)tmplt );
		break;
	case FBtnGraphics::GRAPH_PRIM_TEXT:
		newPrim = new CGraphText( *(CGraphText*)tmplt );
		break;
	case FBtnGraphics::GRAPH_PRIM_SPLINE:
		newPrim = new CGraphBSpline( *(CGraphBSpline*)tmplt );
		break;
	case FBtnGraphics::GRAPH_PRIM_SPLINE_EX:
		newPrim = new CGraphBSplineEx( *(CGraphBSplineEx*)tmplt );
		break;
	case FBtnGraphics::GRAPH_PRIM_QUATER_ARC:
		newPrim = new CGraphQuaterArc( *(CGraphQuaterArc*)tmplt );
		break;
	default:
		Q_ASSERT( 0 );
		return;
	}
	m_GraphPrim.push_back( newPrim );
}

void CNodeExInfo_GraphPrimitive::AddGraphPrim( const CGraphPrimFormula& tmplt )
{
	CGraphPrim *newPrim;
	switch( tmplt.GetType() )
	{
	case FBtnGraphics::GRAPH_PRIM_LINE:
		newPrim = new CGraphLine();
		break;
	case FBtnGraphics::GRAPH_PRIM_RECT:
		newPrim = new CGraphRect();
		break;
	case FBtnGraphics::GRAPH_PRIM_TEXT:
		newPrim = new CGraphText();
		break;
	case FBtnGraphics::GRAPH_PRIM_SPLINE:
		newPrim = new CGraphBSpline();
		break;
	case FBtnGraphics::GRAPH_PRIM_SPLINE_EX:
		newPrim = new CGraphBSplineEx();
		break;
	case FBtnGraphics::GRAPH_PRIM_QUATER_ARC:
		newPrim = new CGraphQuaterArc();
		break;
	default:
		Q_ASSERT( 0 );
		return;
	}
	newPrim->Create( tmplt );
	newPrim->setTxtSize();
	m_GraphPrim.push_back( newPrim );
}

int CNodeExInfo_GraphPrimitive::CreateGraphPrimIndirect( const CGraphPrimFormula *grpr, long nCount )
{
	for( long i = 0; i < nCount; i++ )
	{
		CGraphPrimFormula& _p = (CGraphPrimFormula&)grpr[ i ];
		m_GraphPrimFormula.push_back( _p );
		AddGraphPrim( _p );
	}
	return 1;
}

int CNodeExInfo_GraphPrimitive::CreateGraphPrimIndirect( const QVector<CGraphPrimFormula>& srcGraphics )
{
	for( long i = 0; i < srcGraphics.size(); i++ )
	{
		m_GraphPrimFormula.push_back( srcGraphics.at(i) );
		AddGraphPrim( srcGraphics.at(i) );
	}
	return 1;
}

void CNodeExInfo_GraphPrimitive::CreateFrameCorrector( QVector<qint32>& dest )
{
	for( long i = 0; i < getGraphPrimNumber(); i++ )
	{
		if( m_GraphPrim[ i ]->GetBindToFrame() <= (long) dest.size() &&
			m_GraphPrim[ i ]->GetBindToFrame() > 0 )
			dest[ m_GraphPrim[ i ]->GetBindToFrame() - 1 ] = i;
	}
}

/////////////////////////////////////////////////////////////////////

int CNodeExInfo_GraphPrimitive::Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *master )
{
	long i;
	setActiveAltGroup( master );
	for( i = 0; i < getGraphPrimNumber(); i++ )
		if( GetAltGroupNo( i ) == getActiveAltGroup() )
			break;
	for( ; i < getGraphPrimNumber() && GetAltGroupNo( i ) == getActiveAltGroup(); i++ )
	{
		if( m_GraphPrim[ i ] == NULL )
			continue;

		m_GraphPrim[ i ]->Draw( fde, tl, master );
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////

int CNodeExInfo_GraphPrimitive::RecalculateGrph( CNode *master, FXmlBtn *active_cc )
{
	int ret = 0, cr;
	RectFde realPos;

	if( active_cc && 
		active_cc->isPositionRunExist() && 
		isEnabledCalc( master ) )
	{
		cr = master->RunCalc();
		ret = ret || cr;
	}
	for( long i = 0; i < getGraphPrimNumber(); i++ )
	{
		if( active_cc == 0 || (i >= active_cc->getGraphicsCount()) ||
			!active_cc->isFormulaTypeGraphics( i ) )
		{
			realPos = m_GraphPrimFormula[ i ].GetRealPosition( master );
			m_GraphPrim[ i ]->SetPosition( realPos );
		}
	}

	return ret;
}

/////////////////////////////////////////////////////////////////////

int CNodeExInfo_GraphPrimitive::correctFrameIndent( CFormulaNode *master, long grpr_no )
{
	if( grpr_no >= 0 && grpr_no < getGraphPrimNumber() )
	{
		return m_GraphPrim[ grpr_no ]->canDraw( master ) ?
			m_GraphPrim[ grpr_no ]->correctParentNodeSize( master ) : 0;
	}
	return 0;
}

int CNodeExInfo_GraphPrimitive::correctFrameIndentByIndex( CFormulaNode *master, long child_idx )
{
	setActiveAltGroup( master );
	int ret = 0, cr;
	for( long i = 0; i < getGraphPrimNumber(); i++ )
	{
		if( GetAltGroupNo( i ) != getActiveAltGroup() ) continue;
		if( m_GraphPrim[ i ]->getBound() == child_idx + 1 )
		{
			cr = m_GraphPrim[ i ]->correctParentNodeSize( master );
			ret = ret || cr;
		}
	}
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
