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

void ml_InitOperatorSmbTable( CParseDsrSymbolTable *smbtable )
{
	CParseDsrSymbol toadd( _T(""), SMBTABLE_TYPE_FUNCTION );
	CDsrDataTypeTable& typetable = smbtable->getTypeTable();
	CDsrDataType_Composition *args, *args1;
	CDsrDataType_Node *resnode;

	//=== OPERATOR_op_setadd ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_ABS_SET ) );
	toadd.param1 = OPERATOR_op_setadd;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_setmul ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_ABS_SET ) );
	toadd.param1 = OPERATOR_op_setmul;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_in ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_BOOL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_STRING, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RANGE, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_GROUP_STRING, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_GROUP, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_rel_in;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_not_in ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_BOOL, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_STRING, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RANGE, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_GROUP_STRING, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_GROUP, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_rel_not_in;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_set_in ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_rel_set_in;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_set_in_inv ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_rel_set_in_inv;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_set_in_eq ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_rel_set_in_eq;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_set_in_eq_inv ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_rel_set_in_eq_inv;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_set_not_in ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_ABS_SET, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_rel_set_not_in;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_add ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	//
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
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_STRING ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
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
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeFunctionNode_2Vector( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_add;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

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
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

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
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

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
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_mod ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_mod;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

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
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_l ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_BOOL ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_rel_l;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_le ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_BOOL ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_rel_le;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_g ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_BOOL ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_rel_g;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_rel_ge ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_BOOL ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_rel_ge;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_bool_and ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_bool_and;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_bool_or ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_bool_or;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_bool_not ===
	toadd.setDataType( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_BOOL ) );
	toadd.param1 = OPERATOR_op_bool_not;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_compare_ne ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_BOOL, DSRDATA_TYPE_BOOL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_STRING, DSRDATA_TYPE_STRING, DSRDATA_TYPE_BOOL ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_compare_ne;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_compare_e ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_REAL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_COMPLEX, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_BOOL, DSRDATA_TYPE_BOOL, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_STRING, DSRDATA_TYPE_STRING, DSRDATA_TYPE_BOOL ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) ) );
	//
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_compare_e;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_type_spec ===
	toadd.setDataType( ::__ml_makeBinaryOpRecord( typetable, DSRDATA_TYPE_IGNORE ) );
	toadd.param1 = OPERATOR_op_type_spec;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_assign ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_COMPLEX ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_STRING ) );
	//
	args->Add( ::__ml_makeAssignNode_Polynomial( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode_Polynomial( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode_Polynomial( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeAssignNode_RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode_RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode_RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_GROUP_STRING ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_ABS_GROUP ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_ABS_SET ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_assign;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_INTEGER ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_INTEGER ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_INTEGER;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_BITS ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_BITS ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_BITS;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_RATIONAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_RATIONAL ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_RATIONAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_REAL ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_NATURAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_BITS ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_BITS ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_BITS;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_RATIONAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_RATIONAL ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_RATIONAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_REAL ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_INTEGER, DSRDATA_TYPE_COMPLEX ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_2_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_REAL ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_2_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_RATIONAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_REAL_2_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, DSRDATA_TYPE_REAL, DSRDATA_TYPE_COMPLEX ) );
	toadd.param1 = OPERATOR_op_convert_REAL_2_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_POLYNOM_INTEGER ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_POLYNOM_INTEGER;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_NATURAL_2_POLYNOM_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_POLYNOM_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_NATURAL_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_INTEGER ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_INTEGER;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_INTEGER_2_POLYNOM_INTEGER ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_POLYNOM_INTEGER;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_INTEGER_2_POLYNOM_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_POLYNOM_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_INTEGER_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_INTEGER ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_INTEGER;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_RATIONAL_2_POLYNOM_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_POLYNOM_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_RATIONAL_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_REAL_2_POLYNOM_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_REAL_2_POLYNOM_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_REAL_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_REAL_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_REAL_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_REAL_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_REAL_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_REAL_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_COMPLEX_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_COMPLEX_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
	
	//=== OPERATOR_op_convert_COMPLEX_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_COMPLEX_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_INTEGER ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_INTEGER;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_COMPLEX_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_COMPLEX_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_POLYNOM_REAL_2_POLYNOM_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_POLYNOM_REAL_2_POLYNOM_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_REAL ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_REAL;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_FUN_REAL_2_RATIONAL_FUN_COMPLEX ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_FUN_REAL_2_RATIONAL_FUN_COMPLEX;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_INTEGER_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_INTEGER_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_RATIONAL_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_RATIONAL_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_REAL_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_REAL_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_NATURAL_2_COMPLEX_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_NATURAL_2_COMPLEX_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_RATIONAL_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_RATIONAL_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_REAL_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_REAL_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_INTEGER_2_COMPLEX_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_INTEGER_2_COMPLEX_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_2_REAL_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_REAL_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_RATIONAL_2_COMPLEX_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_RATIONAL_2_COMPLEX_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_convert_REAL_2_COMPLEX_VECTOR ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	toadd.param1 = OPERATOR_op_convert_REAL_2_COMPLEX_VECTOR;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_round_brackets ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_COMPLEX ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_STRING ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_RationalFun( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ) ) );
	//
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_RANGE ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_GROUP_STRING ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_ABS_GROUP ) );
	args->Add( ::__ml_makeUnaryOpRecord( typetable, DSRDATA_TYPE_ABS_SET ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_round_brackets;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

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
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_curly_brackets ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( DSRDATA_TYPE_COMPOSITION );
	args1 = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args1->Add( typetable.makeDataTypeNode( DSRDATA_TYPE_IGNORE ) );
	args->Add( args1 );
	resnode = typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET );
	toadd.setDataType( typetable.makeDataTypeNode( new CDsrDataType_Function( args, resnode ) ) );
	toadd.param1 = OPERATOR_curly_brackets;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_create_x ===
	toadd.setDataType( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_IGNORE ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_IGNORE ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	toadd.param1 = OPERATOR_op_create_x;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_equation_e ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_GROUP_STRING ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_GROUP_STRING ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_equation_e;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_equation_l ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_equation_l;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_equation_le ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_equation_le;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_equation_g ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_equation_g;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_equation_ge ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ), 
		typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );
	args->Add( ::__ml_makeFunctionNode( typetable, 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ), 
		typetable.makeDataTypeNode( new CDsrDataType_Vector( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) ),
		typetable.makeDataTypeNode( DSRDATA_TYPE_NONE ) ) );

	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_equation_ge;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_op_return ===
	args = (CDsrDataType_Composition*)typetable.makeDataTypeNode( new CDsrDataType_Composition() );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_COMPLEX ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_BOOL ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_STRING ) );
	//
	args->Add( ::__ml_makeAssignNode_Polynomial( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode_Polynomial( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode_Polynomial( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeAssignNode_RationalFun( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode_RationalFun( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode_RationalFun( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_NATURAL ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_INTEGER ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_RATIONAL ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_REAL ) );
	args->Add( ::__ml_makeAssignNode_Vector( typetable, DSRDATA_TYPE_COMPLEX ) );
	//
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_RANGE ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_GROUP_STRING ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_ABS_GROUP ) );
	args->Add( ::__ml_makeAssignNode( typetable, DSRDATA_TYPE_ABS_SET ) );
	toadd.setDataType( args );
	toadd.param1 = OPERATOR_op_return;
	toadd.name = ::ml_Op2String( toadd.param1 );
	smbtable->Add( toadd );
	//=======================
}

///////////////////////////////////////////////////////////////////////////////
