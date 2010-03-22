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

#if !defined( __FORMULATOR_EXT_FORMULA_H__ )
#define __FORMULATOR_EXT_FORMULA_H__
 
#include <QVector>
#include <map>
#include "HUtils/idrawstruct.h"
#include "../../fmlcore_global.h"

class CParentNode;
typedef std::map<RealFde, RealFde> CExCalcMap;

class CFormulaPos  
{
protected:
	RectFde		m_pos;
	qint32		m_align;
	qint32		m_level;

public:
	CFormulaPos( const RectFde& pos = RectFde(), long align = 0, long level = 0 )
		: m_pos( pos ), m_align( align ), m_level( level ) {}
	CFormulaPos( const CFormulaPos& fp )
		: m_pos( fp.m_pos ), m_align( fp.m_align ), m_level( fp.m_level ) {}
	const CFormulaPos& operator = ( const CFormulaPos& fp )
		{ m_pos = fp.m_pos; m_align = fp.m_align; m_level = fp.m_level; return *this; }

	const RectFde& GetPecentPosition( void ) const { return m_pos; };
	void SetPecentPosition( const RectFde& pos ) { m_pos = pos; };

	long GetAlignType( void ) const { return m_align; };
	void SetAlignType( long align ) { m_align = align; };

	long GetLevel( void ) const { return m_level; };
	void SetLevel( long level ) { m_level = level; };

	friend QDataStream& operator << ( QDataStream& ar, const CFormulaPos& pos );
	friend QDataStream& operator >> ( QDataStream& ar, CFormulaPos& pos );
};

///////////////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const CFormulaPos& fp )
	{ return ar << fp.m_pos << fp.m_align << fp.m_level; }

inline
QDataStream& operator >> ( QDataStream& ar, CFormulaPos& fp )
	{ return ar >> fp.m_pos >> fp.m_align >> fp.m_level; }

///////////////////////////////////////////////////////////////////////////////
//

class CFormulaExCalc  
{
public:
	CFormulaExCalc();
	CFormulaExCalc( CFormulaExCalc* pFormulaEx );
	virtual ~CFormulaExCalc();

public:
	long GetSlotCount( void );
	CFormulaPos& GetSlot( long i );
	void AddSlot( const CFormulaPos& fp );

public:

	virtual void Store( QDataStream& ar ) const;
	virtual void Load( QDataStream& ar );
	virtual SizeFde RecalculateSize( CParentNode *pParentNode );
	virtual SizeFde GetDefaultSize( void );
	virtual RectFde GetRealPosition( RectFde rc );

protected:
	SizeFde CalculateDefaultSize( CParentNode *pParentNode );
	SizeFde CalculateFormulaSize( const SizeFde& defSize, CParentNode *pParentNode );
	SizeFde CalculateSizeByPercents( QVector<RectFde>& rects, QVector<SizeFde>& sizes );
	RealFde GetMapValue( RealFde value, CExCalcMap& ps );

protected:
	QVector<CFormulaPos>		m_slots;

	SizeFde		m_defSize;
	CExCalcMap	m_psx;
	CExCalcMap	m_psy;
};

///////////////////////////////////////////////////////////////////////////////

inline
long CFormulaExCalc::GetSlotCount( void )
	{ return (long) m_slots.size(); }

inline
CFormulaPos& CFormulaExCalc::GetSlot( long i )
	{ return m_slots[ i ]; }

inline
void CFormulaExCalc::AddSlot( const CFormulaPos& fp )
	{ m_slots.push_back( fp ); }

inline
SizeFde CFormulaExCalc::GetDefaultSize( void )
	{ return m_defSize; }

///////////////////////////////////////////////////////////////////////////////

#endif
