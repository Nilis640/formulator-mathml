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

void ml_InitFormulaSmbTable( CParseDsrSymbolTable *smbtable )
{
	CParseDsrSymbol toadd( _T(""), SMBTABLE_TYPE_FORMULA );
	CDsrDataTypeTable& typetable = smbtable->getTypeTable();
	CDsrDataType_Composition *args;
	
	//=== FORMULA_box_lt ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_box_lt;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_box_rt ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_box_rt;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_box_lb ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_box_lb;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_box_rb ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_box_rb;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_box ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_box;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_abs ===
	toadd.name = ::ml_Op2String( FORMULA_abs );
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_abs;
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_norm ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_norm;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_floor ===
	toadd.name = ::ml_Op2String( FORMULA_floor );
	toadd.setDataType( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_floor;
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_ceil ===
	toadd.name = ::ml_Op2String( FORMULA_ceil );
	toadd.setDataType( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	toadd.param1 = FUNCTION_std_ceil;
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_fraction ===
	toadd.name = ::ml_Op2String( FORMULA_fraction );
	//=== OPERATOR_op_div ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	// poly / const has higher priority than poly / poly if a type conversion is needed
	// so the order of the next 2 sections is critical
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	//args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	//args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	//args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_div;
	smbtable->Add( toadd );
	//=======================
	//=======================
	
	//=== FORMULA_diagonal_fraction ===
	toadd.name = ::ml_Op2String( FORMULA_diagonal_fraction );
	//=== OPERATOR_op_div ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	// poly / const has higher priority than poly / poly if a type conversion is needed
	// so the order of the next 2 sections is critical
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	//args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	//args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	//args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_div;
	smbtable->Add( toadd );
	//=======================
	//=======================
	
	//=== FORMULA_sqrt ===
	toadd.name = ::ml_Op2String( FORMULA_sqrt );
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_sqrt;
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_root ===
	toadd.name = ::ml_Op2String( FORMULA_root );
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.setDataType( args );
	toadd.param1 = FUNCTION_std_root;
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_plusminus ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_plusminus;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_circle_plus ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_circle_plus;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_circle_times ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_circle_times;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_minus ===
	toadd.name = ::ml_Op2String( FORMULA_minus );
	//=== OPERATOR_op_sub ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	//args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_3Polynomial( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_3Polynomial( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_3Polynomial( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_2Polynomial( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_2Polynomial( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_2Polynomial( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_2RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_2RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_2RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_COMPLEX ) );

	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_ABS_SET ) );

	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_sub;
	smbtable->Add( toadd );
	//=======================
	//=======================
	
	//=== FORMULA_center_dot ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_center_dot;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_bullet ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_bullet;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_lang ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_lang;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_rang ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_rang;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_identical ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_identical;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_almost_equal ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_almost_equal;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_proportional ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_proportional;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_therefore ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_therefore;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_exist ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_exist;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_forall ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_forall;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_times ===
	toadd.name = ::ml_Op2String( FORMULA_times );
	//=== OPERATOR_op_mul ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_3Polynomial( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_3Polynomial( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_3Polynomial( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_3RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_3Vector( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_GROUP_STRING ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Set() ), 
		typetable.makeDataTypeNode( new CDsrDataType_Set() ), 
		typetable.makeDataTypeNode( new CDsrDataType_Set() ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_mul;
	smbtable->Add( toadd );
	//=======================
	//=======================
	
	//=== FORMULA_in ===
	toadd.name = ::ml_Op2String( FORMULA_in );
	//=======================
	
	//=== FORMULA_not_in ===
	toadd.name = ::ml_Op2String( FORMULA_not_in );
	//=======================
	
	//=== FORMULA_sadd ===
	toadd.name = ::ml_Op2String( FORMULA_sadd );
	//=======================
	
	//=== FORMULA_smul ===
	toadd.name = ::ml_Op2String( FORMULA_smul );
	//=======================
	
	//=== FORMULA_set_in ===
	toadd.name = ::ml_Op2String( FORMULA_set_in );
	//=======================
	
	//=== FORMULA_set_in_inv ===
	toadd.name = ::ml_Op2String( FORMULA_set_in_inv );
	//=======================
	
	//=== FORMULA_set_in_eq ===
	toadd.name = ::ml_Op2String( FORMULA_set_in_eq );
	//=======================
	
	//=== FORMULA_set_in_eq_inv ===
	toadd.name = ::ml_Op2String( FORMULA_set_in_eq_inv );
	//=======================
	
	//=== FORMULA_set_not_in ===
	toadd.name = ::ml_Op2String( FORMULA_set_not_in );
	//=======================
	
	//=== FORMULA_empty_frame ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_empty_frame;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_sup ===
	toadd.name = ::ml_Op2String( FORMULA_sup );
	//=== OPERATOR_op_pow ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_GROUP_STRING, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_GROUP_STRING ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_REAL, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_pow;
	smbtable->Add( toadd );
	//=======================
	//=======================
	
	//=== FORMULA_sub ===
	toadd.name = ::ml_Op2String( FORMULA_sub );
	//=== OPERATOR_square_brackets ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	//
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_square_brackets;
	smbtable->Add( toadd );
	//=======================
	//=======================
	
	//=== FORMULA_subsup ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_subsup;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_over ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_over;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_under ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_under;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== FORMULA_underover ===
	// vasya : unknown special operator
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_UNKNOWN ) );
	toadd.param1 = FORMULA_underover;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_table ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_VECTOR ) );
	toadd.setDataType( args );
	toadd.param1 = FORMULA_table;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_transpose ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	//
	// 0
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	// 1
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	// 2
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	// 3
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	// 4
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = FORMULA_transpose;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_prime ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = FORMULA_prime;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
}

///////////////////////////////////////////////////////////////////////////////
