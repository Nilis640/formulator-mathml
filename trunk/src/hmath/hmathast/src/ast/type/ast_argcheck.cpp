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

#include "../astdsr.h"

///////////////////////////////////////////////////////////////////////////

/*
 * Returns: -1 - do not match, 
 *           0 - match (without arg type conversions),
 *          >0 - match (<type_conversion> contains enum CDsrFunctionListID for conversion)
 */
long getFunctionArgMatch( long op_id, 
						  CDsrDataType_Function *funt, 
						  CDsrDataType_Composition *argt, 
						  CDsrDataType_Node*& result, 
						  __CDSRTypeConversionOpVector& type_conversion )
{
	if( !funt->n_arg ) return -1;

	CDsrDataType_Composition *n_arg = (CDsrDataType_Composition*)(funt->n_arg);
	if( n_arg->getChildNumber() != argt->getChildNumber() )
		return -1;

	enum CDsrFunctionListID rest;
	long cb = 0;

	type_conversion.Flush();
	for( long i = 0; i < argt->getChildNumber(); i++ )
	{
		if( !n_arg->at( i ) || !argt->at( i ) )
			return -1; // doesn't match
		if( !( !n_arg->at( i )->isLValue() || argt->at( i )->isLValue() ) )
			return -1; // doesn't match: can't assign expr to not l-value
		if( !n_arg->at( i )->compare( argt->at( i ) ) )
		{
			if( op_id == OPERATOR_op_assign && i == 0 && n_arg->at( i )->isLValue() )
				return -1; //  doesn't match: can't convert left arg of the assignment
			rest = ::getTypeConvOp( n_arg->at( i )/*->getType()*/, 
									argt->at( i )/*->getType()*/ );
			if( rest == FUNCTION_STD_NONE )
				return -1; //  doesn't match: can't convert
			cb++;
		}
		else
			rest = FUNCTION_STD_NONE;
		type_conversion.Add( rest );
	}

	result = funt->n_res;

	//=== individual checks
	switch( argt->getChildNumber() )
	{
	case 1:
		if( argt->at( 0 )->getType() == DSRDATA_TYPE_VECTOR )
		{
			CDsrDataType_Vector *larg = (CDsrDataType_Vector*)argt->at( 0 );
			switch( op_id )
			{
			case FUNCTION_std_gcd:
			case FUNCTION_std_lcm:
				if( larg->getRows() > 1 && larg->getColumns() > 1 )
					return -1;
/*				lev 01.05.2006
				if( larg->getRows() != 1 && larg->getColumns() != 1 )
					return -1;*/
				break;
			case FUNCTION_std_abs:
			case FUNCTION_std_det:
			case FUNCTION_std_inverse:
				if( larg->getRows() != larg->getColumns() )
					return -1;
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
					((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
				}
				break;
			case FUNCTION_std_eigenvals:
				if( larg->getRows() != larg->getColumns() )
					return -1;
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setColumns( 1 );
					((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
				}
				break;
			case OPERATOR_op_add:
			case OPERATOR_op_sub:
			case OPERATOR_round_brackets:
			case OPERATOR_op_convert_NATURAL_2_INTEGER_VECTOR:
			case OPERATOR_op_convert_NATURAL_2_RATIONAL_VECTOR:
			case OPERATOR_op_convert_NATURAL_2_REAL_VECTOR:
			case OPERATOR_op_convert_NATURAL_2_COMPLEX_VECTOR:
			case OPERATOR_op_convert_INTEGER_2_RATIONAL_VECTOR:
			case OPERATOR_op_convert_INTEGER_2_REAL_VECTOR:
			case OPERATOR_op_convert_INTEGER_2_COMPLEX_VECTOR:
			case OPERATOR_op_convert_RATIONAL_2_REAL_VECTOR:
			case OPERATOR_op_convert_RATIONAL_2_COMPLEX_VECTOR:
			case OPERATOR_op_convert_REAL_2_COMPLEX_VECTOR:
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
					((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
				}
				break;
			case FORMULA_transpose:
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setRows( larg->getColumns() );
					((CDsrDataType_Vector*)result)->setColumns( larg->getRows() );
				}
				break;
			}
		}
		break;
	case 2:
	case 3:
		if( argt->at( 0 )->getType() == DSRDATA_TYPE_VECTOR && 
			argt->at( 1 )->getType() == DSRDATA_TYPE_VECTOR )
		{
			CDsrDataType_Vector *larg = (CDsrDataType_Vector*)argt->at( 0 );
			CDsrDataType_Vector *rarg = (CDsrDataType_Vector*)argt->at( 1 );
			/*if( !larg->getRows() || !rarg->getRows() ||
				!larg->getColumns() || !rarg->getColumns() )
				return -1;*/
			switch( op_id )
			{
			case FUNCTION_std_lsolve:
				if( larg->getColumns() != larg->getRows() )
					return -1;
				if( rarg->getRows() == 1 )
				{
					if( rarg->getColumns() != larg->getRows() )
						return -1;
				}
				else
				{
					if( rarg->getColumns() == 1 )
					{
						if( rarg->getRows() != larg->getRows() )
							return -1;
					}
					else
						return -1;
				}
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setColumns( 1 );
					((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
				}
				break;
			case OPERATOR_op_mul:
				if( (larg->getRows() == 1 && 
					 rarg->getRows() == 1 &&
					 larg->getColumns() == rarg->getColumns()) ||
					(larg->getColumns() == 1 && 
					 rarg->getColumns() == 1 &&
					 larg->getRows() == rarg->getRows()) )
				{
					if( result->getType() == DSRDATA_TYPE_VECTOR )
						return -1;
				}
				else
				{
					if( larg->getColumns() != rarg->getRows() )
						return -1;
					else
					{
						if( result->getType() != DSRDATA_TYPE_VECTOR )
							return -1;
						((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
						((CDsrDataType_Vector*)result)->setColumns( rarg->getColumns() );
					}
				}
				break;
			case OPERATOR_op_add:
			case OPERATOR_op_sub:
			case OPERATOR_op_compare_e:
			case OPERATOR_op_compare_ne:
			case OPERATOR_op_assign:
				if( larg->getRows() != rarg->getRows() ||
					larg->getColumns() != rarg->getColumns() )
					return -1;
				else
				{
					if( result->getType() == DSRDATA_TYPE_VECTOR )
					{
						((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
						((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
					}
				}
				break;
			}
		}
		else if( argt->at( 0 )->getType() == DSRDATA_TYPE_VECTOR )
		{
			CDsrDataType_Vector *larg = (CDsrDataType_Vector*)argt->at( 0 );
			/*if( !larg->getRows() || !larg->getColumns() )
				return -1;*/
			switch( op_id )
			{
			case FUNCTION_std_eigenvals:
				if( larg->getRows() != larg->getColumns() )
					return -1;
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setColumns( 1 );
					((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
				}
				break;
			case OPERATOR_square_brackets:
				if( larg->getRows() == 0 || larg->getColumns() == 0 )
					return -1;	// vasya : smart report
				else if( larg->getRows() == 1 || larg->getColumns() == 1 )
				{
					if( result->getType() == DSRDATA_TYPE_VECTOR )
						return -1;	// vector[] -> scalar
					result->setLValue();
				}
				else
				{
					if( result->getType() != DSRDATA_TYPE_VECTOR )
						return -1;
					((CDsrDataType_Vector*)result)->setRows( 1 );
					((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
				}
				break;
			case OPERATOR_op_div:
			case OPERATOR_op_mul:
				if( result->getType() == DSRDATA_TYPE_VECTOR )
				{
					((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
					((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
				}
				break;
			case OPERATOR_op_pow:
				if( larg->getRows() != larg->getColumns() )
					return -1;
				else
				{
					if( result->getType() == DSRDATA_TYPE_VECTOR )
					{
						((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
						((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
					}
				}
				break;
			}
		}
		else if( argt->at( 1 )->getType() == DSRDATA_TYPE_VECTOR && 
			result->getType() == DSRDATA_TYPE_VECTOR )
		{
			CDsrDataType_Vector *larg = (CDsrDataType_Vector*)argt->at( 1 );
			/*if( !larg->getRows() || !larg->getColumns() )
				return -1;*/
			switch( op_id )
			{
			case OPERATOR_op_mul:
				((CDsrDataType_Vector*)result)->setRows( larg->getRows() );
				((CDsrDataType_Vector*)result)->setColumns( larg->getColumns() );
				break;
			}
		}
		break;
	}
	return cb;
}

long getFunctionArgMatch( long op_id, 
						  CDsrDataType_Composition *funt, 
						  CDsrDataType_Composition *argt, 
						  CDsrDataType_Node*& result, 
						  std::vector<__CDSRTypeConversionOpVector>& opc, long& _cb )
{
	if( funt->getChildNumber() == 0 ) return -1;
	long min_cb_idx = -1, min_cb = -1, cb = -1;
	opc.resize( funt->getChildNumber() );
	result = 0;
	CDsrDataType_Node *tmp_res;
	for( long i = 0; i < funt->getChildNumber(); i++ )
	{
		if( funt->at( i ) && 
			funt->at( i )->getType() == DSRDATA_TYPE_FUNCTION &&
		    (cb = getFunctionArgMatch( op_id, (CDsrDataType_Function*)funt->at( i ), argt, tmp_res, opc[ i ] )) != -1 )
		{
			if( min_cb == -1 || cb < min_cb )
			{
				min_cb = cb;
				min_cb_idx = i;
				result = tmp_res;
				if( min_cb == 0 ) break;
			}
		}
	}
	_cb = min_cb;
	return min_cb_idx;
}

CDsrDataType_Node* CompareOp2Args(	long op_id, CDsrDataType_Node* op_type, 
									//enum CDsrFunctionListID& fun_id,
									long& fun_id_param,
									CDsrDataType_Composition* args_type,
									std::vector<__CDSRTypeConversionOpVector>& opc,
									long& cb,
									CParseDsrSymbolTable& smbtable,
                                                                        CParseDsrSymbol * /*smb*/ )
{
	CDsrDataType_Node *ret = 0;
	switch( op_type->getType() )
	{
	case DSRDATA_TYPE_COMPOSITION:
		fun_id_param = getFunctionArgMatch( 
						op_id,
						(CDsrDataType_Composition*)op_type,
						args_type,
						ret,
						opc, cb );
		if( fun_id_param != -1 )
		{
			if( !ret ) ret = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR );
			return ret;
		}
		break;
	case DSRDATA_TYPE_FUNCTION:
		fun_id_param = 0;
		opc.resize( 1 );
		cb = getFunctionArgMatch( 
					op_id,
					(CDsrDataType_Function*)op_type, 
					args_type,
					ret,
					opc[ fun_id_param ] );
		if( cb != -1 )
		{
			if( !ret ) ret = smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR );
			return ret;
		}
		break;
	default:
		break;
	}
	return smbtable.getTypeTable().makeDataTypeNode( DSRDATA_TYPE_ERROR );
}

enum CDsrDataTypes convert2MaxScalarType( CParseDsrSymbolTable& smbtable, CDsrAST_ArgList *args )
{
	enum CDsrFunctionListID rest;
	enum CDsrDataTypes maxt = DSRDATA_TYPE_RESERVED;

	long i;
	for( i = 0; i < args->getArgNumber(); i++ )
	{
		if( !args->expr[ i ] || !args->expr[ i ]->getDataType() )
			return DSRDATA_TYPE_ERROR;
		if( args->expr[ i ]->getDataType()->isScalar() )
		{
			if( maxt < args->expr[ i ]->getDataType()->getType() )
			{
				//ret = args->expr[ i ]->getDataType();
				//maxt = ret->getType();
				maxt = args->expr[ i ]->getDataType()->getType();
			}
		}
		else
			return DSRDATA_TYPE_ERROR;
	}
	if( maxt == DSRDATA_TYPE_NATURAL ) maxt = DSRDATA_TYPE_INTEGER;
	else if( maxt == DSRDATA_TYPE_RATIONAL ) maxt = DSRDATA_TYPE_REAL;
	for( i = 0; i < args->getArgNumber(); i++ )
	{
		if( args->expr[ i ]->getDataType()->getType() != maxt )
		{
			rest = ::getTypeConvOp( maxt, args->expr[ i ]->getDataType()->getType() );
			if( rest == FUNCTION_STD_NONE )
				return DSRDATA_TYPE_ERROR;
			args->convertArgTypes( smbtable, i, rest );
		}
	}
	
	return maxt;
}

CDsrDataType_Node* correctOpDataType2New( CParseDsrSymbolTable& smbtable, long new_fun_id, CDsrAST_Op *op )
{
	CDsrDataType_Node *op_type = 0;
	QString opname = _T("");
	if( new_fun_id == op->id )
		op_type = op->getDataType();
	else
	{
		opname = ::ml_Op2String( new_fun_id );
		long *pid = smbtable.is_find( opname );
		if( pid )
		{
			CParseDsrSymbol *opsmb = smbtable.getSymbol( *pid );
			if( opsmb )
				op_type = opsmb->getDataType();
		}
	}
	return op_type;
}

///////////////////////////////////////////////////////////////////////////////
