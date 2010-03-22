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

#include "../uniword/uniword_op.h"
#include "mmd_group.h"

///////////////////////////////////////////////////////////////////////////////


MMD_GroupString::MMD_GroupString( void ) :
	MArray<long>( 64, 0, 64 )
{}


MMD_GroupString::MMD_GroupString( const MMD_GroupString& ms ) :
	MArray<long>( 64, 0, 64 )
{
	copy( &ms );
}


MMD_GroupString* MMD_GroupString::copy( const MMD_GroupString *src )
{
	MArray<long>::Copy( *src );
	return this;
}


MMD_GroupString::~MMD_GroupString( void )
{
}

///////////////////////////////////////////////////////////////////////////////


MMD_GroupStringArray::MMD_GroupStringArray( void ) :
	MArray<MMD_GroupString>( 64, 0, 64 )
{}


MMD_GroupStringArray::MMD_GroupStringArray( const MMD_GroupStringArray& ms ) :
	MArray<MMD_GroupString>( 64, 0, 64 )
{
	copy( &ms );
}


MMD_GroupStringArray* MMD_GroupStringArray::copy( const MMD_GroupStringArray *src )
{
	MArray<MMD_GroupString>::Copy( *src );
	return this;
}


MMD_GroupStringArray::~MMD_GroupStringArray( void )
{
}

///////////////////////////////////////////////////////////////////////////////

MMD_Group::MMD_Group( CParseDsrSymbolTable& _smbtable, long power, QString* names, long **arg_table ) :
	MMD_Object( _smbtable, DSRDATA_TYPE_ABS_GROUP ),
	str_id( power ), table( arg_table, power, power ), subgr_int(8, 0, 8)
{
	for( long i = 0; i < power; i++ )
	{
		if( !names[ i ].length() )
			throw "Internal error while group constructing";
		str_id.Add( new QString( names[ i ] ) );
	}
	grp_power = power;
	ready = GRT_Ready;
	ConstructGetReady();
}

MMD_Group::MMD_Group( CParseDsrSymbolTable& _smbtable, long power, QString* names, MMD_GroupMulTable& arg_table ) :
	MMD_Object( _smbtable, DSRDATA_TYPE_ABS_GROUP ),
	str_id( power ), table( arg_table ), subgr_int(8, 0, 8)
{
	for( long i = 0; i < power; i++ )
	{
		if( !names[ i ].length() )
			throw "Internal error while group constructing";
		str_id.Add( new QString( names[ i ] ) );
	}
	grp_power = power;
	ready = GRT_Ready;
	ConstructGetReady();
}


long MMD_Group::StartSubGroupList( void )
{
	subgrp_det = (BN_TYPE)3;
	subgr_int.Flush();
	return 0;
}

long MMD_Group::StepSubGroupList( void )
{
	MMD_Group* mg;
	CDSRBits max_sub_det = ((CBigNumber(1) << GetPower()) - 1);
	if( subgrp_det >= max_sub_det )
		return -1;

	long i, j, k, cb;
	CDSRBitsStorage p_sub_det = &subgrp_det;
	if( (mg = GetSubGroup( p_sub_det )) == 0 )
	{
		subgrp_det +=2;
		return 0;
	}

	k = subgr_int.Add( new long[ GetPower() * GetPower() + 1 ] );
	(subgr_int[ k ])[ 0 ] = mg->GetPower();

	cb = 1;
	for( i = 0; i < GetPower(); i++ )
		for( j = 0; j < GetPower(); j++ )
		{
			if( bit( &subgrp_det, i ) && bit( &subgrp_det, j ) )
				subgr_int[ k ][ cb++ ] = 1;
			else
				subgr_int[ k ][ cb++ ] = 0;
		}

	subgrp_det +=2;
	return 1;
}

MIArray<long>& MMD_Group::GetSubGroupList( void )
{
	return subgr_int;
}

MIArray<long>& MMD_Group::FillSubGroupList( void )
{
	subgr_int.Flush();

	MMD_Group* mg;
	CDSRBits sub_det, max_sub_det = ((CBigNumber(1) << GetPower()) - 1);
	long i, j, k, cb;
	for( sub_det = (BN_TYPE)3; sub_det < max_sub_det; sub_det +=2 )
	{
		CDSRBitsStorage p_sub_det = &sub_det;
		if( (mg = GetSubGroup( p_sub_det )) == 0 ) continue;

		k = subgr_int.Add( new long[ GetPower() * GetPower() + 1 ] );
		(subgr_int[ k ])[ 0 ] = mg->GetPower();

		cb = 1;
		for( i = 0; i < GetPower(); i++ )
			for( j = 0; j < GetPower(); j++ )
			{
				if( bit( &subgrp_det, i ) && bit( &subgrp_det, j ) )
					subgr_int[ k ][ cb++ ] = 1;
				else
					subgr_int[ k ][ cb++ ] = 0;
			}
	}
	return subgr_int;
}

long MMD_Group::EvalString2Index( const QString& names )
{ // evaluate QString of format "aa * bb * cc ..." or "aa + bb + cc ..."
	QStringList res = names.split( QString(" * "), QString::SkipEmptyParts );
	res += names.split( QString(" + "), QString::SkipEmptyParts );
	return EvalString2Index( res );
}

long MMD_Group::EvalString2Index( const QStringList& names )
{
	if(GetReady() != GRT_Ready || names.size() < 2)
		return INT_MAX;
	long a, b;
	a = GetIndex( names.at(0) );
	b = GetIndex( names.at(1) );
	if( (a == INT_MAX) || (b == INT_MAX) )
		return INT_MAX;
	int i = 2;
	do
	{
		a = table( a, b );
		if( i >= names.size() ) return a;
		b = GetIndex( names.at( i++ ) );
	}
	while( i < names.size() );
	return INT_MAX;
}

