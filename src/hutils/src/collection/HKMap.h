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

#ifndef __HUTILS_HKMAP_H__
#define __HUTILS_HKMAP_H__

#include <QDataStream>
#include <QFile>
#include <QString>
#include <QVector>
#include <QMap>
#include <map>

#include "../hutils_global.h"
#include "HKArray.h"

template <class Key, class T> class HKMap : public std::map<Key, T>
{
public:
	HKMap() : std::map<Key, T>() {}
	HKMap( const HKMap& x ) : std::map<Key, T>( x ) {}
	HKMap( const std::map<Key, T>& x ) : std::map<Key, T>( x ) {}

	// step by step access
	bool tofirst()
	{
		_nav = std::map<Key, T>::begin();
		return (_nav != std::map<Key, T>::end());
	}
	bool tonext()
	{
		_nav++;
		return (_nav != std::map<Key, T>::end());
	}
	void get( Key& key, T& val )
	{
		key = (*_nav).first;
		val = (*_nav).second;
	}
	// find operation
	T* is_find( Key& key )
	{
		typename std::map<Key, T>::iterator i = std::map<Key, T>::find( key );
		return i == std::map<Key, T>::end() ? 0 : &(i->second);
	}
	const T* is_find( const Key& key ) const
	{
		//const_iterator nav = find( key );
		//return (find( key ) == end() ? 0 : &(*find( key )).second);
		typename std::map<Key, T>::const_iterator i = std::map<Key, T>::find( key );
		return i == std::map<Key, T>::end() ? 0 : &(i->second);
	}

protected:
	typename std::map<Key, T>::iterator	_nav;
};

//#pragma warning( pop )

//////////////////////////////////////////////////////////////////////

template <class T> class HKMap2Long
{
public:
	HKMap2Long()
		: m_vector( 32, 0, 32 )
	{}
	HKMap2Long( const HKMap2Long& a ) 
		: m_map( a.m_map )
		, m_vector( a.m_vector )
	{}
	HKMap2Long( const T* src, size_t n )
		: m_map()
		, m_vector( n, 0, n )
	{
		T _t;
		for( size_t i = 0; i < n; i++ )
		{
			_t = src[ i ];
			Add( _t );
		}
	}

	long* isFind( T& key )
	{
		return ( m_map.find( key ) == m_map.end() ) ? 0 : &m_map[ key ];
	}
	long Add( T& key )
	{
		long idx = m_vector.Add( key );
		m_map[ key ] = idx;
		return idx;
	}
	T* getItem( long idx )
	{
		return (idx >= 0 && idx < (long) m_vector.ArrayDim()) ? &m_vector[ idx ] : 0;
	}
	long getItemCount( void )
	{
		return (long) m_vector.ArrayDim();
	}
protected:
	HKMap<T,long>	m_map;
	MArray<T>		m_vector;
};

typedef HKMap2Long<QString> HKMapString2Long;

//////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT HKStringMap : public HKMap<QString, QString>
{
public:
	HKStringMap( void );
	HKStringMap( const HKStringMap& x );
	~HKStringMap( void );
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const HKStringMap& lp )
{
	return ar << QMap<QString, QString>( (std::map<QString, QString>) lp );
}

inline
QDataStream& operator >> ( QDataStream& ar, HKStringMap& lp )
{
	QMap<QString, QString> mp;
	QDataStream& ret = ar >> mp;
	std::map<QString, QString> mpData = mp.toStdMap();
	lp.clear();
	lp.insert( mpData.begin(), mpData.end() );
	return ret;
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT HKKeywordMapString2Long
{
protected:
	QMap<QString, qint32>	m_map;
	QVector<QString>		m_vector;
	QString					m_id;

public:
	HKKeywordMapString2Long()	{ m_id = ""; }
	HKKeywordMapString2Long( const HKKeywordMapString2Long& a ) 
		: m_map( a.m_map )
		, m_vector( a.m_vector )
		, m_id( a.m_id )
	{}
	HKKeywordMapString2Long( const QString *args, long n )
		: m_map()
		, m_vector(n)
	{
		m_id = "";
		for( long i = 0; i < n; i++ )
		{
			m_vector[ i ] = args[ i ];
			m_map[ args[ i ] ] = i;
		}
	}
	virtual ~HKKeywordMapString2Long() {}

	qint32* isFind( QString& key )
	{
		return (m_map.find( key ) == m_map.end()) ? 0 : &m_map[ key ];
	}
	long Add( QString& key )
	{
		long idx = m_vector.size();
		m_vector.push_back( key );
		m_map[ key ] = idx;
		return idx;
	}
	QString* getItem( long idx )
	{
		return (idx >= 0 && idx < m_vector.size()) ? &m_vector[ idx ] : 0;
	}


	QString& getID()
		{ return m_id; }
	void Reset()
		{ m_id = ""; }
	long GetRead()
		{ return (long) m_id.length(); }

	long Analisys( QString text )
	{
		qint32 *nID = isFind( text );
		if( nID )
		{
			m_id = text;
			return long(*nID + 1);
		}
		m_id = "";
		return 0;
	}

	virtual int ReadFile( const QString& path );
	virtual int WriteFile( const QString& path );

	friend QDataStream& operator << ( QDataStream& ar, const HKKeywordMapString2Long& lp );
	friend QDataStream& operator >> ( QDataStream& ar, HKKeywordMapString2Long& lp );
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const HKKeywordMapString2Long& lp )
{
	return ar << lp.m_id << lp.m_map << lp.m_vector;
}

inline
QDataStream& operator >> ( QDataStream& ar, HKKeywordMapString2Long& lp )
{
	return ar >> lp.m_id >> lp.m_map >> lp.m_vector;
}

inline
int HKKeywordMapString2Long::ReadFile( const QString& path )
{
	QFile file( path );
	if( !file.open(QIODevice::ReadOnly) ) return -1;
	QDataStream ar( &file );
	ar >> *this;
	file.close();
	return 0;
}

inline
int HKKeywordMapString2Long::WriteFile( const QString& path )
{
	QFile file( path );
	if( !file.open(QIODevice::WriteOnly) ) return -1;
	QDataStream ar( &file );
	ar << *this;
	file.close();
	return 0;
}

/////////////////////////////////////////////////////////////////////

#endif
