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

#include "mmd_set_op.h"

///////////////////////////////////////////////////////////////////////////////

double fact( unsigned long ii )
{
	if( !ii ) return 1.0;
	double p = (double)ii;
	for( --ii; ii > 0; ii-- )
		p *= ii;
	return p;
}
double int_power( unsigned long base, unsigned long power )
{
	double p = 1;
	for( unsigned long i = 0; i < power; i++ )
		p *= base;
	return p;
}

double MMD_Set::GetCountOf_SubSet( unsigned m )
{
	//return int_power( 2, getCount() );
	return GetCountOf_ComboC_First( m );
}

double MMD_Set::GetCountOf_ComboC_First( unsigned m )
{
	return fact(getCount()) / (fact(m) * fact(getCount() - m));
}

double MMD_Set::GetCountOf_ComboA_First( unsigned m )
{
	return fact(getCount()) / fact(getCount() - m);
}

double MMD_Set::GetCountOf_ComboP_First( void )
{
	return fact(getCount());
}

double MMD_Set::GetCountOf_ComboARet_First( unsigned m )
{
	return int_power( getCount(), m );
}

double MMD_Set::GetCountOf_ComboCRet_First( unsigned /*m*/ )
{
	return 1.0;
}

MMD_Set* MMD_Set::GetComboC_First( unsigned m )
{
	if( !getCount() || ( m > getCount() ) || !m )
		return 0;

	m_to_combine = m;
	combo_storage.Flush();

	MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
	for( long i = 0; i < (long) m; i++ )
	{
		combo_storage.Add( i );
		ret->Add( at( i ) );
	}
	return ret;
}

MMD_Set* MMD_Set::GetComboC_Next( void )
{
	if( !getCount() || ( m_to_combine > (long) getCount() ) || !m_to_combine )
		return 0;

	long i, j;

	if( combo_storage.ArrayDim() != m_to_combine )
		return 0;

	if( combo_storage[ m_to_combine - 1 ] + 1 == (long) getCount() )
	{
		for( i = m_to_combine - 1; i > 0; i-- )
			if( combo_storage[ i - 1 ] + 1 != combo_storage[ i ] )
			{
				combo_storage[ i - 1 ]++;
				for ( j = i; j < m_to_combine; j++ )
					combo_storage[ j ] = combo_storage[ j - 1 ] + 1;
				MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
				for( long k = 0; k < m_to_combine; k++ )
					ret->Add( at( combo_storage[ k ] ) );
				return ret;
			}
	}
	else
	{
		combo_storage[ m_to_combine - 1 ]++;
		MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
		for( long k = 0; k < m_to_combine; k++ )
			ret->Add( at( combo_storage[ k ] ) );
		return ret;
	}

	m_to_combine = 0;
	combo_storage.Flush();
	return 0;
}

MMD_Set* MMD_Set::GetComboA_First( unsigned m )
{
	if( !getCount() || ( m > getCount() ) || !m )
		return 0;

	m_to_combine = m;
	combo_storage.Flush();
	combo_no_storage.Flush();

	MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
	for( long i = 0; i < (long) m; i++ )
	{
		combo_storage.Add( i );
		combo_no_storage.Add( i );
		ret->Add( at( i ) );
	}
	return ret;
}

MMD_Set* MMD_Set::GetComboA_Next( void )
{
	if( !getCount() || ( m_to_combine > (long) getCount() || !m_to_combine ) )
		return 0;

	long i, j;

	if( (combo_storage.ArrayDim() != m_to_combine) ||
		(combo_no_storage.ArrayDim() != m_to_combine) )
		return 0;

	if( next_permutation( combo_no_storage.begin(), combo_no_storage.end() ) )
	{	// new permutation
		MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
		for( long k = 0; k < m_to_combine; k++ )
			ret->Add( at( combo_storage[ combo_no_storage[ k ] ] ) );
		return ret;
	}
	else
	{	// new C(n, m) combination generates
		if( combo_storage[ m_to_combine - 1 ] + 1 == (long) getCount() )
		{
			for ( i = m_to_combine - 1; i > 0; i-- )
				if( combo_storage[ i - 1 ] + 1 != combo_storage[ i ] )
				{
					combo_storage[ i - 1 ]++;
					for ( j = i; j < m_to_combine; j++ )
						combo_storage[ j ] = combo_storage[ j - 1 ] + 1;

					MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
					for( long k = 0; k < m_to_combine; k++ )
						ret->Add( at( combo_storage[ combo_no_storage[ k ] ] ) );
					return ret;
				}
		}
		else
		{
			combo_storage[ m_to_combine - 1 ]++;

			MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
			for( long k = 0; k < m_to_combine; k++ )
				ret->Add( at( combo_storage[ combo_no_storage[ k ] ] ) );
			return ret;
		}

		m_to_combine = 0;
		combo_storage.Flush();
		combo_no_storage.Flush();
		return 0;
	}
}

