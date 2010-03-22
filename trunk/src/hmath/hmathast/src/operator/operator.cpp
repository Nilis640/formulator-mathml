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

#include "../type/typetable.h"
#include "../symbol/symbol_table_dsr.h"
#include "operator.h"

QString CDsrFunctionListName[ FUNCTION_STD_LAST ] =
{








	_T("no operation"),
	_T("@sadd"),
	_T("@smul"),
	_T("@in"),
	_T("@not_in"),
	_T("@set_in"),
	_T("@set_in_inv"),
	_T("@set_in_eq"),
	_T("@set_in_eq_inv"),
	_T("@set_not_in"),
	_T("()"),
	_T("[]"),
	_T("l-value []"),
	_T("{}"),
	_T("+"),
	_T("-"),
	_T("*"),
	_T("/"),
	_T("%"),
	_T("^"),
	_T("<"),
	_T("<="),
	_T(">"),
	_T(">="),
	_T("&"),
	_T("|"),
	_T("!"),
	_T("!="),
	_T("="),
	_T("return a value"),
	_T("independent variable"),	// OPERATOR_op_create_x,
	_T("equation ="),	// OPERATOR_op_equation_e,
	_T("equation <"),	// OPERATOR_op_equation_l,
	_T("equation <="),	// OPERATOR_op_equation_le,
	_T("equation >"),	// OPERATOR_op_equation_g,
	_T("equation >="),	// OPERATOR_op_equation_ge,
	_T("=="),
	_T("type specification"),
	_T("NATURAL->INTEGER"),
	_T("NATURAL->BITS"),
	_T("NATURAL->RATIONAL"),
	_T("NATURAL->REAL"),
	_T("NATURAL->COMPLEX"),
	_T("INTEGER->BITS"),
	_T("INTEGER->RATIONAL"),
	_T("INTEGER->REAL"),
	_T("INTEGER->COMPLEX"),
	_T("RATIONAL->REAL"),
	_T("RATIONAL->COMPLEX"),
	_T("REAL->COMPLEX"),
	_T("NATURAL->POLYNOM_INTEGER"),
	_T("NATURAL->POLYNOM_REAL"),
	_T("NATURAL->POLYNOM_COMPLEX"),
	_T("NATURAL->RATIONAL_FUN_INTEGER"),
	_T("NATURAL->RATIONAL_FUN_REAL"),
	_T("NATURAL->RATIONAL_FUN_COMPLEX"),
	_T("INTEGER->POLYNOM_INTEGER"),
	_T("INTEGER->POLYNOM_REAL"),
	_T("INTEGER->POLYNOM_COMPLEX"),
	_T("INTEGER->RATIONAL_FUN_INTEGER"),
	_T("INTEGER->RATIONAL_FUN_REAL"),
	_T("INTEGER->RATIONAL_FUN_COMPLEX"),
	_T("RATIONAL->POLYNOM_REAL"),
	_T("RATIONAL->POLYNOM_COMPLEX"),
	_T("RATIONAL->RATIONAL_FUN_REAL"),
	_T("RATIONAL->RATIONAL_FUN_COMPLEX"),
	_T("REAL->POLYNOM_REAL"),
	_T("REAL->POLYNOM_COMPLEX"),
	_T("REAL->RATIONAL_FUN_REAL"),
	_T("REAL->RATIONAL_FUN_COMPLEX"),
	_T("COMPLEX->POLYNOM_COMPLEX"),
	_T("COMPLEX->RATIONAL_FUN_COMPLEX"),
	_T("POLYNOM_INTEGER->POLYNOM_REAL"),
	_T("POLYNOM_INTEGER->POLYNOM_COMPLEX"),
	_T("POLYNOM_REAL->POLYNOM_COMPLEX"),
	_T("POLYNOM_INTEGER->RATIONAL_FUN_INTEGER"),
	_T("POLYNOM_INTEGER->RATIONAL_FUN_REAL"),
	_T("POLYNOM_INTEGER->RATIONAL_FUN_COMPLEX"),
	_T("POLYNOM_REAL->RATIONAL_FUN_REAL"),
	_T("POLYNOM_REAL->RATIONAL_FUN_COMPLEX"),
	_T("POLYNOM_COMPLEX->RATIONAL_FUN_COMPLEX"),
	_T("RATIONAL_FUN_INTEGER->RATIONAL_FUN_REAL"),
	_T("RATIONAL_FUN_INTEGER->RATIONAL_FUN_COMPLEX"),
	_T("RATIONAL_FUN_REAL->RATIONAL_FUN_COMPLEX"),
	_T("NATURAL->INTEGER_VECTOR"),
	_T("NATURAL->RATIONAL_VECTOR"),
	_T("NATURAL->REAL_VECTOR"),
	_T("NATURAL->COMPLEX_VECTOR"),
	_T("INTEGER->RATIONAL_VECTOR"),
	_T("INTEGER->REAL_VECTOR"),
	_T("INTEGER->COMPLEX_VECTOR"),
	_T("RATIONAL->REAL_VECTOR"),
	_T("RATIONAL->COMPLEX_VECTOR"),
	_T("REAL->COMPLEX_VECTOR"),
	_T("abs"),
	_T("arch"),
	_T("arccos"),
	_T("arccosec"),
	_T("arcsec"),
	_T("arcsin"),
	_T("arctg"),
	_T("arcth"),
	_T("arsh"),
	_T("ch"),
	_T("cos"),
	_T("cosec"),
	_T("cosech"),
	_T("ctg"),
	_T("cth"),
	_T("sec"),
	_T("sech"),
	_T("sin"),
	_T("sh"),
	_T("sqrt"),
	_T("tg"),
	_T("th"),
	_T("exp"),
	_T("int"),
	_T("lg"),
	_T("ln"),
	_T("log"),
	_T("pow"),
	_T("root"),
	_T("atan2"),
	_T("fmod"),
	_T("polar"),
	_T("ceil"),
	_T("floor"),
	_T("imag"),
	_T("pow10"),
	_T("real"),
	_T("random"),
	_T("identity"),

	_T("primes"),
	_T("isprime"),
	_T("gcd"),
	_T("lcm"),

	_T("polyroots"),
	_T("polyder"),
	_T("polyint"),
	_T("polygcd"),
	_T("polylcm"),
	_T("lsolve"),
	_T("ratfpart"),
	_T("ratfder"),
	_T("det"),
	_T("inverse"),
	_T("eigenvals"),
	_T("eigenvec"),

	_T("@box_lt"),
	_T("@box_rt"),
	_T("@box_lb"),
	_T("@box_rb"),
	_T("@box"),
	_T("@abs"),
	_T("@norm"),
	_T("@floor"),
	_T("@ceil"),
	_T("@empty_frame"),
	_T("@fraction"),
	_T("@diagonal_fraction"),
	_T("@sqrt"),
	_T("@root"),
	_T("@plusminus"),
	_T("@times"),
	_T("@circle_plus"),
	_T("@circle_times"),
	_T("@minus"),
	_T("@center_dot"),
	_T("@bullet"),
	_T("@lang"),
	_T("@rang"),
	_T("@identical"),
	_T("@almost_equal"),
	_T("@proportional"),
	_T("@therefore"),
	_T("@exist"),
	_T("@forall"),
	_T("@in"),
	_T("@not_in"),
	_T("@sadd"),
	_T("@smul"),
	_T("@set_in"),
	_T("@set_in_inv"),
	_T("@set_in_eq"),
	_T("@set_in_eq_inv"),
	_T("@set_not_in"),
	_T("@set_empty"),
	_T("@sup"),
	_T("@sub"),
	_T("@subsup"),
	_T("@over"),
	_T("@under"),
	_T("@underover"),
	_T("@table"),
	_T("@transpose"),
	_T("@prime"),
	_T("integer polynom x"),
	_T("real polynom x"),
	_T("complex polynom x"),
	_T("true"),
	_T("false"),
	_T("t"),
	_T("@exponentiale"),	// 2.718281828
	_T("@eulergamma"),		// 0.5772156649
	_T("@imaginaryi"),
	_T("@pi"),				// 3.1415926535897932384626433832795
	_T("p$g"),				// 3.1415926535897932384626433832795
	_T("@integers"),
	_T("@reals"),
	_T("@rationals"),
	_T("@naturalnumbers"),
	_T("@complexes"),
	_T("?"),
	_T("global variable"),
	_T("local variable"),
	_T("global variable: index"),
	_T("local variable: index"),
	_T("user call"),
	_T("user call (polynomial)"),
	_T("user call (rational fun)")
};