int MMD_Group::IsNotValid( MMD_GroupMulTable& _table, long _grp_power )
{
	long i, j, item;

	long **prow = new long*[ _grp_power ];
	long **pcolumn = new long*[ _grp_power ];

	for( i = 0; i < _grp_power; i++ )
	{
		prow[ i ] = new long[ _grp_power ];
		for( j = 0; j < _grp_power; j++ )
			prow[ i ][ j ] = 0;
		pcolumn[ i ] = new long[ _grp_power ];
		for( j = 0; j < _grp_power; j++ )
			pcolumn[ i ][ j ] = 0;
	}

	for( i = 0; i < _grp_power; i++ )
	{
		for( j = 0; j < _grp_power; j++ )
		{
			if( (item = _table( i, j )) >= _grp_power )
				return (int)MMD_Group::Range;
			if( prow[ i ][ item ] || pcolumn[ j ][ item ] )
				return (int)MMD_Group::NotUnique;
			prow[ i ][ item ] = 1;
			pcolumn[ j ][ item ] = 1;
		}
	}
			
	for( i = 0; i < _grp_power; i++ )
	{
		delete[] prow[ i ];
		delete[] pcolumn[ i ];
	}
	delete[] prow;
	delete[] pcolumn;

	for( i = 0; i < _grp_power; i++ )
	{
		if( (_table( 0, i ) != i) || (_table( i, 0 ) != i) )
			return (int)MMD_Group::NotNeutral;
	}

	return (int)MMD_Group::ValidGroup;
}

MMD_Group* MMD_Group::GetSubGroup( CDSRBitsStorage& det )
{
	if( !bit( det, 0 ) )
		return 0;

	long *items = new long[ GetPower() ];
	long *weights = new long[ GetPower() ];

	long i, j, de, weight = 0;

	for( i = 0; i < GetPower(); i++ )
	{
		if( bit( det, i ) )
		{
			weights[ weight ] = i;
			items[ i ] = weight++;
		}
		else
			items[ i ] = -1;
	}
	long **new_table = new long*[ weight ];
	for( i = 0; i < weight; i++ )
		new_table[ i ] = new long[ weight ];

	for( i = 0; i < weight; i++ )
		for( j = 0; j < weight; j++ )
		{
			de = table( weights[ i ], weights[ j ] );
			if( items[ de ] == -1 )
			{
				for( i = 0; i < weight; i++ )
					delete[] new_table[ i ];
				delete[] new_table;
				delete[] items;
				delete[] weights;
				return 0;
			}
			else
				new_table[ i ][ j ] = items[ de ];
		}

	MMD_GroupMulTable tblV( new_table, weight, weight );
	if( IsNotValid( tblV, weight ) )
	{
		for( i = 0; i < weight; i++ )
			delete[] new_table[ i ];
		delete[] new_table;
		delete[] items;
		delete[] weights;
		return 0;
	}

	QString *names = new QString[ weight ];
	for( i = 0; i < weight; i++ )
		names[ i ] = *(str_id[ weights[ i ] ]);

	delete[] items;
	delete[] weights;

	MMD_Group *ret = new MMD_Group( getSmbTable(), weight, names, new_table );
	for( i = 0; i < weight; i++ )
		delete[] new_table[ i ];
	delete[] new_table;
	delete[] names;
	return ret;
}

//==========================================================================
// Params:
//   det - subgroup bits,
//   chosen - result gi*H bits,
//   left - is left mul
// Returns:
//   term's quantity (0 on error)
//==========================================================================
long MMD_Group::Factorization( CDSRBitsStorage& det, long *chosen, bool left )
//                 ~~~~~~~~~~~~~
{
	if( !bit( det, 0 ) )
		return 0;

	long i, j, index, weight = 0;
	long *items = new long[ GetPower() ];
	long *weights = new long[ GetPower() ];

	for( i = 0; i < GetPower(); i++ )
		chosen[ i ] = 0;

	for( i = 0; i < GetPower(); i++ )
	{
		if( bit( det, i ) )
		{
			items[ i ] = 1;
			weights[ weight ] = i;
			weight++;
		}
		else
			items[ i ] = 0;
	}
/*
	if( weight < 2 )
	{
		delete[] items;
		delete[] weights;
		return 0;
	}
*/
	long cb = 0;
	for( i = 1; i < GetPower(); i++ )
	{
		if( !items[ i ] )
		{
			cb++;
			items[ i ] = chosen[ i ] = cb/*1*/;	// chosen <-> gi*H
			for( j = 0; j < weight; j++ )
			{
				index = (left ? GetIndex2Index( i, weights[ j ] ) :
								GetIndex2Index( weights[ j ], i ));
				items[ index ] = chosen[ index ] = cb/*1*/;
			}
		}
	}
	delete[] items;
	delete[] weights;
	return cb + 1;
}


long MMD_Group::GetInv( long index )
{
	if( index == 0 )
		return index;
	for( long i = 1; i < GetPower(); i++ )
		if( GetTable( index, i ) == 0 )
			return i;
	return INT_MAX; // internal error: no inv. item
}


int MMD_Group::IsNormalSubGroup( CDSRBitsStorage& det )
{
	if( !bit( det, 0 ) )
		return 0;

	long i, j, weight = 0;
	long *items = new long[ GetPower() ];
	long *weights = new long[ GetPower() ];

	for( i = 0; i < GetPower(); i++ )
	{
		if( bit( det, i ) )
		{
			items[ i ] = 1;
			weights[ weight ] = i;
			weight++;
		}
		else
			items[ i ] = 0;
	}

	if( weight < 2 )
	{
		delete[] items;
		delete[] weights;
		return 0;
	}

	long cb, index;
	long *tst = new long[ GetPower() ];
	long *tst_1 = new long[ GetPower() ];
	for( i = 1; i < GetPower(); i++ )
	{
		if( !items[ i ] )
		{
			cb = 0;
			for( j = 0; j < weight; j++ )
				tst[ j ] = tst_1[ j ] = 0;

			for( j = 0; j < weight; j++ ) // left mul.
			{
				index = GetIndex2Index( i, weights[ j ] );
				if( !tst[ index ] )
				{
					tst[ index ] = 1;
					cb++;
				}
			}

			for( j = 0; j < weight; j++ ) // right mul. and check
			{
				index = GetIndex2Index( weights[ j ], i );
				if( !tst[ index ] )
				{
					delete[] items;
					delete[] weights;
					delete[] tst;
					delete[] tst_1;
					return 0;
				}
				else
				{
					if( !tst_1[ index ] )
					{
						tst_1[ index ] = 1;
						cb--;
					}
				}
			}
			if( cb )
			{
				delete items;
				delete weights;
				delete tst;
				delete tst_1;
				return 0;
			}
		}
	}
	delete items;
	delete weights;
	delete tst;
	delete tst_1;
	return 1;
}


