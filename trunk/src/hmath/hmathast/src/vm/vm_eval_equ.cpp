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

#include "vmeval.h"

///////////////////////////////////////////////////////////////////////////////

int CDSR_VMEval_SE::Summarize( CParseDsrSymbolTable& smbtable )
{
	if( !ArrayDim() ) return 0;

	long i, j;
	used_xvar.Flush();
	is_koshi = is_linear = 1;
	is_algebraic = is_differential = 0;
	data_type = 0;
	for( i = 0; i < (long) ArrayDim(); i++ )
	{
		is_koshi  = is_koshi  && (*this)[ i ].is_koshi;
		is_linear = is_linear && (*this)[ i ].is_linear;
		is_algebraic = is_algebraic || (*this)[ i ].is_algebraic;
		is_differential = is_differential || (*this)[ i ].is_differential;
		data_type = (*this)[ i ].data_type;
		for( j = 0; j < (long) (*this)[ i ].used_xvar.ArrayDim(); j++ )
			if( used_xvar.Find( (*this)[ i ].used_xvar[ j ] ) == INT_MAX )
				used_xvar.Add( (*this)[ i ].used_xvar[ j ] );
		for( j = 0; j < (long) (*this)[ i ].used_dxvar.ArrayDim(); j++ )
			if( used_xvar.Find( (*this)[ i ].used_dxvar[ j ] ) == INT_MAX )
				used_xvar.Add( (*this)[ i ].used_dxvar[ j ] );
	}

	long _unknowns = getUnknownsNumber( smbtable );
#if _DEBUG
	if( used_xvar_idx.ArrayDim() != _unknowns )
		throw _T("CDSR_VMEval_SE::Summarize : internal problem");
#endif
	if( (is_differential && is_algebraic) || (!is_differential && !is_algebraic) )
	{
		smbtable.AddLine( _T("system of equations must be either differential or algebraical") );
		return -1;
	}
	else if( _unknowns != ArrayDim() )
	{
		smbtable.AddLine( QString("system of %1 equations has different number of equations and unknown variables").arg( is_differential ? _T("differential") : _T("algebraical") ) );
		return -1;
	}
	else if( is_differential && !is_koshi )
	{
		smbtable.AddLine( _T("system of differential equations must be in Koshi form") );
		return -1;
	}

	if( is_differential && is_koshi )
	{
		m_left_part.Flush();
		if( m_prime_index.ArrayDim() != ArrayDim() )
		{
			smbtable.AddLine( _T("internal problem while the system of ODE in Koshi form is forming") );
			return -1;
		}
		long i, j;
		for( i = 0; i < (long) ArrayDim(); i++ )
		{
			if( (*this)[ i ].used_dxvar.ArrayDim() != 1 )
			{
				smbtable.AddLine( _T("internal problem while the system of ODE in Koshi form is forming") );
				return -1;
			}
			CDSR_VMEval_SE_VPair pair( (*this)[ i ].used_dxvar[ 0 ], m_prime_index[ i ] );
			if( (j = m_left_part.Find( pair )) != INT_MAX )
			{
				smbtable.AddLine( QString("system of differential equations in Koshi form has duplicated left parts in equations %1 and %2").arg( j ).arg( i ) );
				return -1;
			}
			m_left_part.Add( pair );
		}
	}

	return 0;
}

long CDSR_VMEval_SE::getUnknownsNumber( CParseDsrSymbolTable& smbtable )
{
	m_unknown_cell = 0;
	used_xvar_idx.Flush();
	for( long i = 0; i < (long) used_xvar.ArrayDim(); i++ )
	{
		CParseDsrSymbol *smb = smbtable.getSymbol( used_xvar[ i ] );
		if( !smb || !smb->getDataType() ) continue;
		if( smb->getDataType()->getType() == DSRDATA_TYPE_VECTOR )	// equation in vector form
		{
			CDsrDataType_Vector *tp = (CDsrDataType_Vector*)(smb->getDataType());
			long delta = (tp->getColumns() == 1) ? tp->getRows() : tp->getColumns();
			m_unknown_cell += delta;
			for( long j = 1; j <= delta; j++ )
			{
				CDSR_VMEval_SE_VPair item( used_xvar[ i ], j );
				used_xvar_idx.Add( item );
			}
		}
		else	// equation in scalar form
		{
			m_unknown_cell++;
			CDSR_VMEval_SE_VPair item( used_xvar[ i ], 0 );
			used_xvar_idx.Add( item );
		}
	}
	return m_unknown_cell;
}

