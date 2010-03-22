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

#ifndef __COMMON_GUICTRL_ACCELER_H__
#define __COMMON_GUICTRL_ACCELER_H__

#include <QVector>

#include "../hutils_global.h"
#include "HUtils/ihkflag.h"
#include "HUtils/ixml.h"

///////////////////////////////////////////////////////////////////////////

class HKAccelCode;
class HKAccel;
class HKAccelCodeEx;
class HKAccelerator;

///////////////////////////////////////////////////////////////////////////

#define HK_Key_All						0xFFFF
#define HK_Key_None						0x0000
#define HK_Key_Ctrl						0x0001
#define HK_Key_Alt						0x0002
#define HK_Key_Shift					0x0004

inline
qint32 getCode( const HKShortFlag flag, const qint16 key )
{
	return ((qint32( flag.getValue() ) << 16) | qint32( key ));
}

class HKAccelCode
{
protected:
	HKShortFlag	m_flag;
	short		m_key;

public:
	HKAccelCode( HKShortFlag flag = HKShortFlag( HK_Key_None ), short key = 0 );
	const HKAccelCode& operator = ( const HKAccelCode& ac );

	short getFlag() const;
	short isSetFlag( short flag ) const;
	void setFlag( short flag );
	void clearFlag( short flags );

	short getKey() const;
	void setKey( short key );

	long getAccelCode() const;

	friend QDataStream& operator << ( QDataStream& ar, const HKAccelCode& lp );
	friend QDataStream& operator >> ( QDataStream& ar, HKAccelCode& lp );
};

inline 
QDataStream& operator << ( QDataStream& ar, const HKAccelCode& lp )
{
	return ar << lp.m_flag << lp.m_key;
}

inline 
QDataStream& operator >> ( QDataStream& ar, HKAccelCode& lp )
{
	return ar >> lp.m_flag >> lp.m_key;
}

inline 
HKAccelCode::HKAccelCode( HKShortFlag flag, short key ) 
	: m_flag( flag )
	, m_key( key )
{
}

inline 
const HKAccelCode& HKAccelCode::operator = ( const HKAccelCode& ac )
{
	m_flag = ac.m_flag;
	m_key = ac.m_key;
	return *this;
}

inline 
short HKAccelCode::getFlag() const
	{ return m_flag.getValue(); }

inline 
short HKAccelCode::isSetFlag( short flag ) const
	{ return m_flag.isSetFlag( flag ); }

inline 
void HKAccelCode::setFlag( short flag ) 
	{ m_flag.setFlag( flag ); }

inline 
void HKAccelCode::clearFlag( short flag ) 
	{ m_flag.clearFlag( flag ); }

inline 
short HKAccelCode::getKey() const
	{ return m_key; }

inline 
void HKAccelCode::setKey( short key ) 
	{ m_key = key; }

inline 
long HKAccelCode::getAccelCode() const
	{ return getCode( m_flag, m_key ); }

typedef QVector<HKAccelCode>	HKAccelCodeVector;
typedef QVector<long>			HKLongVector;

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

class HKAccel
{
protected:
	HKAccelCodeVector	m_ac;

public:
	const HKAccel& operator = ( const HKAccel& accel );

	void addAccelCode( HKAccelCode& ac );
	long getAccelCodeCount() const;
	long getAccelCode( long i ) const;

	short getFlag( long i ) const;
	short isSetFlag( long i, short flag ) const;
	void setFlag( long i, short flag );
	void clearFlag( long i, short flags );

	short getKey( long i ) const;
	void setKey( long i, short key );

	friend QDataStream& operator << ( QDataStream& ar, const HKAccel& lp );
	friend QDataStream& operator >> ( QDataStream& ar, HKAccel& lp );
};

inline 
QDataStream& operator << ( QDataStream& ar, const HKAccel& lp )
{
	return ar << lp.m_ac;
}

inline 
QDataStream& operator >> ( QDataStream& ar, HKAccel& lp )
{
	return ar >> lp.m_ac;
}

