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

#include "HUtils/idfas.h"
#include "../data/data.h"
#include "symbol_table_dsr.h"

///////////////////////////////////////////////////////////////////////////////

CParseDsrSymbolTable::CParseDsrSymbolTable( int is_local ) :
	m_table( 64, 0, 64 ), m_used_xvar( 16, 0, 16 ), m_used_dxvar( 16, 0, 16 )
{
	static LDsr2Var kdfa;
	m_infile = _T("");
	m_is_local = is_local;
	m_var_number = 1;
	m_fun_number = 1;
	if( !is_local )
	{
		CParseDsrSymbol toadd( _T(""), SMBTABLE_TYPE_KEYWORD );
		for( long i = kdfa.getKeywordStartIdx(); i < kdfa.getKeywordNumber(); i++ )
		{
			toadd.name = kdfa.getKeyword( i );
			toadd.setDataType( getTypeTable().makeDataTypeNode( DSRDATA_TYPE_KEYWORD ) );
			Add( toadd );
		}
		::ml_InitOperatorSmbTable( this );
		::ml_InitFunctionSmbTable( this );
		::ml_InitFormulaSmbTable( this );
		::ml_InitConstSmbTable( this );
		setMessanger();
	}
}

CParseDsrSymbolTable::~CParseDsrSymbolTable( void )
{
	for( long i = 0; i < (long) m_local_smbtable.size(); i++ )
	{
		if( m_local_smbtable[ i ] )
			delete m_local_smbtable[ i ];
	}
	ObjectTableFlush();
}

CParseDsrSymbolTable* CParseDsrSymbolTable::getNewLocalSmbTable( void )
{
	CParseDsrSymbolTable *lst = new CParseDsrSymbolTable( 1 );
	m_local_smbtable.push_back( lst );
	return lst;
}

void CParseDsrSymbolTable::allocateVariables( enum CDsrFunctionListID address_scheme )
{
	long i;
	for( i = 0; i < m_table.ArrayDim(); i++ )
	{
		if( m_table[ i ].smb_record_type == SMBTABLE_TYPE_VARIABLE && !m_table[ i ].param2 )
		{
			m_table[ i ].param1 = address_scheme;
			m_table[ i ].param2 = getNewVarNo();
		}		
	}

	for( i = 0; i < (long) m_local_smbtable.size(); i++ )
	{
		if( m_local_smbtable[ i ] )
			m_local_smbtable[ i ]->allocateVariables( LOCAL_VARIABLE_USER );
	}
}
/*
void CParseDsrSymbolTable::addReservedAsk( void )
{
	CParseDsrSymbol toadd( "?", SMBTABLE_TYPE_VARIABLE );
	toadd.setDataType( makeDefaultDataTypeNode() );
	toadd.getDataType()->setXValue();
	Add( toadd );
}
*/
///////////////////////////////////////////////////////////////////////////////

MMD_Vector* CParseDsrSymbolTable::makeUniWord( MMD_Vector* node )
{
	m_object_table.push_back( node );
	return node;
}

MMD_Set* CParseDsrSymbolTable::makeUniWord( MMD_Set* node )
{
	m_object_table.push_back( node );
	return node;
}

MMD_Group* CParseDsrSymbolTable::makeUniWord( MMD_Group* node )
{
	m_object_table.push_back( node );
	return node;
}

MMD_Polynomial* CParseDsrSymbolTable::makeUniWord( MMD_Polynomial* node )
{
	m_object_table.push_back( node );
	return node;
}

MMD_RationalFun* CParseDsrSymbolTable::makeUniWord( MMD_RationalFun* node )
{
	m_object_table.push_back( node );
	return node;
}

MMD_String* CParseDsrSymbolTable::makeUniWord( MMD_String* node )
{
	m_object_table.push_back( node );
	return node;
}

void CParseDsrSymbolTable::ObjectTableFlush( void )
{
	for( long i = 0; i < (long) m_object_table.size(); i++ )
	{
		if( m_object_table[ i ] )
			delete m_object_table[ i ];
	}
}

///////////////////////////////////////////////////////////////////////////////

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRArray<CDSRNatural>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_NATURAL, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_NATURAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRArray<CDSRInteger>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_INTEGER, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_INTEGER, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRArray<CDSRRational>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_RATIONAL, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_RATIONAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRArray<CDSRReal>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_REAL, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_REAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRArray<CDSRComplex>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_COMPLEX, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_COMPLEX, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMMatrix<CDSRNatural>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_NATURAL, (long) res.row(), (long) res.col() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_NATURAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMMatrix<CDSRInteger>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_INTEGER, (long) res.row(), (long) res.col() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_INTEGER, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMMatrix<CDSRRational>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_RATIONAL, (long) res.row(), (long) res.col() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_RATIONAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMMatrix<CDSRReal>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_REAL, (long) res.row(), (long) res.col() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_REAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMMatrix<CDSRComplex>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_COMPLEX, (long) res.row(), (long) res.col() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_COMPLEX, 0, 0 ) );
	return vec;
}

