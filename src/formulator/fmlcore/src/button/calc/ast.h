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

#if !defined( __BULAN_FBAST_H__ )
#define __BULAN_FBAST_H__

#include "HUtils/ihkarray.h"
#include "vmi.h"

enum FBAST_Types
{
	ASTT_None,
	ASTT_Expr,
	ASTT_Assign,
	ASTT_Value,
	ASTT_Variable,
	ASTT_Function,
	ASTT_Compound,
	ASTT_Last
};

class __HERMITECH_FORMULIB__ FBAST_Node
{
protected:
	enum FBAST_Types type;

public:
	long line;

	FBAST_Node()							{ type = ASTT_None; line = -1; }
	enum FBAST_Types getType()				{ return type; }
	void setLine( long ln )					{ line = ln; }
	long getLine()							{ return line; }
	virtual ~FBAST_Node()					{}
	virtual void getInstruction( FBtnVM& vm )	{ vm.Nop( line ); }
	virtual int getInstructionLength()		{ return 1; }
};

class __HERMITECH_FORMULIB__ FBAST_Expr : public FBAST_Node
{
public:
	enum FBtnVM_Operation op;
	FBAST_Node *arg1, *arg2;

	FBAST_Expr( enum FBtnVM_Operation _op, FBAST_Node *_arg1 = 0, FBAST_Node *_arg2 = 0 )
	{
		type = ASTT_Expr;
		op = _op;
		arg1 = _arg1;
		arg2 = _arg2;
	}
	virtual ~FBAST_Expr()
	{
		if( arg1 ) delete arg1;
		if( arg2 ) delete arg2;
	}
	virtual void getInstruction( FBtnVM& vm )
	{
		if( arg1 ) arg1->getInstruction( vm );
		if( arg2 ) arg2->getInstruction( vm );
		if( arg1 || arg2 )
			vm.Op( op, line );
		else
			vm.Nop( line );
	}
	virtual int getInstructionLength()
	{
		return	( arg1 ? arg1->getInstructionLength() : 0 ) +
				( arg2 ? arg2->getInstructionLength() : 0 ) + 1;
	}
};

class __HERMITECH_FORMULIB__ FBAST_Variable : public FBAST_Node
{
public:
	QString var_name;
	QString parent_name;

	FBAST_Variable( QString _parent_name, QString _var_name )
	{
		type = ASTT_Variable;
		var_name = _var_name;
		parent_name = _parent_name;
	}
	virtual void getInstruction( FBtnVM& vm )
	{
		vm.PushVar( parent_name, var_name, line );
	}
	virtual int getInstructionLength()
	{
		return 1;
	}
	virtual ~FBAST_Variable()
	{}
};

class __HERMITECH_FORMULIB__ FBAST_Assign : public FBAST_Node
{
public:
	FBAST_Variable *left;
	FBAST_Expr *right;

	FBAST_Assign( FBAST_Variable *_left = 0, FBAST_Expr *_right = 0 )
	{
		type = ASTT_Assign;
		left = _left;
		right = _right;
	}
	virtual ~FBAST_Assign()
	{
		if( left ) delete left;
		if( right ) delete right;
	}
	virtual void getInstruction( FBtnVM& vm )
	{
		if( left && right )
		{
			right->getInstruction( vm );
			vm.Assign( left->parent_name, left->var_name, line );
		}
	}
	virtual int getInstructionLength()
	{
		if( left && right )
			return right->getInstructionLength() + 1;
		else
			return 0;
	}
};

class __HERMITECH_FORMULIB__ FBAST_Value : public FBAST_Node
{
public:
	int is_integer;
	union
	{
		long	larg;
		double	farg;
	};

	FBAST_Value( int _is_integer = 0 )
	{
		type = ASTT_Value;
		is_integer = _is_integer;
	}
	FBAST_Value( int _is_integer, long *l )
	{
		type = ASTT_Value;
		is_integer = _is_integer;
		larg = *l;
	}
	FBAST_Value( int _is_integer, double *f )
	{
		type = ASTT_Value;
		is_integer = _is_integer;
		farg = *f;
	}
	virtual void getInstruction( FBtnVM& vm )
	{
		if( is_integer )
			vm.PushInt( larg, line );
		else
			vm.PushFloat( farg, line );
	}
	virtual int getInstructionLength()
	{
		return 1;
	}
	virtual ~FBAST_Value()
	{}
};

class __HERMITECH_FORMULIB__ FBAST_Function : public FBAST_Node
{
public:
	enum FBtnVM_Function func;
	FBAST_Expr *n_expr1, *n_expr2;

	FBAST_Function( enum FBtnVM_Function fnc = ASTOF_Last, FBAST_Expr *_expr1 = 0, FBAST_Expr *_expr2 = 0 )
	{
		func = fnc;
		type = ASTT_Function;
		n_expr1 = _expr1;
		n_expr2 = _expr2;
	}
	virtual ~FBAST_Function()
	{
		if( n_expr1 ) delete n_expr1;
		if( n_expr2 ) delete n_expr2;
	}
	virtual void getInstruction( FBtnVM& vm )
	{
		if( n_expr1 ) n_expr1->getInstruction( vm );
		if( n_expr2 ) n_expr2->getInstruction( vm );
		vm.Call( func, line );
	}
	virtual int getInstructionLength()
	{
		return	( n_expr1 ? n_expr1->getInstructionLength() : 1 ) + 
				( n_expr2 ? n_expr2->getInstructionLength() : 1 ) + 1;
	}
};

class __HERMITECH_FORMULIB__ FBAST_Compound : public FBAST_Node
{
public:
	MIArray<FBAST_Node> stmt;

	FBAST_Compound() : 
		stmt(MIArray<FBAST_Node>::LeaveItems, 8, 0, 8)
	{
		type = ASTT_Compound;
	}
	int Add( FBAST_Node *node )
	{
		return stmt.Add( node );
	}
	virtual void getInstruction( FBtnVM& vm )
	{
		for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		{
			FBAST_Node *ptr = stmt[ i ];
			if( ptr )
				ptr->getInstruction( vm );
			else
				vm.Nop( line );
		}
	}
	virtual int getInstructionLength()
	{
		int ret = 0;
		for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		{
			FBAST_Node *ptr = stmt[ i ];
			if( ptr )
				ret += ptr->getInstructionLength();
			else
				ret ++;
		}
		return ret;
	}
	virtual ~FBAST_Compound()
	{
		for( long i = 0; i < (long) stmt.ArrayDim(); i++ )
		{
			FBAST_Node *ptr = stmt[ i ];
			if( ptr ) delete ptr;
		}
	}
};

typedef FBAST_Compound PiCodeASTStorage;

#endif