MMD_Set* MMD_Set::GetComboP_First( void )
{
	if( !getCount() )
		return 0;

	combo_no_storage.Flush();

	MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
	for( long i = 0; i < (long) getCount(); i++ )
	{
		combo_no_storage.Add( i );
		ret->Add( at( i ) );
	}
	return ret;
}

MMD_Set* MMD_Set::GetComboP_Next( void )
{
	if( !getCount() )
		return 0;

	if( next_permutation( combo_no_storage.begin(), combo_no_storage.end() ) )
	{
		MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
		for( long k = 0; k < (long) getCount(); k++ )
			ret->Add( at( combo_no_storage[ k ] ) );
		return ret;
	}
	else
	{
		m_to_combine = 0;
		combo_storage.Flush();
		combo_no_storage.Flush();
		return 0;
	}
}

MMD_Set* MMD_Set::GetComboARet_First( unsigned m )
{
	if( !getCount() || ( m > getCount() ) || !m )
		return 0;

	m_to_combine = m;
	combo_storage.Flush();

	MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
	long _null = 0;
	for( long i = 0; i < (long) m; i++ )
	{
		combo_storage.Add( _null );
		ret->Add( at( 0 ) );
	}
	return ret;
}

MMD_Set* MMD_Set::GetComboARet_Next( void )
{
	if( !getCount() || ( m_to_combine > (long) getCount() ) || !m_to_combine )
		return 0;

	long i;
	for( i = 0; i < m_to_combine; i++ )
	{
		if( combo_storage[ i ] == (long) getCount() - 1 )
		{
			combo_storage[ i ] = 0;
			continue;
		}
		else
		{
			combo_storage[ i ]++;
			MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
			for( long k = 0; k < m_to_combine; k++ )
				ret->Add( at( combo_storage[ k ] ) );
			return ret;
		}
	}

	m_to_combine = 0;
	return 0;
}

MMD_Set* MMD_Set::GetComboCRet_First( unsigned m )
{
	if( !getCount() || ( m > getCount() ) || !m )
		return 0;

	m_to_combine = m;
	combo_storage.Flush();

	MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
	long nullint = 0;
	for( long i = 0; i < (long) m; i++ )
	{
		combo_storage.Add( nullint );
		ret->Add( at( 0 ) );
	}
	return ret;
}

MMD_Set* MMD_Set::GetComboCRet_Next( void )
{
	if( !getCount() || ( m_to_combine > (long) getCount() ) || !m_to_combine )
		return 0;

	long i, j;
	for( i = 0; i < m_to_combine; i++ )
	{
		if( combo_storage[ i ] == (long) getCount() - 1 )
			continue;
		else
		{
			combo_storage[ i ]++;
			if( i )
				for( j = i - 1; j >= 0; j-- )
					combo_storage[ j ] = combo_storage[ i ];

			MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
			for( long k = 0; k < m_to_combine; k++ )
				ret->Add( at( combo_storage[ k ] ) );
			return ret;
		}
	}

	m_to_combine = 0;
	return 0;
}

MMD_Set* MMD_Set::GetSubSet( CDSRBitsStorage& det )
{
	MMD_Set* ret = getSmbTable().makeUniWord( new MMD_Set( getSmbTable() ) );
	for( long i = 0; i < (long) getCount(); i++ )
		if( bit( det, i ) )
			ret->Add( at( i ) );
	return ret;
}

///////////////////////////////////////////////////////////////////////////////
// SETADD
///////////////////////////////////////////////////////////////////////////////
MMD_Set* _dsrop_SetAdd( MMD_Set *a, MMD_Set *b )
{
	MMD_Set* sum = a->getSmbTable().makeUniWord( new MMD_Set( *a ) );
	for( long i = 0; i < (long) b->getCount(); i++ )
		sum->Add( b->at( i ) );
	return sum;
}

MMD_Set* _dsrop_SetMul( MMD_Set *vset1, MMD_Set *vset2 )
{
	MMD_Set* sum = vset1->getSmbTable().makeUniWord( new MMD_Set( vset1->getSmbTable() ) );
	MMD_Set *tmp;

	// Count(vset1) must be greater than Count(vset2)
	if( vset1->getCount() < vset2->getCount() )
	{
		tmp = vset1;
		vset1 = vset2;
		vset2 = tmp;
	}
	for( long i = 0; i < (long) vset2->getCount(); i++ )
	{
		UniWord uw( CDSRInteger( 0 ) );
		if( _dsrop_SetRelIn( uw, vset1, vset2->at( i ) ).getInteger() )
			sum->Add( vset2->at( i ) );
	}
	return sum;
}

