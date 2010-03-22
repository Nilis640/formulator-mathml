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

#include "../operator.h"
#include "../smbtbllink.h"

///////////////////////////////////////////////////////////////////////////////

void ml_InitFunctionSmbTable( CParseDsrSymbolTable *smbtable )
{
	CParseDsrSymbol toadd( _T(""), SMBTABLE_TYPE_FUNCTION );
	CDsrDataTypeTable& typetable = smbtable->getTypeTable();
	CDsrDataType_Composition *args;

	//=== abs ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_abs;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arch ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arch;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arccos ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arccos;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arccosec ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arccosec;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arcsec ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arcsec;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arcsin ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arcsin;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arctg ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arctg;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arcth ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arcth;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== arsh ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_arsh;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== ch ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_ch;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== cos ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_cos;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== cosec ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_cosec;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== cosech ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_cosech;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== ctg ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_ctg;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== cth ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_cth;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== sec ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_sec;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== sech ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_sech;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== sin ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_sin;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== sh ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_sh;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== sqrt ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_sqrt;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== tg ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_tg;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== th ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_th;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== exp ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_exp;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== int ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_INTEGER ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_int;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== lg ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_lg;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== ln ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_ln;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== log ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_log;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== pow ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_pow;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== root ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_root;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== atan2 ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_atan2;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== fmod ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_fmod;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== polar ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.param1 = FUNCTION_std_polar;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== ceil ===
	toadd.setDataType( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_ceil;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== floor ===
	toadd.setDataType( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_floor;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== imag ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_imag;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== pow10 ===
	toadd.setDataType( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_pow10;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== real ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_real;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== random ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	toadd.setDataType( typetable.makeDataTypeNode( new CDsrDataType_Function( args, typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) );
	toadd.param1 = FUNCTION_std_random;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== identity ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
				typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
				typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = FUNCTION_std_identity;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== primes ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_primes;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== isprime ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
				typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
				typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	toadd.param1 = FUNCTION_std_isprime;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== gcd ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
				typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
				typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) );
	toadd.param1 = FUNCTION_std_gcd;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== lcm ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
				typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
				typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) );
	toadd.param1 = FUNCTION_std_lcm;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== polyroots ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_polyroots;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== polyder ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) )
	);
	toadd.param1 = FUNCTION_std_polyder;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== polyint ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
			typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) )
	);
	toadd.param1 = FUNCTION_std_polyint;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== polygcd ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) )
	);
	toadd.param1 = FUNCTION_std_polygcd;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== polylcm ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) )
	);
	toadd.param1 = FUNCTION_std_polylcm;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== lsolve ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_STRING ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_lsolve;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== ratfpart ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
			typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( DSRDATA_TYPE_STRING ) )
	);
	toadd.param1 = FUNCTION_std_ratfpart;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== ratfder ===
	toadd.setDataType( 
		::__ml_makeFunctionNode( typetable, 
			typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
			typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) )
	);
	toadd.param1 = FUNCTION_std_ratfder;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== det ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_det;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== inverse ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_inverse;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== eigenvals ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_eigenvals;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

}

///////////////////////////////////////////////////////////////////////////////
