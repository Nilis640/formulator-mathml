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

#include "../polynomial/internalfunc.h"
#include "../polynomial/polina.h"
#include "eigenval.h"
#include "matrix_internal.h"
#include "slae/lu.h"

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _eigenValLevere( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRArray<CDSRReal>& polynom, CDSRReal rlPrec )
{
	CDSRMMatrix<CDSRReal> A_k, Adp;
	CDSRArray<CDSRReal> Sp;
	CDSRArray<CDSRReal> *pPoly = &polynom;
	CDSRArray<ROOT_INFO> *pRootsArray = &Lyambda;
	CDSRReal	sum;
	long i, j, k, l;
	long n = A.n_row();

	A_k.resize_matrix(n, n);
	Adp.resize_matrix(n, n);
	Sp.resize(n);
	polynom.resize(n + 1);
	Lyambda.resize(0);

	/*
	k*pk = Sp[k] - p1*sk-1 - ... - pk-1*s1

k=0	(k+1)*p[k] = Sp[k]
k=1	(k+1)*p[k] = Sp[k] - p[0]*S[k-1]
k=2	(k+1)*p[k] = Sp[k] - p[0]*S[k-1] - p[1]*S[0]
.	.	.	.	.	.	.	.	.	.	.	.	.

	*/
	Sp[0] = 0.;
	for( i = 0; i < n; i++ )
	{
		for( j = 0; j < n; j++ )
		{
			A_k(i, j) = A(i, j);
//			Adp(i, j) = A(i, j);
		}
		Sp[ 0 ] += A(i, i);
	}

	for( k = 1; k < n; k++ )
	{
		for( i = 0; i < n; i++ )
			for( j = 0; j < n; j++ )
			{
				sum = 0.;
				for( l = 0; l < n; l++ )
					sum += A_k(i, l) * A(l, j);
				Adp(i, j) = sum;
			}

		Sp[ k ] = 0.;
		for( i = 0; i < n; i++ )
		{
			for( j = 0; j < n; j++ )
				A_k(i, j) = Adp(i, j);
			Sp[ k ] += Adp(i, i);
		}
	}

	polynom[ 0 ] = n % 2 ? -1 : 1;
	for( k = 1; k < n + 1; k++ )
	{
		polynom[ k ] = Sp[ k - 1 ];
		for( i = 1; i < k; i++ )
			polynom[ k ] -= -polynom[ i ] * Sp[ k - 1 - i ];
		polynom[ k ] = -polynom[ k ] / k;
	}
	_rootsOfPolinom(pPoly, pRootsArray, rlPrec);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _eigenValFadeev(CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRArray<CDSRReal>& polynom, CDSRReal rlPrec)
{
	CDSRMMatrix<CDSRReal> A_k, B_k;
	CDSRArray<CDSRReal> Sp /*, polynom*/;
	CDSRArray<CDSRReal> *pPoly = &polynom;
	CDSRReal	sum;
	CDSRArray<ROOT_INFO> *pRootsArray = &Lyambda;
	long i, j, k, l;
	long n = A.n_row();

	A_k.resize_matrix(n, n);
	B_k.resize_matrix(n, n);
	Sp.resize(n);
	Lyambda.resize(0);
	polynom.resize(n);

	Sp[ 0 ] = 0.;
	for( i = 0; i < n; i++ )
	{
		for( j = 0; j < n; j++ )
			A_k(i, j) = A(i, j);
		Sp[ 0 ] += A(i, i);
	}

	polynom[ 0 ] = Sp[ 0 ]; // q1 = Sp A1

	for( i = 0; i < n; i++ ) // B1 = A1 - q1E
		for( j = 0; j < n; j++ )
			if( i != j )
				B_k(i, j) = A_k(i, j);
			else
				B_k(i, i) = A_k(i, i) - polynom[0];

	for( k = 1; k < n; k++ )
	{
		for( i = 0; i < n; i++ )
			for( j = 0; j < n; j++ )
			{
				sum = 0.;
				for( l = 0; l < n; l++ )
					sum += A(i, l)*B_k(l, j);
				A_k(i, j) = sum;
			}

		Sp[ k ] = 0.;
		for( i = 0; i < n; i++ )
			Sp[ k ] += A_k(i, i);

		polynom[ k ] = Sp[ k ] / ( k + 1 ); // qk = Sp Ak / k

		for( i = 0; i < n; i++ ) // Bk=Ak - qkE
			for( j = 0; j < n; j++ )
				if( i != j )
					B_k(i, j) = A_k(i, j);
				else
					B_k(i, i) = A_k(i, i) - polynom[ k ];
	}

	polynom.insert(0, -1);
	for( i = 0; i < n + 1; i++ )
		polynom[ i ] = n % 2 ? polynom[ i ] : -polynom[ i ];

	_rootsOfPolinom(pPoly, pRootsArray, rlPrec);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _eigenValDanilev( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRReal rlPrec )
{
	CDSRMMatrix<CDSRReal> A_k, B_k, A_2;
	CDSRArray<CDSRReal> P_1, P_2, P;
	CDSRArray<CDSRReal> *pPoly_1 = &P_1;
	CDSRArray<CDSRReal> *pPoly = &P;
	CDSRArray<ROOT_INFO> *pRootsArray = &Lyambda;
	CDSRArray<ROOT_INFO> _Lya;
	ROOT_INFO info;
	CDSRReal	temp;
	long l, flag, flag_vir = 0;
	long i, j, k;
	long n = A.n_row();

	A_k.resize_matrix(n, n);
	B_k.resize_matrix(n, n);
	P.resize(n);
	Lyambda.resize(0);

	for( i = 0; i < n; i++ )
		for( j = 0; j < n; j++ )
			A_k(i, j) = A(i, j);

	for( k = 0; k < n - 1 ; k++ )
	{
		if( !IsNoll( A_k(k + 1, k) ) )
			for( j = 0; j < n; j++ )
				B_k(k + 1, j) = A_k(k + 1, j) / A_k(k + 1, k);
		else
		{
			flag = 0;
			for( i = 0; i < n; i++ )
			{
				if( !IsNoll(A_k(i, k)) && i > k + 1 )
				{
					flag = 1;
					for( l = 0; l < n; l++ )
					{
						temp = A_k(k + 1, l);
						A_k(k + 1, l) = A_k(i, l);
						A_k(i, l) = temp;
					}
					for( l = 0; l < n; l++ )
					{
						temp = A_k(l, k + 1);
						A_k(l, k + 1) = A_k(l, i);
						A_k(l, i) = temp;
					}
					for( j = 0; j < n; j++ )
						B_k(k + 1, j) = A_k(k + 1, j) / A_k(k + 1, k);
					break;
				}
			}
			if( !flag )
			{
				P_1.resize(k + 2);
				P_1[ 0 ] = 1;
				for( l = 0; l < k; l++ )
					P_1[ l + 1 ] = - A_k(k - l - 1, k);
				if( ( k + 1 ) % 2 )
					for( l = 0; l < k + 2; l++ )
						P_1[ l ] = - P_1[ l ];
				_rootsOfPolinom(pPoly_1, pRootsArray, rlPrec);

				P_2.resize(n);
				A_2.resize_matrix(n - k, n - k);
				for( i = 0; i < n - k - 1; i++ )
					for( l = 0; l < n - k - 1; l++ )
						A_2(i, l) = A_k(k + 1 + i, k + 1 + l);
				if( n - k != 2 )
				{
					_eigenValDanilev(A_2, _Lya, rlPrec);
					for( i = 0; i < (long) _Lya.size(); i++ )
						Lyambda.add(_Lya[ i ]);
				}
				else
				{
					info.real = A_2(0, 0);
					info.image = 0.;
					info.isComplex = false;
					info.multiplicity = 1;
					Lyambda.add(info);
					//Lyambda.add(A_2(0, 0));
				}
				flag_vir = 1;
				break;
			}
		}

		if( flag_vir )
			break;
		for( i = 0; i < n; i++ )
			for( j = 0; j < n; j++ )
				if( i != k + 1)
					B_k(i, j) = A_k(i, j)  - \
									A_k(i, k) * B_k(k + 1, j);
		for( i = 0; i < n; i++ )
		{
			CDSRReal sum;
			sum = 0.0;
			for( j = 0; j < n; j++ )
			{
				sum += B_k(i, j) * A_k(j, k);
			}
			A_k(i, k + 1) = sum;
		}
		for( i = 0; i < n; i++ )
			for( j = 0; j < n; j++ )
				if( j != k + 1)
					A_k(i, j) = B_k(i, j);
	}
	if( !flag_vir )
	{
		for( i = 0; i < n; i++ )
			P[ i ] = -A_k( n - i - 1, n - 1);
		P.insert(0, 1);
		_rootsOfPolinom(pPoly, pRootsArray, rlPrec);
	}
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _eigenValKrilov( CDSRMMatrix<CDSRReal>& A, CDSRArray<ROOT_INFO>& Lyambda, CDSRArray<CDSRReal>& polynom, CDSRReal rlPrec )
{
	long i, j, k;
	long n = A.n_row();
	CDSRMMatrix<CDSRReal> C_k(n, n);
	CDSRMVector<CDSRReal> vectorB(n), vectorP(n);
//	CDSRArray<CDSRReal> polynom;
	CDSRArray<CDSRReal> *pPoly = &polynom;
	CDSRArray<ROOT_INFO> *pRootsArray = &Lyambda;

	C_k(0, 0) = 1;
	for( i = 1; i < n; i++ )
		C_k(i, 0) = 0; // vektor-stolbik C0 = { 1, 0,..., 0 }

	for( k = 0; k < n ; k++ )
	{
		for( i = 0; i < n; i++ )
		{
			CDSRReal sum;
			sum = 0.0;
			for( j = 0; j < n; j++ )
				sum += A(i, j) * C_k(j, k);
			if( k + 1 < n )
				C_k(i, k + 1) = sum;
			else
				vectorB[ i ] = sum;

		}
	}
/*
 --  
 | Ñ_k[0]*p1 +  C_k[1]*p2 +  ... C_k[n-1]*pn = Ñ_k[n]
/  Ñ_k[1*(n+1)]*p1 +  C_k[1*(n+1) +1 ]*p2 +  ... = Ñ_k[1*(n+1) + n] 
\  ..........................................
 | Ñ_k[n*(n+1)]*p1 +  C_k[n*(n+1) +1 ]*p2 +  ... = Ñ_k[n*(n+1) + n] 
  --
*/
	_LU(&C_k, &vectorB, &vectorP);
	polynom. resize(n + 1);
	polynom[ 0 ] = 1;
	for( i = 1; i < n + 1; i++ )
		polynom[ i ] = -vectorP[ n - i ];
	Lyambda.resize(0);
	_rootsOfPolinom(pPoly, pRootsArray, rlPrec);
}

//////////////////////////////////////////////////////////////////////

MATHBASE_API
void _eigenValJacobi( CDSRMMatrix<CDSRReal>& A, CDSRArray<CDSRReal>& Lyambda, CDSRReal rlPrec )
{
	CDSRMMatrix<CDSRReal> A_k, C_k;
	CDSRReal	c, s, d, r, t2;
	long i, j, k, l, i_max = 0, j_max = 1;
	int	sgn;

	Lyambda.resize(A.n_row());
	A_k.resize_matrix(A.n_row(), A.n_column());
	C_k.resize_matrix(A.n_row(), A.n_column());
	for( i = 0; i < A.n_row(); i++ )
		for( j = 0; j < A.n_column(); j++ )
			A_k(i, j) = A(i, j);

	t2 = SumOffdiagonalSqrs( A_k );

	while( t2 > rlPrec )
	{
		for( i = 0; i < A.n_row() - 1; i++ )
			for( j = i + 1; j < A.n_column(); j++ )
				if( fabs(A_k(i, j)) > fabs( A_k(i_max, j_max) ) )
				{
					i_max = i;
					j_max = j;
				}
		i = i_max;
		j = j_max;
		d = ( A_k(i, i) - A_k(j, j) )*( A_k(i, i) - A_k(j, j) );
		d = sqrt(d + 4*A_k(i, j) * A_k(i, j) );
		r = fabs(A_k(i, i) - A_k(j, j));
		c = sqrt( ( 1 + r/d )/2 );
		if( A_k(i, j)*( A_k(i, i) - A_k(j, j) ) >= 0 )
			sgn = 1;
		else
			sgn = -1;
		s = sgn * sqrt(( 1 - r/d )/2);
		for( k = 0; k < A.n_row(); k++ )
			for( l = 0; l < A.n_column(); l++ )
				if( k != i && k != j && l != i && l != j )
					C_k(k, l) = A_k(k, l);

		for( k = 0; k < A.n_row(); k++ )
			if( k != i && k != j )
				C_k(k, i) = C_k(i, k) = c * A_k(k, i) + s * A_k(k, j);

		for( k = 0; k < A.n_row(); k++ )
			if( k != i && k != j )
				C_k(k, j) = C_k(j, k) = - s * A_k(k, i) + c * A_k(k, j);

		r = c*c * A_k(i, i) + s*s * A_k(j, j);
		C_k(i, i) = r + 2*c*s*A_k(i, j);
		r = s*s * A_k(i, i) + c*c * A_k(j, j);
		C_k(j, j) = r - 2*c*s * A_k(i, j);
		C_k(i, j) = C_k(j, i) = 0;
		for( i = 0; i < A.n_row(); i++ )
			for( j = 0; j < A.n_column(); j++ )
				A_k(i, j) = C_k(i, j);
		t2 = SumOffdiagonalSqrs( A_k );
	}

	for( i = 0; i < A.n_row(); i++ )
	{
		CDSRReal sum;
		sum = 0.;
		for( j = 0; j < A.n_column(); j++ )
			if( j!= i)
				sum += A_k(i, j) * A_k(i, j)/( A_k(i, i) - A_k(j, j) );
		Lyambda[ i ] = A_k(i, i) + sum;
	}
}

//////////////////////////////////////////////////////////////////////
