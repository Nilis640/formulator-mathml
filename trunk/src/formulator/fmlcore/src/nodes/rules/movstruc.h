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

#ifndef __FORMULATOR_NODES_RULES_MOVSTRUCT_H__
#define __FORMULATOR_NODES_RULES_MOVSTRUCT_H__

#include <float.h>
#include "HUtils/ihkflag.h"
#include "../info/nodeinfo.h"

/////////////////////////////////////////////////////////////////////////////

class MotionData
{
public:
	MotionData( int fLeft, int fSel, int fWord, int fAllowIntoText, SelectNode &sn, SelectInfo &si );

	SelectNode& GetSN( void );
	SelectNode& GetSB( void );
	SelectNode& GetSE( void );

	SelectInfo& GetSelectInfo( void );

	int IsLeft( void );
	int IsSel( void );
	int IsWord( void );
	int IsAllowIntoText( void );

	void Update( void );

protected:
	int IsSel2Left( void );
	int IsSimple( void );

protected:
	SelectNode &m_sn;
	SelectInfo &m_si;

	HKShortFlag m_shf;
};

/////////////////////////////////////////////////////////////////////////////

#define HKSHORTFLAG_FORMULATOR_LEFT					0x0001
#define HKSHORTFLAG_FORMULATOR_SEL					0x0002
#define HKSHORTFLAG_FORMULATOR_WORD					0x0004
#define HKSHORTFLAG_FORMULATOR_SEL2LEFT				0x0008
#define HKSHORTFLAG_FORMULATOR_ALLOWINTOTEXT		0x0010

/////////////////////////////////////////////////////////////////////////////

inline
MotionData::MotionData( int fLeft, int fSel, int fWord,	int fAllowIntoText, SelectNode &sn, SelectInfo &si )
	: m_sn( sn )
	, m_si( si )
	, m_shf( 0 )
{
	if( fLeft ) m_shf.setFlag( HKSHORTFLAG_FORMULATOR_LEFT );
	if( fSel ) m_shf.setFlag( HKSHORTFLAG_FORMULATOR_SEL );
	if( fWord ) m_shf.setFlag( HKSHORTFLAG_FORMULATOR_WORD );
	if( fWord ) m_shf.setFlag( HKSHORTFLAG_FORMULATOR_WORD );
	if( fAllowIntoText ) m_shf.setFlag( HKSHORTFLAG_FORMULATOR_ALLOWINTOTEXT );

	if( fSel )
	{
		if( m_si.GetBegin().IsEmpty() )
			m_si.GetBegin() = m_si.GetEnd() = m_sn;

		if( m_si.GetBegin() == m_si.GetEnd() )
		{
			if( fLeft )
				m_shf.setFlag( HKSHORTFLAG_FORMULATOR_SEL2LEFT );
		}
		else 
		{
			if( m_si.GetBegin() == sn )
				m_shf.setFlag( HKSHORTFLAG_FORMULATOR_SEL2LEFT );
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

inline 
SelectNode& MotionData::GetSN( void )
	{ return m_sn; }

inline 
SelectNode& MotionData::GetSB( void )
	{ return m_si.GetBegin(); }

inline 
SelectNode& MotionData::GetSE( void )
	{ return m_si.GetEnd(); }

inline
SelectInfo& MotionData::GetSelectInfo( void )
	{ return m_si; }

inline
int MotionData::IsLeft( void )
	{ return m_shf.isSetFlag( HKSHORTFLAG_FORMULATOR_LEFT ); }

inline
int MotionData::IsSel( void )
	{ return m_shf.isSetFlag( HKSHORTFLAG_FORMULATOR_SEL ); }

inline
int MotionData::IsWord( void )
	{ return m_shf.isSetFlag( HKSHORTFLAG_FORMULATOR_WORD ); }

/////////////////////////////////////////////////////////////////////////////

inline
int MotionData::IsAllowIntoText( void )
	{ return m_shf.isSetFlag( HKSHORTFLAG_FORMULATOR_ALLOWINTOTEXT ); }

/////////////////////////////////////////////////////////////////////////////

inline
int MotionData::IsSel2Left( void )
	{ return m_shf.isSetFlag( HKSHORTFLAG_FORMULATOR_SEL2LEFT ); }

/////////////////////////////////////////////////////////////////////////////

struct Nearest_Node
{
	Nearest_Node( const PointFde &pt ) 
		: m_pNode( NULL )
		, m_value( FLT_MAX )
		, m_pt( pt )
	{
	};

	CNode*		m_pNode;
	RealFde		m_value;
	PointFde	m_pt;
};

/////////////////////////////////////////////////////////////////////////////
#endif	//__FORMULATOR_NODES_RULES_MOVSTRUCT_H__
