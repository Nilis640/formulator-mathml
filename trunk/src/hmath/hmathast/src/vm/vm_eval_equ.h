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

#if !defined( __PARSEDSR_VM_VM_EVAL_EQU_H__ )
#define __PARSEDSR_VM_VM_EVAL_EQU_H__

#include "vm_eval_cmd.h"

///////////////////////////////////////////////////////////////////////////////

#define CDSR_VMEval_Equation_l	-2
#define CDSR_VMEval_Equation_le	-1
#define CDSR_VMEval_Equation_e	 0
#define CDSR_VMEval_Equation_ge	 1
#define CDSR_VMEval_Equation_g	 2

///////////////////////////////////////////////////////////////////////////////

struct CDSR_VMEval_SE_VPair
{
	long p1, p2;	// p1 - id of the variable, p2 - index in array or 0
	CDSR_VMEval_SE_VPair( long _p1 = 0, long _p2 = 0 )
	{
		p1 = _p1;
		p2 = _p2;
	}
	int operator==( const CDSR_VMEval_SE_VPair& a ) const 
	{
		return p1 == a.p1 && p2 == a.p2;
	}
};

class CDSR_VMEval_SE_VPairArray : public MArray<CDSR_VMEval_SE_VPair>
{
public:
	CDSR_VMEval_SE_VPairArray( void ) : 
		MArray<CDSR_VMEval_SE_VPair>( 64, 0, 64 )
	{}
};

///////////////////////////////////////////////////////////////////////////////

class __HMATHLNGDLL__ CDSR_VMEval_Equation
{
public:
	int is_koshi;					// form of Koshi
	int is_linear;					// linear equation
	int is_algebraic;				// algebraic equation
	int is_differential;			// differential equation
	int sign;						// -2 <, -1 <=, 0 =, 1 >=, 2 >
	CDsrDataType_Node *data_type;	// data type of the equation
	MArray<long> used_xvar;			// list if smbtable ids of used x-vars
	MArray<long> used_dxvar;		// list if smbtable ids of used primes of x-vars
	CDSR_VMEval_Line left, right;	// left and right part of the equation

	CDSR_VMEval_Equation( void ) :
		used_xvar( 16, 0, 16 ), used_dxvar( 16, 0, 16 )
	{ is_koshi = is_linear = is_algebraic = is_differential = 0; data_type = 0; }
	CDsrDataType_Node* getDataType( void )
	{ return data_type; }
	void setDataType( CDsrDataType_Node *node )
	{ data_type = node; }

        int operator == ( CDSR_VMEval_Equation& /*a*/ ) const
	{
		return 0;
	}
};

class __HMATHLNGDLL__ CDSR_VMEval_SE : public MArray<CDSR_VMEval_Equation>
{
public:
	UniWordVector A, B;				// SLAE

	int is_koshi;					// form of Koshi
	int is_linear;					// linear equation
	int is_algebraic;				// algebraic equation
	int is_differential;			// differential equation
	CDsrDataType_Node *data_type;	// data type of the equation
	MArray<long> used_xvar;			// list if smbtable ids of used x-vars
	CDSR_VMEval_SE_VPairArray used_xvar_idx; // array (var_no, var_index_if_x_vector)
	long m_unknown_cell;			// number of unknown cells
	MArray<long> m_prime_index;		// either index of vector primes or 0
	CDSR_VMEval_SE_VPairArray m_left_part;	// indexes of vars in the left part of SODE in Koshi form

	CDSR_VMEval_SE( void ) :
		MArray<CDSR_VMEval_Equation>( 64, 0, 64 ),
		used_xvar( 16, 0, 16 ), m_prime_index( 64, 0, 64 )
	{
		is_koshi = is_linear = is_algebraic = is_differential = 0; 
		data_type = 0;
		m_unknown_cell = 0;
	}
	int Summarize( CParseDsrSymbolTable& smbtable );
	long getUnknownsNumber( CParseDsrSymbolTable& smbtable );
	void Equation_AddPrimeIndex( long idx )
	{ m_prime_index.Add( idx ); }
	int getKoshiEquationLeft( long eq_no, long& var_id, long& var_idx );
};

///////////////////////////////////////////////////////////////////////////////

__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_Equation& c );
__HMATHLNGDLL__
QString getTextView( CParseDsrSymbolTable& smbtable, CDSR_VMEval_SE& c );

///////////////////////////////////////////////////////////////////////////////

#endif