int CParseDsrSymbolTable::convert2DSRNaturalArray( MMD_Vector *vec, CDSRArray<CDSRNatural>& res )
{
	if( !vec || vec->getType() != DSRDATA_TYPE_NATURAL ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( CDSRNatural( (*vec)[ i ].getNatural() ) );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRIntegerArray( MMD_Vector *vec, CDSRArray<CDSRInteger>& res )
{
	if( !vec || vec->getType() != DSRDATA_TYPE_INTEGER ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( CDSRInteger( (*vec)[ i ].getInteger() ) );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRationalArray( MMD_Vector *vec, CDSRArray<CDSRRational>& res )
{
	if( !vec || vec->getType() != DSRDATA_TYPE_RATIONAL ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( (*vec)[ i ].getRational() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRealArray( MMD_Vector *vec, CDSRArray<CDSRReal>& res )
{
	if( !vec || vec->getType() != DSRDATA_TYPE_REAL ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( (*vec)[ i ].getReal() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRComplexArray( MMD_Vector *vec, CDSRArray<CDSRComplex>& res )
{
	if( !vec || vec->getType() != DSRDATA_TYPE_COMPLEX ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( (*vec)[ i ].getComplex() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRNaturalMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRNatural>& res )
{
	if( !vec ) return -1;
	res.resize_matrix( vec->getRows(), vec->getColumns() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res[ i ] = CDSRNatural( (*vec)[ i ].getNatural() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRIntegerMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRInteger>& res )
{
	if( !vec ) return -1;
	res.resize_matrix( vec->getRows(), vec->getColumns() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res[ i ] = CDSRInteger( (*vec)[ i ].getInteger() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRationalMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRRational>& res )
{
	if( !vec ) return -1;
	res.resize_matrix( vec->getRows(), vec->getColumns() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res[ i ] = (*vec)[ i ].getRational();
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRealMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRReal>& res )
{
	if( !vec ) return -1;
	res.resize_matrix( vec->getRows(), vec->getColumns() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res[ i ] = (*vec)[ i ].getReal();
	return 0;
}

int CParseDsrSymbolTable::convert2DSRComplexMMatrix( MMD_Vector *vec, CDSRMMatrix<CDSRComplex>& res )
{
	if( !vec ) return -1;
	res.resize_matrix( vec->getRows(), vec->getColumns() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res[ i ] = (*vec)[ i ].getComplex();
	return 0;
}

MMD_Polynomial* CParseDsrSymbolTable::makeUniWord_Polynomial( CDSRArray<CDSRInteger>& res )
{
	return makeUniWord( new MMD_Polynomial( *this, res ) );
}

MMD_Polynomial* CParseDsrSymbolTable::makeUniWord_Polynomial( CDSRArray<CDSRReal>& res )
{
	return makeUniWord( new MMD_Polynomial( *this, res ) );
}

MMD_Polynomial* CParseDsrSymbolTable::makeUniWord_Polynomial( CDSRArray<CDSRComplex>& res )
{
	return makeUniWord( new MMD_Polynomial( *this, res ) );
}

int CParseDsrSymbolTable::convert2DSRIntegerArray( MMD_Polynomial *vec, CDSRArray<CDSRInteger>& res )
{
	vec->getCoef( res );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRealArray( MMD_Polynomial *vec, CDSRArray<CDSRReal>& res )
{
	vec->getCoef( res );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRComplexArray( MMD_Polynomial *vec, CDSRArray<CDSRComplex>& res )
{
	vec->getCoef( res );
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

int CParseDsrSymbolTable::convert2DSRNaturalMVector( MMD_Vector *vec, CDSRMVector<CDSRNatural>& res )
{
	if( !vec ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( CDSRNatural( (*vec)[ i ].getNatural() ) );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRIntegerMVector( MMD_Vector *vec, CDSRMVector<CDSRInteger>& res )
{
	if( !vec ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( CDSRInteger( (*vec)[ i ].getInteger() ) );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRationalMVector( MMD_Vector *vec, CDSRMVector<CDSRRational>& res )
{
	if( !vec ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( (*vec)[ i ].getRational() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRRealMVector( MMD_Vector *vec, CDSRMVector<CDSRReal>& res )
{
	if( !vec ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( (*vec)[ i ].getReal() );
	return 0;
}

int CParseDsrSymbolTable::convert2DSRComplexMVector( MMD_Vector *vec, CDSRMVector<CDSRComplex>& res )
{
	if( !vec ) return -1;
	if( res.size() )
		res.erase( 0, res.end() - res.begin() );
	for( long i = 0; i < (long) vec->getCount(); i++ )
		res.push_back( (*vec)[ i ].getComplex() );
	return 0;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMVector<CDSRNatural>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_NATURAL, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_NATURAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMVector<CDSRInteger>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_INTEGER, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_INTEGER, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMVector<CDSRRational>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_RATIONAL, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_RATIONAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMVector<CDSRReal>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_REAL, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_REAL, 0, 0 ) );
	return vec;
}

MMD_Vector* CParseDsrSymbolTable::makeUniWord_Vector( CDSRMVector<CDSRComplex>& res )
{
	MMD_Vector *vec;
	if( res.size() )
	{
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_COMPLEX, 1, (long) res.size() ) );
		for( long i = 0; i < (long) res.size(); i++ )
			(*vec)[ i ] = res[ i ];
	}
	else
		vec = makeUniWord( new MMD_Vector( *this, DSRDATA_TYPE_COMPLEX, 0, 0 ) );
	return vec;
}

///////////////////////////////////////////////////////////////////////////////

void CParseDsrSymbolTable::debugPrint( QString& dest, long flags )
{
	long i;
	for( i = 0; i < (long) m_table.ArrayDim(); i++ )
	{
		if( flags & (1 << m_table[ i ].smb_record_type) )
		{
			m_table[ i ].debugPrint( dest, this );
			dest += '\n';
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