int CDSR_VMEval_SE::getKoshiEquationLeft( long eq_no, long& var_id, long& var_index )
{
	if( is_differential && is_koshi && eq_no >= 0 && eq_no < (long) m_left_part.ArrayDim() )
	{
		var_id = m_left_part[ eq_no ].p1;
		var_index = m_left_part[ eq_no ].p2;
		return 0;
	}
	var_id = var_index = -1;
	return -1;
}

///////////////////////////////////////////////////////////////////////////////

int CDSR_VMEval::getEquation_getXVar( long x_var_order_no, UniWord& value )
{
	if( x_var_order_no < 0 ||
		x_var_order_no >= getEquation_UnknownNumber() ||
		getEquation_UnknownNumber() != m_equ_table.used_xvar_idx.ArrayDim() )
		return -1;
	long var_no = m_equ_table.used_xvar_idx[ x_var_order_no ].p1;
	long var_idx = m_equ_table.used_xvar_idx[ x_var_order_no ].p2;
	return var_idx ? getVariable_getVectorValue( var_no, var_idx, value ) : 
					 getVariable_getValue( var_no, value );
}

int CDSR_VMEval::getEquation_setXVar( long x_var_order_no, UniWord& value )
{
	if( x_var_order_no < 0 ||
		x_var_order_no >= getEquation_UnknownNumber() ||
		getEquation_UnknownNumber() != m_equ_table.used_xvar_idx.ArrayDim() )
		return -1;
	long var_no = m_equ_table.used_xvar_idx[ x_var_order_no ].p1;
	long var_idx = m_equ_table.used_xvar_idx[ x_var_order_no ].p2;
	return var_idx ? getVariable_setVectorValue( var_no, var_idx, value ) : 
					 getVariable_setValue( var_no, value );
}

int CDSR_VMEval::InitSLAU_AB( void )
{
	if( !getEquation_isLinear() || !getEquation_isAlgebraic() )
		return -1;

	long dx = getEquation_UnknownNumber();
	long dy = getEquation_Number();

	if( !dx || !dy )
		return -1;

	UniWordVector& A = getEquation_LinearA();
	UniWordVector& B = getEquation_LinearB();
	A.resize( dx * dy );
	B.resize( dy );

	UniWord value0, value1, vl, vr;
	switch( getEquation_Type() )
	{
	case DSRDATA_TYPE_INTEGER:
		value0 = CDSRInteger( 0 );
		value1 = CDSRInteger( 1 );
		break;
	case DSRDATA_TYPE_RATIONAL:
		value0 = CDSRRational( 0, 1 );
		value1 = CDSRRational( 1, 1 );
		break;
	case DSRDATA_TYPE_REAL:
		value0 = CDSRReal( 0.0 );
		value1 = CDSRReal( 1.0 );
		break;
	case DSRDATA_TYPE_COMPLEX:
		value0 = CDSRComplex( 0.0, 0.0 );
		value1 = CDSRComplex( 1.0, 0.0 );
		break;
	default:
		return -1;
	}

	long i, j;

	//=== vector B
	for( i = 0; i < dx; i++ ) 
		getEquation_setXVar( 0, value0 );
	for( i = 0; i < dy; i++ )
	{
		execEquationLeft( i, &vl );
		execEquationRight( i, &vr );
		::_dsrop_Sub_scalar( B[ i ], vr, vl );
	}

	//=== matrix A
	for( j = 0; j < dx; j++ )
	{
		if( j != 0 ) getEquation_setXVar( j - 1, value0 );
		getEquation_setXVar( j, value1 );

		for( i = 0; i < dy; i++ )
		{
			execEquationLeft( i, &vl );
			execEquationRight( i, &vr );
			::_dsrop_Sub_scalar( vl, vl, vr );
			::_dsrop_Add_scalar( A[ i * dx + j ], vl, B[ i ] );
		}
	}
	return 0;
}

void CDSR_VMEval::getEquation_getXVariableNameList( std::vector<QString>& dest )
{
	dest.erase( dest.begin(), dest.end() );
	long var_no;
	for( long i = 0; i < (long) m_equ_table.used_xvar.ArrayDim(); i++ )
	{
		var_no = getVariable_getNoByID( m_equ_table.used_xvar[ i ] );
		dest.push_back( getVariable_Name( var_no ) );
	}
}

int CDSR_VMEval::getEquation_setInitialXVar( long eq_no, UniWord& value )
{
	if( eq_no < 0 || eq_no >= getEquation_Number() )
		return -1;
	long var_id = -1, var_no = -1, var_idx = 0;
	getEquation_getLeftKoshiVariable( eq_no, var_id, var_idx );
	var_no = getVariable_getNoByID( var_id );
	return var_idx ? getVariable_setVectorValue( var_no, var_idx, value ) : 
					 getVariable_setValue( var_no, value );
}

///////////////////////////////////////////////////////////////////////////////