MMD_Group* MMD_Group::GetFactorGroup( CDSRBitsStorage& det, bool check_normal )
//                            ~~~~~~~~~~~~~~
{
	if( check_normal )
		if( !IsNormalSubGroup( det ) ) return NULL;

	if( !bit( det, 0 ) )
		return 0;

	long i, j, index, weight = 0;
	long *items = new long[ GetPower() ];
	long *weights = new long[ GetPower() ];

	for( i = 0; i < GetPower(); i++ )
	{
		if( bit( det, i ) )
		{
			items[ i ] = 1;
			weights[ weight ] = i;
			weight++;
		}
		else
			items[ i ] = 0;
	}

	if( weight < 2 )
	{
		delete[] items;
		delete[] weights;
		return 0;
	}

	long factor_grp_power = GetPower() / weight;
	long *deleg = new long[ factor_grp_power ];
	deleg[ 0 ] = 0;

	long *chosen = new long[ GetPower() ];
	for( i = 0; i < GetPower(); i++ )
		chosen[ i ] = 0;

	long cb = 1;
	
	for( i = 1; i < GetPower(); i++ )
	{
		if( !items[ i ] )
		{
			deleg[ cb ] = i;
			items[ i ] = chosen[ i ] = cb;	// chosen <-> gi*H
			for( j = 0; j < weight; j++ )
			{
				index = GetIndex2Index( i, weights[ j ] );
				items[ index ] = chosen[ index ] = cb;
			}
			cb++;
		}
	}
	// must be (cb == factor_grp_power)
	MMD_GroupMulTable *factor_grp_table = new MMD_GroupMulTable( factor_grp_power, factor_grp_power );
	for( i = 0; i < factor_grp_power; i++ )
		for( j = 0; j < factor_grp_power; j++ )
			(*factor_grp_table)( i, j ) = chosen[ GetTable( deleg[ i ], deleg[ j ] ) ];

	QString *factor_grp_names = new QString[ factor_grp_power ];
	for( i = 0; i < factor_grp_power; i++ )
	{
		factor_grp_names[ i ] = _T("{ ");
		for( j = 0; j < GetPower(); j++ )
		{
			if( chosen[ j ] == i )
			{
				factor_grp_names[ i ] += *( str_id[ j ] ) + _T(", ");
			}
		}
		size_t k = factor_grp_names[ i ].length();
                factor_grp_names[ i ].replace( k - 2, 1, ' ');
                factor_grp_names[ i ].replace( k - 1, 1, '}');
	}

	MMD_Group *ret = new MMD_Group( getSmbTable(), factor_grp_power, factor_grp_names, *factor_grp_table );
	delete[] items;
	delete[] weights;
	delete[] chosen;
	delete[] deleg;
	delete factor_grp_table;
	delete[] factor_grp_names;
	return ret;
}


//==========================================================================
//==========================================================================
bool MMD_Group::CheckPod( long /*pod_count*/, long pod_power, long i, long j, MArray<long>& pod )
{
	long _j;
	for( _j = 0; _j < pod_power; _j++ )
		if( pod[ i * pod_power + _j ] != pod[ j * pod_power + _j ] )
			break;
	return ( _j == pod_power ) ? false : true;
}

//==========================================================================
//==========================================================================
long MMD_Group::FindPod( long pod_count, long pod_power, MArray<long>& pod, long *new_pod )
{
	long i, j, index;
	index = INT_MAX;
	for( i = 0; i < pod_count; i++ )
	{
		for( j = 0; j < pod_power; j++ )
		{
			if( pod[ i * pod_power + j ] != new_pod[ j ] )
				break;
		}
		if( j == pod_power )
		{
			index = i;
			break;
		}
	}
	return index;
}

void MMD_Group::GetReady2GenerateGroup( long items_c, QString* items, long rules_c, QString* left, QString* right )
{
	inf_genes.items_c = items_c;
	inf_genes.rules_c = rules_c;
	long i;
	inf_genes.items = new QString[ items_c ];
	inf_genes.left = new QString[ rules_c ];
	inf_genes.right = new QString[ rules_c ];
	for( i = 0; i < items_c; i++ )
		inf_genes.items[ i ] = items[ i ];
	for( i = 0; i < rules_c; i++ )
	{
		inf_genes.left[ i ] = left[ i ];
		inf_genes.right[ i ] = right[ i ];
	}
	ready = GRT_Ready2Genes;
}

bool MMD_Group::StartGroupGeneration( void )
{
	bool res = false;
	if(GetReady() == GRT_Ready2Genes)
		res = GenerateGroup( inf_genes.items_c, inf_genes.items, inf_genes.rules_c, inf_genes.left, inf_genes.right );
	else if(GetReady() == GRT_Ready2Pods)
	{
		MArray<long> _pod( inf_pods.pod_count * inf_pods.pod_power, 0, 4 * inf_pods.pod_power );
		_pod.resize(inf_pods.pod.end() - inf_pods.pod.begin());
		std::copy(inf_pods.pod.begin(), inf_pods.pod.end(), _pod.begin());
		res = GenerateGroup( inf_pods.pod_count, inf_pods.pod_power, _pod );
	}
	if( res )
		ready = GRT_Ready;
	return res;
}