inline 
const HKAccel& HKAccel::operator = ( const HKAccel& accel )
{
	m_ac = accel.m_ac;
	return *this;
}

inline 
void HKAccel::addAccelCode( HKAccelCode& ac )
	{ m_ac.push_back( ac ); }

inline 
long HKAccel::getAccelCodeCount() const
	{ return (long) m_ac.size(); }

inline 
long HKAccel::getAccelCode( long i ) const
	{ return m_ac.at(i).getAccelCode(); }

inline 
short HKAccel::getFlag( long i ) const
	{ return m_ac[ i ].getFlag(); }

inline 
short HKAccel::isSetFlag( long i, short flag ) const
	{ return m_ac[ i ].isSetFlag( flag ); }

inline 
void HKAccel::setFlag( long i, short flag ) 
	{ m_ac[ i ].setFlag( flag ); }

inline 
void HKAccel::clearFlag( long i, short flag ) 
	{ m_ac[ i ].clearFlag( flag ); }

inline 
short HKAccel::getKey( long i ) const
	{ return m_ac[ i ].getKey(); }

inline 
void HKAccel::setKey( long i, short key ) 
	{ m_ac[ i ].setKey( key ); }

typedef QVector<HKAccel>		HKAccelVector;

///////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////

typedef HKMap<long, HKAccelCodeEx*> HKLong2pAccelCodeExMap;

class HKAccelCodeEx : public HKAccelCode
{
protected:
	HKLong2pAccelCodeExMap*	m_pacmap;
	HKLongVector*			m_pid;

public:
	HKAccelCodeEx(); 
	~HKAccelCodeEx();

	const HKAccelCodeEx& operator = ( const HKAccelCodeEx& ace );

	int create( HKAccel& accel, HKLongVector& id, long i = 0 );
	int destroy( void );

	int verifyAccel( void );
	int procAccel( HKLongVector& code2id );
	int removeAccel( HKAccel& accel, long codeno );
};

inline
HKAccelCodeEx::HKAccelCodeEx() 
	: HKAccelCode()
	, m_pacmap( 0 )
	, m_pid( 0 )
{
}

inline
HKAccelCodeEx::~HKAccelCodeEx()
{
	destroy();
}

inline
const HKAccelCodeEx& HKAccelCodeEx::operator = ( const HKAccelCodeEx& ace )
{
	HKAccelCode::operator = ( ace );

	m_pacmap = ace.m_pacmap;
	m_pid = ace.m_pid;

	return *this;
}

inline 
int HKAccelCodeEx::create( HKAccel& accel, HKLongVector& id, long i )
{
	setFlag( accel.getFlag( i ) );
	setKey( accel.getKey( i ) );

	i++;
	if( i < accel.getAccelCodeCount() )
	{
		if( m_pacmap == 0 ) 
			m_pacmap = new HKLong2pAccelCodeExMap();

		HKLong2pAccelCodeExMap::iterator it = m_pacmap->find( accel.getAccelCode( i ) );
		if( it != m_pacmap->end() ) 
			return -1;

		HKAccelCodeEx *pAccelCodeEx = new HKAccelCodeEx();
		if( pAccelCodeEx->create( accel, id, i ) == -1 )
		{
			delete pAccelCodeEx;
			return -1;
		}

		(*m_pacmap)[ accel.getAccelCode( i ) ] = pAccelCodeEx;
	}
	else
	{
		if( m_pid == 0 ) 
			m_pid = new HKLongVector();
		*m_pid = id;
	}

	return 0;
}

inline
int HKAccelCodeEx::verifyAccel( void )
	{ return (m_pacmap && m_pid == 0) || (m_pacmap == 0 && m_pid ); }

inline
int HKAccelCodeEx::destroy( void )
{
	if( m_pacmap != NULL ) 
	{
		HKLong2pAccelCodeExMap::iterator it, end = m_pacmap->end();
		for( it = m_pacmap->begin(); it != end; it++ )
			delete it->second;
		delete m_pacmap;
		m_pacmap = NULL;
	}

	if( m_pid != NULL )
	{
		delete m_pid;
		m_pid = NULL;
	}

	return 0;
}

