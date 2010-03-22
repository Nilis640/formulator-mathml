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

#ifndef __COMMON_FASTPARAM_H__
#define __COMMON_FASTPARAM_H__

#include "param.h"

///////////////////////////////////////////////////////////////////////////

class HUTILS_EXPORT HKFastParameterList
{
private:
	std::vector<HKParameter> m_list_value;
	HKMap<QString, long> m_list_name;

public:
	HKFastParameterList( void );
	HKFastParameterList( HKFastParameterList& p );

	HKFastParameterList& operator = ( HKFastParameterList& p );

	unsigned long getParameterNumber();
	void Flush( void );
	void setParameter( const QString& key, HKParameter& param );
	void getParameter( unsigned long index, HKParameter& param );
	long getParameterIndex( const QString& key );
	HKParameter* getParameter( const QString& key );

	int Read_XML( AST_TXML_Tag *root, DRMessageManager& dmm, long dmm_page = 0 );

	//virtual void store( CArchive& ar );
	//virtual void load( CArchive& ar );

	//friend CArchive& operator << ( CArchive& ar, HKFastParameterList& param );
	//friend CArchive& operator >> ( CArchive& ar, HKFastParameterList& param );
};

///////////////////////////////////////////////////////////////////////////

inline
HKFastParameterList::HKFastParameterList( void ) : 
	m_list_value(), m_list_name()
{
}

inline
HKFastParameterList::HKFastParameterList( HKFastParameterList& p ) : 
	m_list_value( p.m_list_value ), m_list_name( p.m_list_name )
{
}

inline
HKFastParameterList& HKFastParameterList::operator = ( HKFastParameterList& p ) 
{
	m_list_value = p.m_list_value;
	m_list_name = p.m_list_name;
	return *this;
}

inline
unsigned long HKFastParameterList::getParameterNumber()
{
	return (unsigned long) m_list_value.size();
}

inline
void HKFastParameterList::Flush( void )
{
	m_list_value.clear();
	m_list_name.clear();
}

inline
void HKFastParameterList::getParameter( unsigned long index, HKParameter& param )
{
	param = m_list_value[ index ];
}

inline
long HKFastParameterList::getParameterIndex( const QString& key )
{
	if ( m_list_name.is_find( key ) == 0 )
		return -1;
	return m_list_name[ key ];
}

inline
HKParameter* HKFastParameterList::getParameter( const QString& key )
{
	if ( m_list_name.is_find( key ) == 0 )
		return 0;
	return &m_list_value[ m_list_name[ key ] ];
}

/*
inline
void HKFastParameterList::store( CArchive& ar )
{
	ar << m_list_value;
	ar << m_list_name;
}

inline
void HKFastParameterList::load( CArchive& ar )
{
	ar >> m_list_value;
	ar >> m_list_name;
}

inline
CArchive& operator << ( CArchive& ar, HKFastParameterList& param )
{
	param.store( ar );
	return ar;
}

inline
CArchive& operator >> ( CArchive& ar, HKFastParameterList& param )
{
	param.load( ar );
	return ar;
}
*/

///////////////////////////////////////////////////////////////////////////

#endif // __COMMON_FASTPARAM_H__