void MMD_Group::GetReady2GenerateGroup( long pod_count, long pod_power, MArray<long>& pod )
{
	inf_pods.pod_count = pod_count;
	inf_pods.pod_power = pod_power;
	inf_pods.pod.resize(pod.ArrayDim());
	std::copy(pod.begin(), pod.end(), inf_pods.pod.begin());
	ready = GRT_Ready2Pods;
}

//==========================================================================
// Params:
//   items_c - genes quantity,
//   pod - pods.
// Returns:
//   true - ok, otherwise - false
//==========================================================================
bool MMD_Group::GenerateGroup( long pod_count, long pod_power, MArray<long>& pod )
{
	long i, j, index;
	
	str_id.Flush();
	if( table.n_row() || table.n_column() )
		table.erase_all_column();

	index = INT_MAX;
	for( i = 0; i < pod_count; i++ )
	{
		for( j = 0; j < pod_power; j++ )
		{
			if( pod[ i * pod_power + j ] != j + 1 )
				break;
		}
		if( j == pod_power )
		{
			index = i;
			break;
		}
	}
	if( index == INT_MAX )
	{
		str_id.Flush();
		table.erase_all_column();
		return false;
	}
	else
	{
		if( index )
			for( i = 0; i < pod_power; i++ )
			{
				j = pod[ i ];
				pod[ i ] = i + 1;
				pod[ index * pod_power + i ] = j;
			}
	}

	grp_power = pod_count;
	long items_c = pod_count;

	//QString temp_str( commonGetNeutralElementName() );
	QString temp_str( QString( commonGetGroupPodPrefix() ) + _T("1") );
	table.add_row();
	table.add_column();
	str_id.Add( new QString( temp_str ) );
	temp_str = commonGetGroupPodPrefix();
	for( i = 1; i < pod_count; i++ )
	{
		table.add_row();
		table.add_column();
		str_id.Add( new QString( temp_str + QString("%1").arg(i + 1) ) );
	}

	unsigned long steps = 0, gpow = 1;
	long new_1st = 0, new_items_c;
	long *new_pod = new long[ pod_power ];
	while( !StopGenerating( steps, gpow ) )
	{
		new_items_c = 0;
		for( i = 0; i < items_c + new_1st; i++ )
		{
			for( j = 0; j < items_c + new_1st; j++ )
			{
				if( (i < new_1st) && (j < new_1st) )
					continue;
				if( (i > j) && !CheckPod( pod_count, pod_power, i, j, pod ) )
				{
					str_id.Flush();
					table.erase_all_column();
					delete[] new_pod;
					return false;
				}
				long k;
				for( k = 0; k < pod_power; k++ )
					new_pod[ k ] = pod[ j * pod_power + (pod[ i * pod_power + k ] - 1) ];
				if( (index = FindPod( pod_count, pod_power, pod, new_pod )) != INT_MAX )
					table( i, j ) = index;
				else
				{
					QString *str = new QString();
					*str = QString( temp_str + "%1" ).arg( grp_power + 1 );
					str_id.Add( str );
					table.add_row();
					table.add_column();
					table( i, j ) = grp_power++;
					new_items_c++;
					for( k = 0; k < pod_power; k++ )
						pod[ (grp_power - 1) * pod_power + k ] = new_pod[ k ];
					pod_count++;
				}
			}
		}
		if( new_items_c )
		{
			steps += items_c;
			gpow++;
			new_1st += items_c;
			items_c = new_items_c;
			continue;
		}

		delete[] new_pod;
		// Group ok!
		for( i = 0; i < grp_power; i++ )
			table( i, 0 ) = table( 0, i ) = i;

		if( IsNotValid( GetTable(), GetPower() ) )
		{
			str_id.Flush();
			table.erase_all_column();
			return false;
		}

		return true;
	}

	str_id.Flush();
	table.erase_all_column();
	return false;
}


