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

#if !defined( __COMMON_UTILS_AUTOHEAP_H__ )
#define __COMMON_UTILS_AUTOHEAP_H__

#include "HUtils/ihkmap.h"

#define HU_DEFAULT_RTHEAP2FREE_SIZE	128

template <class T> class HU_Heap2Free
{
protected:
	HKMap<T*, long>	m_map;

public :
	MIArray<T> ptr_list;

	HU_Heap2Free( int delete_on_desctruct = 1, long scb = HU_DEFAULT_RTHEAP2FREE_SIZE )
		: ptr_list( delete_on_desctruct ? MIArray<T>::DeleteItems : MIArray<T>::LeaveItems, scb, 0, scb )
	{
	}
	~HU_Heap2Free( void )
	{
	}

	unsigned long Heap2FreeDim( void )
	{
		return ptr_list.ArrayDim();
	}
	void*& Get( unsigned i )
	{
		return ptr_list[ i ];
	}

	void Free( void );
	void Add2List( HU_Heap2Free& to_add );
	T*& Add2List( T*& ptr_to_list );
};

template <class T>
T*& HU_Heap2Free<T>::Add2List( T*& ptr_to_list )
{
	if( m_map.find( ptr_to_list ) == m_map.end() )
		m_map[ ptr_to_list ] = ptr_list.Add( ptr_to_list );
	return ptr_to_list;
}

template <class T>
void HU_Heap2Free<T>::Add2List( HU_Heap2Free& to_add )
{
	for( unsigned i = 0; i < to_add.Heap2FreeDim(); i++ )
		Add2List( to_add.Get( i ) );
}

template <class T>
void HU_Heap2Free<T>::Free( void )
{
	for( unsigned i = 0; i < ptr_list.ArrayDim(); i++ )
	{
		if( ptr_list[ i ] )
		{
			delete ptr_list[ i ];
			ptr_list[ i ] = 0;
		}
	}
	ptr_list.Flush();
}

#endif