MMD_Set* _dsrop_SetSub( MMD_Set *vset1, MMD_Set *vset2 )
{
	MMD_Set* sum = vset1->getSmbTable().makeUniWord( new MMD_Set( vset1->getSmbTable() ) );
	for( long i = 0; i < (long) vset1->getCount(); i++ )
	{
		UniWord uw( CDSRInteger( 0 ) );
		if( !_dsrop_SetRelIn( uw, vset2, vset1->at( i ) ).getInteger() )
			sum->Add( vset1->at( i ) );
	}
	return sum;
}

MMD_Set* _dsrop_SetDecartMul( MMD_Set *vset1, MMD_Set *vset2 )
{
	long i, j;
	MMD_Set* ms = vset1->getSmbTable().makeUniWord( new MMD_Set( vset1->getSmbTable() ) );
	for( i = 0; i < (long) vset1->getCount(); i++ )
	{
		for( j = 0; j < (long) vset2->getCount(); j++ )
		{
			MMD_Set* ms_item = vset1->getSmbTable().makeUniWord( new MMD_Set( vset1->getSmbTable() ) );
			ms_item->Add( vset1->at( i ) );
			ms_item->Add( vset2->at( j ) );
			UniWord uw( ms_item );
			ms->Add( uw );
		}
	}
	return ms;
}

UniWord _dsrop_SetRelIn( UniWord& ret, MMD_Set *vset, UniWord& word )
{
	ret = UniWord( CDSRInteger( vset->Find( word ) == INT_MAX ? 0 : 1 ) );
	return ret;
}

UniWord _dsrop_SetRelNotIn( UniWord& ret, MMD_Set *vset, UniWord& word )
{
	_dsrop_SetRelIn( ret, vset, word );
	_dsrop_BoolNot_bool( ret, ret );
	return ret;
}

UniWord _dsrop_SetRelSetIn( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 )
{
	if( vset2->getCount() <= vset1->getCount() )
	{
		ret = UniWord( CDSRInteger( 0 ) );
		return ret;
	}
	for( long i = 0; i < (long) vset1->getCount(); i++ )	
	{
		UniWord uw( CDSRInteger( 0 ) );
		if( !_dsrop_SetRelIn( uw, vset2, vset1->at( i ) ).getInteger() )
		{
			ret = UniWord( CDSRInteger( 0 ) );
			return ret;
		}
	}
	ret = UniWord( CDSRInteger( 1 ) );
	return ret;
}

UniWord _dsrop_SetRelSetNotIn( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 )
{
	_dsrop_SetRelSetIn( ret, vset1, vset2 );
	_dsrop_BoolNot_bool( ret, ret );
	return ret;
}

UniWord _dsrop_SetRelSetInInv( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 )
{
	return _dsrop_SetRelSetIn( ret, vset2, vset1 );
}

UniWord _dsrop_SetRelSetInEq( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 )
{
	if( vset2->getCount() < vset1->getCount() )
	{
		ret = UniWord( CDSRInteger( 0 ) );
		return ret;
	}
	for( long i = 0; i < (long) vset1->getCount(); i++ )	
	{
		UniWord uw( CDSRInteger( 0 ) );
		if( !_dsrop_SetRelIn( uw, vset2, vset1->at( i ) ).getInteger() )
		{
			ret = UniWord( CDSRInteger( 0 ) );
			return ret;
		}
	}
	ret = UniWord( CDSRInteger( 1 ) );
	return ret;
}

UniWord _dsrop_SetRelSetInEqInv( UniWord& ret, MMD_Set *vset1, MMD_Set *vset2 )
{
	return _dsrop_SetRelSetInEq( ret, vset2, vset1 );
}

UniWord _dsrop_SetEqual_anytype( MMD_Set *a, MMD_Set *b )
{
	UniWord ret;
	if( a->getCount() != b->getCount() )
		return UniWord( CDSRInteger( 0 ) );
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_SetRelNotIn( ret, a, (*b)[ i ] ).getInteger() )
			return UniWord( CDSRInteger( 0 ) );
	return UniWord( CDSRInteger( 1 ) );
}

UniWord _dsrop_SetNotEqual_anytype( MMD_Set *a, MMD_Set *b )
{
	UniWord ret;
	if( a->getCount() != b->getCount() )
		return UniWord( CDSRInteger( 1 ) );
	for( long i = 0; i < (long) a->getCount(); i++ )
		if( _dsrop_SetRelNotIn( ret, a, (*b)[ i ] ).getInteger() )
			return UniWord( CDSRInteger( 1 ) );
	return UniWord( CDSRInteger( 0 ) );
}

///////////////////////////////////////////////////////////////////////////////