//==========================================================================
// Params:
//   items_c - genes quantity,
//   items - genes' names,
//   rules_c - rules quantity,
//   left & right - QString pairs like "a * a * a" & "E"
// Returns:
//   true - ok, otherwise - false
//==========================================================================
bool MMD_Group::GenerateGroup( long items_c, QString* items, long rules_c, 
												QString* left, QString* right )
{
	MMD_GroupStringArray* ptr2_tmp_left_idx = new MMD_GroupStringArray();
	MMD_GroupStringArray* ptr2_tmp_right_idx = new MMD_GroupStringArray();
	
	MMD_GroupStringArray& tmp_left_idx = *ptr2_tmp_left_idx;
	MMD_GroupStringArray& tmp_right_idx = *ptr2_tmp_right_idx;

	if( !IsValidGenes( items_c, rules_c, left, right, items, tmp_left_idx, tmp_right_idx ) )
		return false;

	DeduceNewGenes( tmp_left_idx, tmp_right_idx );
	rules_c = (long) tmp_left_idx.ArrayDim();

	long i, j;
	
	str_id.Flush();
	if( table.n_row() || table.n_column() )
		table.erase_all_column();

	MArray<long> mem_left( 256, 0, 64 );
	MArray<long> mem_right( 256, 0, 64 );
	str_id_int_vector str_id_int/*( 256, 0, 64 )*/;
	long int_max = INT_MAX;
	
	QString temp_str( commonGetNeutralElementName() );
	table.add_row();
	table.add_column();
	str_id.Add( new QString( temp_str ) );
	str_id_int.push_back( std::list<long>(1, long(0)) );
	mem_left.Add( int_max );
	mem_right.Add( int_max );
	for( i = 0; i < items_c; i++ )
	{
		table.add_row();
		table.add_column();
		str_id.Add( new QString( items[ i ] ) );
		str_id_int.push_back( std::list<long>(1, long(i + 1)) );
		mem_left.Add( int_max );
		mem_right.Add( int_max );
	}
	long basic_grp_power = grp_power = items_c + 1;

#if 0
	// debug
	QString str = _T("");
	for( long h = 0; h < rules_c; h++ )
	{
		long r;
		for( r = 0; r < tmp_left_idx[ h ].ArrayDim(); r++ )
			str += *(str_id[ tmp_left_idx[ h ][ r ] ]) + " ");
		str += _T("=");
		for( r = 0; r < tmp_right_idx[ h ].ArrayDim(); r++ )
			str += _T(" " + *(str_id[ tmp_right_idx[ h ][ r ] ]);
		str += _T("\n");
	}
	// end debug
#endif

	unsigned long steps = 0, gpow = 1;
	long new_1st = 1, index, new_items_c;
	std::list<long> to_treat;
	while( !StopGenerating( steps, gpow ) )
	{
		new_items_c = 0;
		for( i = 1/*new_1st*/; i < items_c + new_1st; i++ )
		{
			for( j = 1/*new_1st*/; j < items_c + new_1st; j++ )
			{
				if( (i < new_1st) && (j < new_1st) )
					continue;
				temp_str = *(str_id[ i ]) + QString( _T(" ") ) + QString(commonGetGroupMultiplicationSign()) + QString( _T(" ") ) + *(str_id[ j ]);
				if( (index = str_id.Find( &temp_str )) != INT_MAX )
					table( i, j ) = index;
				else
				{
					to_treat.erase( to_treat.begin(), to_treat.end() );
					if( i < basic_grp_power )
						to_treat.insert(to_treat.end(), i);
					else
						Unroll( i, to_treat.end(), to_treat, mem_left, mem_right );
					if( j < basic_grp_power )
						to_treat.insert(to_treat.end(), j);
					else
						Unroll( j, to_treat.end(), to_treat, mem_left, mem_right );
					if( (index = IdentifyFactor( to_treat, rules_c, tmp_left_idx, tmp_right_idx, str_id_int )) == INT_MAX )
					{
						table.add_row();
						table.add_column();
						str_id.Add( new QString( temp_str ) );
						str_id_int.push_back( to_treat );
						mem_left.Add( i );
						mem_right.Add( j );
						table( i, j ) = grp_power++;
						new_items_c++;
					}
					else
						table( i, j ) = index;
				}
			}
		}
		if( new_items_c )
		{
			steps += items_c;
			gpow++;
			new_1st += items_c;
			items_c = new_items_c;
			continue;
		}

		// Group ok!
		for( i = 0; i < grp_power; i++ )
			table( i, 0 ) = table( 0, i ) = i;

		if( IsNotValid( GetTable(), GetPower() ) )
			return false;

		delete ptr2_tmp_left_idx;
		delete ptr2_tmp_right_idx;
		return true;
	}

	delete ptr2_tmp_left_idx;
	delete ptr2_tmp_right_idx;
	return false;
}

//========================================================================================
struct GRPHystoryList
{
	long				rule;		// number of rule
	bool				direction;	// if true - left (:=, normal) <- right, left -> right
	std::vector<long>*		lock;		// 0 - raw object, 1 - direct rule, 2 - reverse rule
	std::list<long>*			to_treat;	// QString to treat
	long				del_1st;	// first item to be delete

	GRPHystoryList( void )
	{
		to_treat = 0;
		lock = 0;
	}
	GRPHystoryList( const GRPHystoryList& h )
	{
		rule = h.rule;
		direction = h.direction;
		del_1st = h.del_1st;
		to_treat = new std::list<long>(*(h.to_treat));
		lock = new std::vector<long>(*(h.lock));
	}
	~GRPHystoryList()
	{
		if( to_treat )
		{
			delete to_treat;
			to_treat = 0;
		}
		if( lock )
		{
			delete lock;
			lock = 0;
		}
	}
};