QString ml_Op2String( long id )
{
	static QString value = _T("");
	if( id >= FUNCTION_STD_NONE && id < FUNCTION_STD_LAST )
		value = CDsrFunctionListName[ id ];
	else
		value = QString("%1").arg( id );
	return value;
};

enum CDsrFunctionListID getTypeConvOp( CDsrDataType_Node *dest, CDsrDataType_Node *src )
{
	if( !src || !dest ) return FUNCTION_STD_NONE;

	enum CDsrDataTypes src_type = src->getType();
	enum CDsrDataTypes dest_type = dest->getType();

	switch( src_type )
	{
	case DSRDATA_TYPE_NATURAL:
		switch( dest_type )
		{
		case DSRDATA_TYPE_INTEGER:
			return OPERATOR_op_convert_NATURAL_2_INTEGER;
		case DSRDATA_TYPE_BITS:
			return OPERATOR_op_convert_NATURAL_2_BITS;
		case DSRDATA_TYPE_RATIONAL:
			return OPERATOR_op_convert_NATURAL_2_RATIONAL;
		case DSRDATA_TYPE_REAL:
			return OPERATOR_op_convert_NATURAL_2_REAL;
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_NATURAL_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				return OPERATOR_op_convert_NATURAL_2_POLYNOM_INTEGER;
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_NATURAL_2_POLYNOM_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_NATURAL_2_POLYNOM_COMPLEX;
			default:
				break;
			}
			break;
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				return OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_INTEGER;
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_COMPLEX;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_INTEGER:
		switch( dest_type )
		{
		case DSRDATA_TYPE_BITS:
			return OPERATOR_op_convert_INTEGER_2_BITS;
		case DSRDATA_TYPE_RATIONAL:
			return OPERATOR_op_convert_INTEGER_2_RATIONAL;
		case DSRDATA_TYPE_REAL:
			return OPERATOR_op_convert_INTEGER_2_REAL;
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_INTEGER_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				return OPERATOR_op_convert_INTEGER_2_POLYNOM_INTEGER;
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_INTEGER_2_POLYNOM_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_INTEGER_2_POLYNOM_COMPLEX;
			default:
				break;
			}
			break;
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				return OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_INTEGER;
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_COMPLEX;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_RATIONAL:
		switch( dest_type )
		{
		case DSRDATA_TYPE_REAL:
			return OPERATOR_op_convert_RATIONAL_2_REAL;
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_RATIONAL_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_RATIONAL_2_POLYNOM_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_RATIONAL_2_POLYNOM_COMPLEX;
			default:
				break;
			}
			break;
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_COMPLEX;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_REAL:
		switch( dest_type )
		{
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_REAL_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_REAL_2_POLYNOM_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_REAL_2_POLYNOM_COMPLEX;
			default:
				break;
			}
			break;
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_REAL:
				return OPERATOR_op_convert_REAL_2_RATIONAL_FUN_REAL;
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_REAL_2_RATIONAL_FUN_COMPLEX;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_COMPLEX:
		switch( dest_type )
		{
		case DSRDATA_TYPE_POLYNOM:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_COMPLEX_2_POLYNOM_COMPLEX;
			default:
				break;
			}
			break;
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !dest->getNestedType() ) return FUNCTION_STD_NONE;
			switch( dest->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_COMPLEX:
				return OPERATOR_op_convert_COMPLEX_2_RATIONAL_FUN_COMPLEX;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_POLYNOM:
		switch( dest_type )
		{
		case DSRDATA_TYPE_POLYNOM:
			if( !src->getNestedType() || !dest->getNestedType() )
				return FUNCTION_STD_NONE;
			switch( src->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_REAL;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_POLYNOM_INTEGER_2_POLYNOM_COMPLEX;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_REAL:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_POLYNOM_REAL_2_POLYNOM_COMPLEX;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !src->getNestedType() || !dest->getNestedType() )
				return FUNCTION_STD_NONE;
			switch( src->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_INTEGER:
					return OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_INTEGER;
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_REAL;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_POLYNOM_INTEGER_2_RATIONAL_FUN_COMPLEX;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_REAL:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_REAL;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_POLYNOM_REAL_2_RATIONAL_FUN_COMPLEX;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_COMPLEX:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_POLYNOM_COMPLEX_2_RATIONAL_FUN_COMPLEX;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	case DSRDATA_TYPE_RATIONAL_FUN:
		switch( dest_type )
		{
		case DSRDATA_TYPE_RATIONAL_FUN:
			if( !src->getNestedType() || !dest->getNestedType() )
				return FUNCTION_STD_NONE;
			switch( src->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_INTEGER:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_REAL;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_RATIONAL_FUN_INTEGER_2_RATIONAL_FUN_COMPLEX;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_REAL:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_RATIONAL_FUN_REAL_2_RATIONAL_FUN_COMPLEX;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	case DSRDATA_TYPE_VECTOR:
		switch( dest_type )
		{
		case DSRDATA_TYPE_VECTOR:
			if( !src->getNestedType() ||
				!dest->getNestedType() )
				return FUNCTION_STD_NONE;
			switch( src->getNestedType()->getType() )
			{
			case DSRDATA_TYPE_NATURAL:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_INTEGER:
					return OPERATOR_op_convert_NATURAL_2_INTEGER_VECTOR;
				case DSRDATA_TYPE_RATIONAL:
					return OPERATOR_op_convert_NATURAL_2_RATIONAL_VECTOR;
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_NATURAL_2_REAL_VECTOR;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_NATURAL_2_COMPLEX_VECTOR;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_INTEGER:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_RATIONAL:
					return OPERATOR_op_convert_INTEGER_2_RATIONAL_VECTOR;
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_INTEGER_2_REAL_VECTOR;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_INTEGER_2_COMPLEX_VECTOR;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_RATIONAL:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_REAL:
					return OPERATOR_op_convert_RATIONAL_2_REAL_VECTOR;
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_RATIONAL_2_COMPLEX_VECTOR;
				default:
					break;
				}
				break;
			case DSRDATA_TYPE_REAL:
				switch( dest->getNestedType()->getType() )
				{
				case DSRDATA_TYPE_COMPLEX:
					return OPERATOR_op_convert_REAL_2_COMPLEX_VECTOR;
				default:
					break;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	return FUNCTION_STD_NONE;
}

enum CDsrFunctionListID getTypeConvOp( enum CDsrDataTypes dest, enum CDsrDataTypes src )
{
	switch( src )
	{
	case DSRDATA_TYPE_NATURAL:
		switch( dest )
		{
		case DSRDATA_TYPE_INTEGER:
			return OPERATOR_op_convert_NATURAL_2_INTEGER;
		case DSRDATA_TYPE_BITS:
			return OPERATOR_op_convert_NATURAL_2_BITS;
		case DSRDATA_TYPE_RATIONAL:
			return OPERATOR_op_convert_NATURAL_2_RATIONAL;
		case DSRDATA_TYPE_REAL:
			return OPERATOR_op_convert_NATURAL_2_REAL;
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_NATURAL_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			return OPERATOR_op_convert_NATURAL_2_POLYNOM_INTEGER;
		case DSRDATA_TYPE_RATIONAL_FUN:
			return OPERATOR_op_convert_NATURAL_2_RATIONAL_FUN_INTEGER;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_INTEGER:
		switch( dest )
		{
		case DSRDATA_TYPE_BITS:
			return OPERATOR_op_convert_INTEGER_2_BITS;
		case DSRDATA_TYPE_RATIONAL:
			return OPERATOR_op_convert_INTEGER_2_RATIONAL;
		case DSRDATA_TYPE_REAL:
			return OPERATOR_op_convert_INTEGER_2_REAL;
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_INTEGER_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			return OPERATOR_op_convert_INTEGER_2_POLYNOM_INTEGER;
		case DSRDATA_TYPE_RATIONAL_FUN:
			return OPERATOR_op_convert_INTEGER_2_RATIONAL_FUN_INTEGER;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_RATIONAL:
		switch( dest )
		{
		case DSRDATA_TYPE_REAL:
			return OPERATOR_op_convert_RATIONAL_2_REAL;
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_RATIONAL_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			return OPERATOR_op_convert_RATIONAL_2_POLYNOM_REAL;
		case DSRDATA_TYPE_RATIONAL_FUN:
			return OPERATOR_op_convert_RATIONAL_2_RATIONAL_FUN_REAL;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_REAL:
		switch( dest )
		{
		case DSRDATA_TYPE_COMPLEX:
			return OPERATOR_op_convert_REAL_2_COMPLEX;
		case DSRDATA_TYPE_POLYNOM:
			return OPERATOR_op_convert_REAL_2_POLYNOM_REAL;
		case DSRDATA_TYPE_RATIONAL_FUN:
			return OPERATOR_op_convert_REAL_2_RATIONAL_FUN_REAL;
		default:
			break;
		}
		break;
	case DSRDATA_TYPE_COMPLEX:
		switch( dest )
		{
		case DSRDATA_TYPE_POLYNOM:
			return OPERATOR_op_convert_COMPLEX_2_POLYNOM_COMPLEX;
		case DSRDATA_TYPE_RATIONAL_FUN:
			return OPERATOR_op_convert_COMPLEX_2_RATIONAL_FUN_COMPLEX;
		default:
			break;
		}
		break;
/*	case DSRDATA_TYPE_POLYNOM:
		switch( dest )
		{
		case DSRDATA_TYPE_RATIONAL_FUN:
			return OPERATOR_op_convert_POLYNOM_2_RATIONAL_FUN;
		}
		break;*/
		default:
			break;
	}
	return FUNCTION_STD_NONE;
}

enum CDsrDataTypes getRetType4Op( enum CDsrFunctionListID op, CParseDsrSymbolTable& smbtable )
{
	CParseDsrSymbol *smb;
	long *pid = smbtable.is_find( ::ml_Op2String( op ) );
	if( pid )
	{
		smb = smbtable.getSymbol( *pid );
		if( smb && smb->getDataType() )
		{
			if( smb->getDataType()->getType() == DSRDATA_TYPE_FUNCTION )
			{
				CDsrDataType_Function *node = (CDsrDataType_Function*)smb->getDataType();
				if( node->n_res )
					return node->n_res->getType();
			}
		}	
	}

	return DSRDATA_TYPE_ERROR;
}