// Return values:

#define HK_AccelCode_Cancel				0	// cancel accelerator 
#define HK_AccelCode_Wait				1	// wait for continue  
#define HK_AccelCode_Exec				2	// execute command    

inline
int HKAccelCodeEx::procAccel( HKLongVector& code2id )
{
	HKAccelCodeEx *pAccelCodeEx = this;
	long i, nCount = (long) code2id.size();
	for( i = 1; i < nCount; i++ )
	{
		if( !pAccelCodeEx->verifyAccel() )
			return HK_AccelCode_Cancel;

		if( pAccelCodeEx->m_pacmap == 0 ) 
			break;

		HKLong2pAccelCodeExMap::iterator it = pAccelCodeEx->m_pacmap->find( code2id[ i ] );
		if( it == m_pacmap->end() ) 
			return HK_AccelCode_Cancel;
		pAccelCodeEx = it->second;
	}
	if( pAccelCodeEx->m_pid )
	{
		code2id = *pAccelCodeEx->m_pid;
		return HK_AccelCode_Exec;
	}
	return HK_AccelCode_Wait;
}

inline
int HKAccelCodeEx::removeAccel( HKAccel& accel, long codeno )
{
	if( m_pacmap == 0 ) 
		return 0;

	HKLong2pAccelCodeExMap::iterator it = m_pacmap->find( accel.getAccelCode( codeno ) );
	if( it != m_pacmap->end() )		// есть такая комбинация
		if( it->second->removeAccel( accel, codeno + 1 ) == 0 )
			m_pacmap->erase( it );	// внутри никого не оказалось - можно удалять

	return (int) m_pacmap->size();
}

///////////////////////////////////////////////////////////////////////////
// 
///////////////////////////////////////////////////////////////////////////

typedef HKMap<long, HKAccelCodeEx> HKLong2AccelCodeExMap;

class HKAccelerator
{
protected:
	HKLong2AccelCodeExMap	m_acmap;
	HKLongVector			m_code2id;

public:
	int addAccel( HKAccel& accel, HKLongVector& id );
	int delAccel( HKAccel& accel );
	int removeAccel( HKAccel& accel );

	int accelcode2Sequence( HKAccelCode& accelcode );
	void getSequence( HKLongVector& code2id );
	void clearSequence();
};

inline
int HKAccelerator::addAccel( HKAccel& accel, HKLongVector& id )
	{ return m_acmap[ accel.getAccelCode( 0 ) ].create( accel, id ); }

inline
int HKAccelerator::delAccel( HKAccel& accel )
	{ return (int) m_acmap.erase( accel.getAccelCode( 0 ) ); }

inline
int HKAccelerator::removeAccel( HKAccel& accel )
{
	HKLong2AccelCodeExMap::iterator it = m_acmap.find( accel.getAccelCode( 0 ) );
	if( it != m_acmap.end() )		// есть такая комбинация
		if( it->second.removeAccel( accel, 1 ) == 0 )
			m_acmap.erase( it );	// внутри никого не оказалось - можно удалять

	return (int) m_acmap.size();
}

inline
int HKAccelerator::accelcode2Sequence( HKAccelCode& accelcode )
{
	m_code2id.push_back( accelcode.getAccelCode() );
	HKLong2AccelCodeExMap::iterator it = m_acmap.find( m_code2id[ 0 ] );
	return (it != m_acmap.end() ? it->second.procAccel( m_code2id ) : HK_AccelCode_Cancel);
}

inline
void HKAccelerator::getSequence( HKLongVector& code2id )
{
	code2id = m_code2id;
	clearSequence();
}

inline
void HKAccelerator::clearSequence()
	{ m_code2id.clear(); }

//inline 
//short convertCharacter( short key )
//	{ return (key > VK_RMENU ? key - 0x0080 : key); }

///////////////////////////////////////////////////////////////////////////
#endif // __COMMON_GUICTRL_ACCELER_H__