long MMD_Group::IdentifyFactor( std::list<long>& in_to_treat, long rules_c, MMD_GroupStringArray& left, MMD_GroupStringArray& right, str_id_int_vector& str_id_int )
{
	long i, k, i2;
	bool ok/*, start = true*/;
	GRPHystoryList	fact;

	std::list<long>::iterator location;
	std::list<GRPHystoryList>::iterator hystory_location;
	unsigned long j2 = 0;

	std::list<long> in_to_treat_copy( in_to_treat );
	std::list<long>* to_treat = &(in_to_treat_copy);
	std::vector<long> to_lock_ref( rules_c, long(0) );
	std::vector<long>* to_lock = &(to_lock_ref);
	std::list<GRPHystoryList>* hystory = new std::list<GRPHystoryList>;
	long hystory_count = 0, hystory_index = 0, hystory_local_count = 0;
	std::list<GRPHystoryList>::iterator in_fact = hystory->begin();
	std::list<GRPHystoryList>::iterator after_in_fact = hystory->end();
	
	for(;;)
	{
		hystory_local_count = 0;
		for( i = 0; i < rules_c; i++ )
		{
			bool direction = false;
			i2 = 0;
			while( i2 < 1/*2*/ )
			{
				i2++;
				direction = !direction;

				if( (direction && ((*to_lock)[ i ] == 2)) ||
					(!direction && ((*to_lock)[ i ] == 1)) )
					break;
				MArray<long>& from_part = direction ? left[ i ] : right[ i ];

				ok = false;
//				for( j = 0; j < from_part.ArrayDim(); j++ )
//				{
					long lev = 0;
					std::list<long>::iterator it = to_treat->begin();
					while((it != to_treat->end()) && (lev < (long) to_treat->size()))
					{
						if( *it == from_part[ 0 ] )
						{
							std::list<long>::iterator tmp_it = it;
							long lleevv = lev;
							tmp_it++;
							lleevv++;
							ok = true;
							for( long tmp_j = 1; tmp_j < (long) from_part.ArrayDim(); tmp_j++ )
							{
								if( /*(tmp_it == to_treat->end())*/!(lleevv < (long) to_treat->size()) || (*tmp_it != from_part[ tmp_j ]) )
								{
									ok = false;
									break;
								}
								else
								{
									tmp_it++;
									lleevv++;
								}
							}
							if( ok )
								break;
							else
							{
								it++;
								lev++;
							}
						}
						else
						{
							it++; 
							lev++;
						}
					}
					if( ok )
					{
						fact.rule = i;
						fact.del_1st = lev;
						fact.direction = direction;
						fact.to_treat = new std::list<long>( *to_treat );
						fact.lock = new std::vector<long>( *to_lock );
						if(!(*to_lock)[ i ])
							(*(fact.lock))[ i ] = direction ? 1 : 2;
						hystory_location = hystory->begin();
						for( j2 = 0; j2 < (unsigned long) (hystory_index + hystory_local_count); j2++ )
							hystory_location++;
						hystory->insert( hystory_location, fact );
						hystory_count++;
						hystory_local_count++;
						in_fact = hystory->begin();
						fact.to_treat = 0;
						fact.lock = 0;
					}
				//}
			}	// while
		}
		// <end>

		// get current control structure
		
		//if( in_fact == hystory->end() )
		if( hystory_index >= hystory_count )
		{
			delete hystory;
			return INT_MAX;
		}

		in_fact = hystory->begin();
		for( j2 = 0; j2 < (unsigned long) hystory_index; j2++ )
			in_fact++;
		hystory_index++;
/*		if( start )
		{
			in_fact = hystory->begin();
			start = false;
		}
		else
			in_fact++;
*/
/*
		if( hystory_count >= hystory_location )
		{
			delete hystory;
			return INT_MAX;
		}
*/

		after_in_fact = in_fact;
		after_in_fact++;
		// <end>

		// constructing QString reduced
		MArray<long>& to = (in_fact->direction) ? left[ in_fact->rule ] : right[ in_fact->rule ];
		MArray<long>& fro = (in_fact->direction) ? right[ in_fact->rule ] : left[ in_fact->rule ];

		location = in_fact->to_treat->begin();
		for( j2 = 0; j2 < (unsigned long) in_fact->del_1st; j2++ )
			location++;
		for( k = 0; k < (long) to.ArrayDim(); k++ )
		{
			if( location != in_fact->to_treat->end() )
				in_fact->to_treat->erase( location++ );
		}

		if( (fro.ArrayDim() != 1) || (fro[ 0 ] != 0) || !(in_fact->to_treat->size()) )	// not E
		{
			location = in_fact->to_treat->begin();
			for( j2 = 0; j2 < (unsigned long) in_fact->del_1st; j2++ )
				location++;
			in_fact->to_treat->insert( location, fro.begin(), fro.end() );
		}
		to_treat = in_fact->to_treat;
		to_lock = in_fact->lock;
		// <end>

		// substring search
//		if( in_fact->to_treat->size() == 1 )
//		{
		for( k = 0; k < (long) str_id_int.size(); k++ )
		{
			if(to_treat->size() == str_id_int[ k ].size())
			{
				long lev = 0;
				std::list<long>::iterator it_1 = in_fact->to_treat->begin();
				std::list<long>::iterator it_2 = str_id_int[ k ].begin();
				while(lev < (long) in_fact->to_treat->size())
				{
					if( *it_1 != *it_2 )
						break;
					it_1++;
					it_2++;
					lev++;
				}
				if( lev == (long) in_fact->to_treat->size() )
				{
					//long ret = *(in_fact->to_treat->begin());
					delete hystory;
					return k;
				}
			}
		}
//		}
		// <end>
	}
	delete hystory;
	return INT_MAX;
}

void MMD_Group::FillInNewGene( long len,
								   MMD_GroupString& ab_x__left, MMD_GroupString& ab_x__right, 
								   MMD_GroupString& bc_y__left, MMD_GroupString& bc_y__right,
								   MMD_GroupString& ay_xc__left, MMD_GroupString& ay_xc__right )
{
	long i;
	for( i = 0; i < (long) ab_x__left.ArrayDim() - len; i++ )
		ay_xc__left.Add( ab_x__left[ i ] );
	if( bc_y__right[ 0 ] || ( (long) bc_y__right.ArrayDim() != 1 ) )
		for( i = 0; i < (long) bc_y__right.ArrayDim(); i++ )
			ay_xc__left.Add( bc_y__right[ i ] );
	if( ab_x__right[ 0 ] || ( (long) ab_x__right.ArrayDim() != 1 ) )
		for( i = 0; i < (long) ab_x__right.ArrayDim(); i++ )
			ay_xc__right.Add( ab_x__right[ i ] );
	for( i = len; i < (long) bc_y__left.ArrayDim(); i++ )
		ay_xc__right.Add( bc_y__left[ i ] );
}

void MMD_Group::SimplifyNewGenes( MMD_GroupString& a, MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx )
{
        long i, j, j2, k, start_j = 0;
	bool ok, change;

	do
	{
		change = false;
		for( i = 0; i < (long) tmp_left_idx.ArrayDim(); i++ )
		{
			ok = false;
			for( j = 0; j < (long) a.ArrayDim(); j++ )
			{
				if( a[ j ] == tmp_left_idx[ i ][ 0 ] )
				{
					start_j = j;
					j2 = j + 1;
					ok = true;
					for( k = 1; k < (long) tmp_left_idx[ i ].ArrayDim(); k++, j2++ )
					{
						if( (j2 == a.ArrayDim()) || (a[ j2 ] != tmp_left_idx[ i ][ k ]) )
						{
							ok = false;
							break;
						}
					}
					if( ok ) break;
				}
			}
			if( ok )
			{
				if( (tmp_right_idx[ i ].ArrayDim() == 1) && (tmp_right_idx[ i ][ 0 ] == 0) )
				{
//					if(tmp_left_idx[ i ].ArrayDim() == 1)
//						a.erase( a.begin() + start_j );
//					else
						a.erase( a.begin() + start_j, a.begin() + start_j + tmp_left_idx[ i ].ArrayDim()/* - 1 */);
				}
				else
				{
					for( j = 0; j < (long) tmp_right_idx[ i ].ArrayDim(); j++ )
						a[ start_j + j ] = tmp_right_idx[ i ][ j ];
					if( (k = (long) tmp_left_idx[ i ].ArrayDim() - (long) tmp_right_idx[ i ].ArrayDim()) > 0 )
					{
//						if(k == 1)
//							a.erase( a.begin() + start_j + j );
//						else
							a.erase( a.begin() + start_j + j, a.begin() + start_j + j + k/* - 1*/ );
					}
				}
				change = true;
			}

		}
	}
	while( change );
}

