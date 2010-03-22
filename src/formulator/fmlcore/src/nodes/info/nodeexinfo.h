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

#if !defined( __FORMULATOR_NODEEXINFO_H__ )
#define __FORMULATOR_NODEEXINFO_H__

#include <QVector>

#include "HUtils/idrawstruct.h"

#include "../../settings/options.h"
#include "../../button/toolbarset.h"
#include "../../graphics/baseratio.h"

/////////////////////////////////////////////////////////////////////

class CFormulaNode;

class __HERMITECH_FORMULIB__ CNodeExInfo_Calc
{
protected:
	QString m_button_id;
	qint32 m_toolbar_no, m_button_no;
	FXmlBtn *m_pBtn;

public:
	CNodeExInfo_Calc()							{ m_toolbar_no = m_button_no = -1; m_pBtn = 0; m_button_id = ""; }
	void setCalcButtonID( const QString& src )	{ m_button_id = src; }
	const QString& getCalcButtonID() const		{ return m_button_id; }
	int isAble2Glue() const						{ return m_button_id.length() == 0 && m_pBtn == 0; }

	void setCalcPtr( FXmlBtn *_m_pBtn )			{ m_pBtn = _m_pBtn; }
	void setCalcPtr()							{ setCalcPtr( ::getCurrentToolBarSet()->getButtonCalc( m_button_id ) ); }
	FXmlBtn* getCalc()							{ return m_pBtn; }
	FXmlBtn* regetActiveCalc()					{ setCalcPtr(); return m_pBtn; }

	void setCalcCoord( long toolbar_no, long button_no )	{ m_toolbar_no = toolbar_no; m_button_no = button_no; }
	void setCalcCoord( const QString& button_id )			{ m_button_id = button_id; }

	void CopyCalc( const CNodeExInfo_Calc& a );

	friend QDataStream& operator << ( QDataStream& ar, const CNodeExInfo_Calc& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CNodeExInfo_Calc& lp );
};

///////////////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const CNodeExInfo_Calc& lp )
{
	return ar << lp.m_toolbar_no << lp.m_button_no << lp.m_button_id;
}

inline
QDataStream& operator >> ( QDataStream& ar, CNodeExInfo_Calc& lp )
{
	QDataStream& ret = ar >> lp.m_toolbar_no >> lp.m_button_no >> lp.m_button_id;
	lp.setCalcPtr();
	return ret;
}

inline
void CNodeExInfo_Calc::CopyCalc( const CNodeExInfo_Calc& a )
{
	m_toolbar_no = a.m_toolbar_no;
	m_button_no = a.m_button_no;
	m_button_id = a.m_button_id;
	setCalcPtr();
}

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CNodeExInfo_GraphPrimitive
{
protected:
	QVector<CGraphPrimFormula>	m_GraphPrimFormula;
	QVector<CGraphPrim*>		m_GraphPrim;

	QVector<qint32>				m_altGroup;
	qint32						m_ActiveAltGroup;

public:
	CNodeExInfo_GraphPrimitive()
		{ m_ActiveAltGroup = 0; }
	virtual ~CNodeExInfo_GraphPrimitive();
	long getGraphPrimNumber() const
		{ return (long) m_GraphPrim.size(); }

	RealFde getGraphPrimitivePositionCoord( long gr, long pos ) const
		{ return m_GraphPrim.at(gr)->getPosition(pos); }
	RectFde getGraphPrimitivePositionRect( long gr ) const
		{ return m_GraphPrim.at(gr)->GetPositionRect(); }
	void setGraphPrimitivePositionCoord( long gr, long pos, RealFde value )
		{ m_GraphPrim[ gr ]->getPosition( pos ) = value; }

	void setLinethickness( long i, RealFde val )
	{
		if(i >= 0 && i < getGraphPrimNumber())
		{
			m_GraphPrimFormula[ i ].SetWidth( val );
			m_GraphPrim[ i ]->SetWidth( val );
		}
	}
	RealFde getLinethickness( long i )
		{ return (i >= 0 && i < getGraphPrimNumber()) ? m_GraphPrimFormula.at(i).GetWidth() : -1.0; }

	void setLineColor( QColor val )
	{
		for( long i = 0; i < getGraphPrimNumber(); i++ )
		{
			m_GraphPrimFormula[ i ].SetColor( val );
			m_GraphPrim[ i ]->SetColor( val );
		}
	}
	void setLineColor( long i, QColor val )
	{
		if(i >= 0 && i < getGraphPrimNumber())
		{
			m_GraphPrimFormula[ i ].SetColor( val );
			m_GraphPrim[ i ]->SetColor( val );
		}
	}

	long getActiveAltGroup()
		{ return m_ActiveAltGroup; }
	long GetAltGroupNo( long i )
		{ return m_altGroup.at(i); }
	void FlushAltGroup()
		{ m_altGroup.clear(); }
	int AddAltGroup( long i )
	{
		int iRet = m_altGroup.size();
		m_altGroup.push_back( i );
		return iRet;
	}
	void VerifyAltGroup()
	{
		long sz = m_altGroup.size();
		if( sz != getGraphPrimNumber() )
			m_altGroup.resize( getGraphPrimNumber() );
		for( ; sz < m_altGroup.size(); sz++ )
			m_altGroup[ sz ] = 0;
	}

	void setActiveAltGroup( CNode *master )
		{ m_ActiveAltGroup = regetActiveAltGroup( master ); }
	long regetActiveAltGroup( CNode *master );
	int isEnabledCalc( CNode *master );

	void CopyGraphPrimitive( const CNodeExInfo_GraphPrimitive& a );

	void AddGraphPrim( const CGraphPrimFormula& p );
	void AddGraphPrim( const CGraphPrim *tmplt );

	void SetGraphPrimitiveLevel( long _level );

	int CreateGraphPrimIndirect( const CGraphPrimFormula *grpr, long nCount );
	int CreateGraphPrimIndirect( const QVector<CGraphPrimFormula>& srcGraphics );
	void CreateFrameCorrector( QVector<qint32>& dest );

	virtual int Draw( CFmlDrawEngine& fde, const PointFde& tl, CNode *master );

	virtual int RecalculateGrph( CNode *master, FXmlBtn *active_cc = 0 );
	int correctFrameIndent( CFormulaNode *master, long grpr_no );
	int correctFrameIndentByIndex( CFormulaNode *master, long child_idx );

	friend QDataStream& operator << ( QDataStream& ar, const CNodeExInfo_GraphPrimitive& lp );
	friend QDataStream& operator >> ( QDataStream& ar, CNodeExInfo_GraphPrimitive& lp );
};

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ CNodeExInfo
	: public CNodeExInfo_Calc
	, public CNodeExInfo_GraphPrimitive
{
public:
	CNodeExInfo() {}
	virtual ~CNodeExInfo() {}
	CNodeExInfo( CNodeExInfo *pNodeExInfo )	{ setNodeExInfo( *pNodeExInfo ); }
	int isAble2Glue() const					{ return CNodeExInfo_Calc::isAble2Glue(); }
	void setNodeExInfo( const CNodeExInfo& a )
	{
		CopyCalc( a );
		CopyGraphPrimitive( a );
	}
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const CNodeExInfo& lp )
{
	return ar << (const CNodeExInfo_Calc&) lp << (const CNodeExInfo_GraphPrimitive&) lp;
}

inline
QDataStream& operator >> ( QDataStream& ar, CNodeExInfo& lp )
{
	return ar >> (CNodeExInfo_Calc&) lp >> (CNodeExInfo_GraphPrimitive&) lp;
}
///////////////////////////////////////////////////////////////////////////////

#endif
