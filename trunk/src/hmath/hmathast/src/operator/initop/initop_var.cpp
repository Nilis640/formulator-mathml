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

#include "../datalink.h"
#include "../operator.h"
#include "../smbtbllink.h"

///////////////////////////////////////////////////////////////////////////////

void ml_InitConstSmbTable( CParseDsrSymbolTable *smbtable )
{
	CParseDsrSymbol toadd( _T(""), SMBTABLE_TYPE_VARIABLE );
	CDsrDataTypeTable& typetable = smbtable->getTypeTable();
	CDsrDataType_Set *snode;
	CDsrDataType_Polynomial *pnode;

	//=== FORMULA_set_empty ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_ABS_SET ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_set_empty;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== CONST_true ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = CONST_true;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== CONST_false ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_BOOL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = CONST_false;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== CONST_TIME ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = CONST_TIME;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_exponentiale ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_exponentiale;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_eulergamma ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_eulergamma;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_imaginaryi ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_imaginaryi;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_pi ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_pi;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_p_g ===
	toadd.setDataType( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_pi;
	toadd.name = ::ml_Op2String( FORMULA_p_g );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_integers ===
	snode = (CDsrDataType_Set*)typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	toadd.setDataType( snode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_integers;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_reals ===
	snode = (CDsrDataType_Set*)typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	toadd.setDataType( snode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_reals;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_rationals ===
	snode = (CDsrDataType_Set*)typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( DSRDATA_TYPE_RATIONAL ) ) );
	toadd.setDataType( snode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_rationals;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_naturalnumbers ===
	snode = (CDsrDataType_Set*)typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( DSRDATA_TYPE_NATURAL ) ) );
	toadd.setDataType( snode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_naturalnumbers;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== FORMULA_complexes ===
	snode = (CDsrDataType_Set*)typetable.makeDataTypeNode( new CDsrDataType_Set( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	toadd.setDataType( snode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = FORMULA_complexes;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== OPERATOR_VAR_ask ===
	toadd.setDataType( smbtable->makeDefaultDataTypeNode() );
	toadd.getDataType()->setXValue();
	toadd.getDataType()->setLValue();
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = OPERATOR_VAR_ask;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== CONST_polynom_x_integer ===
	pnode = (CDsrDataType_Polynomial*)typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_INTEGER ) ) );
	toadd.setDataType( pnode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = CONST_polynom_x_integer;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== CONST_polynom_x_real ===
	pnode = (CDsrDataType_Polynomial*)typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_REAL ) ) );
	toadd.setDataType( pnode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = CONST_polynom_x_real;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================

	//=== CONST_polynom_x_complex ===
	pnode = (CDsrDataType_Polynomial*)typetable.makeDataTypeNode( new CDsrDataType_Polynomial( typetable.makeDataTypeNode( DSRDATA_TYPE_COMPLEX ) ) );
	toadd.setDataType( pnode );
	toadd.param1 = VARIABLE_USER;
	toadd.param2 = smbtable->getNewVarNo();
	toadd.param3 = CONST_polynom_x_complex;
	toadd.name = ::ml_Op2String( toadd.param3 );
	smbtable->Add( toadd );
	//=======================
}

void ml_InitConstSmbTable_Value( CParseDsrSymbolTable& smbtable, UniWordVector& var_tbl )
{
	CParseDsrSymbol *smb;
	for( long i = 0; i < smbtable.getSymbolNumber(); i++ )
	{
		if( !(smb = smbtable.getSymbol( i )) ) continue;
		if( smb->smb_record_type == SMBTABLE_TYPE_VARIABLE && smb->param3 )
		{
			switch( smb->param3 )
			{
			case FORMULA_set_empty:
			case FORMULA_integers:
			case FORMULA_reals:
			case FORMULA_rationals:
			case FORMULA_naturalnumbers:
			case FORMULA_complexes:
				var_tbl[ smb->param2 ] = smbtable.makeUniWord( new MMD_Set( smbtable ) );
				break;
			case CONST_true:
				var_tbl[ smb->param2 ] = CDSRInteger( 1 );
				var_tbl[ smb->param2 ].setType( DSRDATA_TYPE_BOOL );
				break;
			case CONST_false:
				var_tbl[ smb->param2 ] = CDSRInteger( 0 );
				var_tbl[ smb->param2 ].setType( DSRDATA_TYPE_BOOL );
				break;
			case CONST_TIME:
				var_tbl[ smb->param2 ] = CDSRReal( 0.0 );
				break;
			case FORMULA_exponentiale:
				var_tbl[ smb->param2 ] = CDSRReal( 2.718281828 );
				break;
			case FORMULA_eulergamma:
				var_tbl[ smb->param2 ] = CDSRReal( 0.5772156649 );
				break;
			case FORMULA_pi:
			case FORMULA_p_g:
				var_tbl[ smb->param2 ] = CDSRReal( 3.1415926535897932384626433832795 );
				break;
			case FORMULA_imaginaryi:
				var_tbl[ smb->param2 ] = CDSRComplex( 0.0, 1.0 );
				break;
			case CONST_polynom_x_integer:
				var_tbl[ smb->param2 ] = smbtable.makeUniWord( new MMD_Polynomial( smbtable, _T("x"), DSRDATA_TYPE_INTEGER ) );
				break;
			case CONST_polynom_x_real:
				var_tbl[ smb->param2 ] = smbtable.makeUniWord( new MMD_Polynomial( smbtable, _T("x"), DSRDATA_TYPE_REAL ) );
				break;
			case CONST_polynom_x_complex:
				var_tbl[ smb->param2 ] = smbtable.makeUniWord( new MMD_Polynomial( smbtable, _T("x"), DSRDATA_TYPE_COMPLEX ) );
				break;
			}
		}		
	}
}

///////////////////////////////////////////////////////////////////////////////