long MMD_Group::AddNewGene( long ii, long jj,
							   MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx )
{
	MMD_GroupString a, b;
	long len, ret, i, index_a, index_b, change_1 = 0, null = 0;
	for( len = 1; (len <= (long) tmp_left_idx[ ii ].ArrayDim() - 1) && (len <= (long) tmp_left_idx[ jj ].ArrayDim()); len++ )
	{
		for( i = 0; i < len; i++ )
		{
			if( tmp_left_idx[ ii ][ i ] != (long) tmp_left_idx[ jj ][ tmp_left_idx[ jj ].ArrayDim() - len + i ] )
				break;
		}
		if( i == len )
		{
			a.Flush();
			b.Flush();
			FillInNewGene( len, tmp_left_idx[ jj ], tmp_right_idx[ jj ], tmp_left_idx[ ii ], tmp_right_idx[ ii ], a, b );

			SimplifyNewGenes( a, tmp_left_idx, tmp_right_idx );
			SimplifyNewGenes( b, tmp_left_idx, tmp_right_idx );
			
			if( a.ArrayDim() == 0 ) a.Add( null );
			if( b.ArrayDim() == 0 ) b.Add( null );

                        ret = compare(a, b);
                        if( ret )
			{
				if( ret == 1 )
				{
					if( ((index_a = tmp_left_idx.Find( a )) == INT_MAX) ||
						((index_b = tmp_right_idx.Find( b )) == INT_MAX) ||
						(index_a != index_b) )
					{
						tmp_left_idx.Add( a );
						tmp_right_idx.Add( b );
						change_1++;
					}
				}
				else
				{
					if( ((index_b = tmp_left_idx.Find( b )) == INT_MAX) ||
						((index_a = tmp_right_idx.Find( a )) == INT_MAX) ||
						(index_b != index_a) )
					{
						tmp_left_idx.Add( b );
						tmp_right_idx.Add( a );
						change_1++;
					}
				}
			}
		}
	}
	return change_1;
}

void MMD_Group::DeduceNewGenes( MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx )
{
	//const double growing_koef = 10;	// rules quantity can't be increased more than in 10 time
	const long deepest_change_step = 10;

	long i, j, cb, new_c;
	long start_c, rules_c;

	start_c = 0;
	rules_c = (long) tmp_left_idx.ArrayDim();
	cb = 0;
	do
	{
		new_c = 0;
		if( start_c )
		{
			for( i = 0; i < start_c; i++ )
				for( j = start_c; j < rules_c; j++ )
					new_c += AddNewGene( i, j, tmp_left_idx, tmp_right_idx );

			for( i = start_c; i < rules_c; i++ )
				for( j = 0; j < start_c; j++ )
					new_c += AddNewGene( i, j, tmp_left_idx, tmp_right_idx );
		}
		for( i = start_c; i < rules_c; i++ )
			for( j = start_c; j < rules_c; j++ )
				new_c += AddNewGene( i, j, tmp_left_idx, tmp_right_idx );

		start_c = rules_c;
		rules_c += new_c;
		cb++;
	}
	while( new_c && (cb < deepest_change_step) );
}
	
bool MMD_Group::StopGenerating( unsigned long steps, unsigned long gpow )
{
	return (( steps > 256 ) || ( gpow > 8 ));
}

bool MMD_Group::IsValidGenes( long item_c, long argc, QString* left, QString *right, QString *items, MMD_GroupStringArray& tmp_left_idx, MMD_GroupStringArray& tmp_right_idx )
{
	long i, j, k;
	QString str;
	MArray<QString>	tmp_left( 256, 0, 64 ), tmp_right( 256, 0, 64 );
	for( i = 0; i < argc; i++ )
	{
		tmp_left.Flush();
		tmp_left_idx[ i ].Flush();
		j = 0;
		for(;;)
		{
			str = _T("");
			for( ; (j < (long) left[ i ].length()) && (left[ i ].at(j) != _T(' ')); j++ )
			{
				str += left[ i ].at(j);
			}
			if( str == commonGetNeutralElementName() )
				k = 0;
			else
			{
				for( k = 1; k < item_c + 1; k++ )
				{
					if( items[ k - 1 ] == str )
						break;
				}
				if( k == item_c + 1 )
					return false;
			}
			tmp_left_idx[ i ].Add( k );
			tmp_left.Add( str );
			if( j == left[ i ].length() )
				break;
			else
				j += 3;
		}

		tmp_right.Flush();
		tmp_right_idx[ i ].Flush();
		j = 0;
		for(;;)
		{
			str = _T("");
			for( ; (j < (long) right[ i ].length()) && (right[ i ].at(j) != _T(' ')); j++ )
			{
				str += right[ i ].at(j);
			}
			if( str == commonGetNeutralElementName() )
				k = 0;
			else
			{
				for( k = 1; k < item_c + 1; k++ )
				{
					if( items[ k - 1 ] == str )
						break;
				}
				if( k == item_c + 1 )
					return false;
			}
			tmp_right_idx[ i ].Add( k );
			tmp_right.Add( str );
			if( j == right[ i ].length() )
				break;
			else
				j += 3;
		}

		if( !Equilibrium( tmp_left_idx[ i ], tmp_right_idx[ i ] ) )
			return false;
	}
	// lets's sort
	return true;
}

bool MMD_Group::Equilibrium( MMD_GroupString& left_idx, MMD_GroupString& right_idx )
{
	MMD_GroupString tmp;
	if( left_idx.ArrayDim() < right_idx.ArrayDim() )
	{
		tmp = left_idx;
		left_idx = right_idx;
		right_idx = tmp;
	}
	else if( left_idx.ArrayDim() == right_idx.ArrayDim() )
	{
		long k;
		for( k = 0; k < (long) left_idx.ArrayDim(); k++ )
		{
			if( left_idx[ k ] > right_idx[ k ] )
				break;
			if( left_idx[ k ] < right_idx[ k ] )
			{
				tmp = left_idx;
				left_idx = right_idx;
				right_idx = tmp;
				break;
			}
		}
		if( k == left_idx.ArrayDim() )
			return false;
	}
	return true;
}

int compare( const MMD_GroupString& left_idx, const MMD_GroupString& right_idx )
{
	long ret = -1;
	if( left_idx.ArrayDim() < right_idx.ArrayDim() )
	{
		ret = -1;
	}
	else
	{
		if( left_idx.ArrayDim() == right_idx.ArrayDim() )
		{
			long k;
			for( k = 0; k < (long) left_idx.ArrayDim(); k++ )
			{
				if( left_idx[ k ] > right_idx[ k ] )
				{
					ret = 1;
					break;
				}
				if( left_idx[ k ] < right_idx[ k ] )
				{
					ret = -1;
					break;
				}
			}
			if( k == left_idx.ArrayDim() )
				ret = 0;
		}
		else
			ret = 1;
	}
	return ret;
}

void MMD_Group::Unroll( long i, std::list<long>::iterator it, std::list<long>& to_treat, MArray<long>& mem_left, MArray<long>& mem_right )
{
	if( (mem_left[ i ] == INT_MAX) && (mem_right[ i ] == INT_MAX) )
		to_treat.insert( it, i );
	else
	{
		Unroll( mem_left[ i ], it, to_treat, mem_left, mem_right );
		Unroll( mem_right[ i ], it, to_treat, mem_left, mem_right );
	}
}

///////////////////////////////////////////////////////////////////////////////


MMD_Object* MMD_Group::copy( const MMD_Group* ms )
{
	MMD_Object::copy( ms );
	ready = ms->ready;
	grp_power = ms->grp_power;
	long i;
	str_id.Flush();
	for( i = 0; i < (long) ms->str_id.ArrayDim(); i++ )
		str_id.Add( new QString( *(ms->str_id[ i ]) ) );
	table = ms->table;
	subgr_int.Flush();
	for( i = 0; i < (long) ms->subgr_int.ArrayDim(); i++ )
		subgr_int.Add( new long( *(ms->subgr_int[ i ]) ) );
	subgrp_det = ms->subgrp_det;
	return this;
}


MMD_Group::MMD_Group( CParseDsrSymbolTable& _smbtable ) :
	MMD_Object( _smbtable, DSRDATA_TYPE_ABS_GROUP ),
	str_id( 256, 0, 64 ), subgr_int( 8, 0, 8 )
{
	grp_power = -1;
	ready = GRT_Raw;
	ConstructGetReady();
}


MMD_Group::MMD_Group( const MMD_Group& ms ) :
	MMD_Object( ms ),
	str_id( 256, 0, 64 ), subgr_int( 8, 0, 8 )
{
	//copy( &ms );
	ready = ms.ready;
	grp_power = ms.grp_power;
	long i;
	str_id.Flush();
	for( i = 0; i < (long) ms.str_id.ArrayDim(); i++ )
		str_id.Add( new QString( *(ms.str_id[ i ]) ) );
	table = ms.table;
	subgr_int.Flush();
	for( i = 0; i < (long) ms.subgr_int.ArrayDim(); i++ )
		subgr_int.Add( new long( *(ms.subgr_int[ i ]) ) );
	subgrp_det = ms.subgrp_det;
}


MMD_Group::~MMD_Group( void )
{
	ReleaseGetReady();
}


enum GroupReadyTo MMD_Group::GetReady( void )
{
	return ready;
}


long& MMD_Group::GetTable( long a, long b ) // a - row, b - column
{
	return table( a, b );
}


MMD_GroupMulTable& MMD_Group::GetTable( void ) // a - row, b - column
{
	return table;
}


QString MMD_Group::GetGroupNeutralElementName( void )
{
	return str_id[ 0 ] ? *(str_id[ 0 ]) : _T("");
}


long MMD_Group::GetPower( void )
{
	return grp_power;
}


QString MMD_Group::GetInv( const QString& a )
{
	return GetString( GetInv( GetIndex( a ) ) );
}


long MMD_Group::GetIndex( const QString& name )
{
	QString str( name );
	return str_id.Find( &str );
}


QString MMD_Group::GetString( long index )
{
	if( index >= grp_power )
		return QString("");
	return str_id[ index ] ? *(str_id[ index ]) : _T("");
}


long MMD_Group::GetIndex2Index( long a, long b )
{
	return table( a, b );
}


QString MMD_Group::GetString2String( const QString& a, const QString& b )
{
	long x = GetIndex( a );
	long y = GetIndex( b );
	if( (x == INT_MAX) || (y == INT_MAX) )
		return QString("");
	else
		return GetString( GetIndex2Index( x, y ) );
}


QString MMD_Group::EvalString2String( const QStringList& names )
{
	long index = EvalString2Index( names );
	return (index == INT_MAX) ? QString("") : GetString( index );
}


QString MMD_Group::EvalString2String( const QString& names )
{
	long index = EvalString2Index( names );
	return (index == INT_MAX) ? QString("") : GetString( index );
}


void MMD_Group::ConstructGetReady( void )
{
	inf_genes.items = inf_genes.left = inf_genes.right = 0;
}


void MMD_Group::ReleaseGetReady( void )
{
	if( GetReady() == GRT_Ready2Genes )
	{
		if( inf_genes.items ) delete inf_genes.items;
		if( inf_genes.left ) delete inf_genes.left;
		if( inf_genes.right ) delete inf_genes.right;
	}
}

///////////////////////////////////////////////////////////////////////////////
