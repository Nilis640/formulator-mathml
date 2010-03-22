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

#if !defined( __BULAN_VMI_H__ )
#define __BULAN_VMI_H__

#include "HUtils/imessage.h"

#include "vmi_calc.h"

///////////////////////////////////////////////////////////////////////////////

class __HERMITECH_FORMULIB__ FBtnVM
{
protected:
	QVector<QString> object_name, primitive_name, locals_name, field_name;
	QVector<char>  primitive_assign_table;
	FBtnCalc& commands;
	int is_error;

public:
	DRMessageManager& dmm;
	long dmm_page;

public:
	FBtnVM( FBtnCalc& _commands, DRMessageManager& _dmm, long _dmm_page = 0 );
	~FBtnVM();
	void Reset( FBtnCalc& _commands );

	void debug_print( QString& str );

	// Error checks
	void setError();
	void clearError();
	int isError();
	void CheckAssignTable( QVector<QPair<long, long> >& err_pairs );

	// Variables
	void AddObject( QString o_name, long ln = -1 );
	long getObjectIdx( QString& n );
	long getObjectNumber();
	void AddPrimitive( QString p_name, long ln = -1 );
	QString getPrimitiveName( long i );
	long getPrimitiveIdx( QString& n );
	long getPrimitiveNumber();
	void AddLocal( QString l_name, long ln = -1 );
	int getAddress( QString obj, QString obj_field, long& obj_addr, long& field_addr, long ln = -1 );

	// Code generating
	void FlushCode();
	void Nop( long ln = -1 );
	void Pop( long ln = -1 );
	void Op( enum FBtnVM_Operation op, long ln = -1 );
	void Assign( QString obj_name, QString var_name, long ln = -1 );
	void PushVar( QString obj_name, QString var_name, long ln );
	void PushInt( long arg, long ln );
	void PushFloat( double arg, long ln = -1 );
	void Call( enum FBtnVM_Function vmf, long ln = -1 );
};

///////////////////////////////////////////////////////////////////////////////

inline
FBtnVM::~FBtnVM()
{}

inline
void FBtnVM::debug_print( QString& str )
{
	commands.debug_print( str );
}

inline
void FBtnVM::setError()
{
	is_error = 1;
}

inline
void FBtnVM::clearError()
{
	is_error = 0;
}

inline
int FBtnVM::isError()
{
	return is_error;
}

inline
void FBtnVM::Reset( FBtnCalc& _commands )
{
	commands = _commands;
	object_name.clear();
	primitive_name.clear();
	primitive_assign_table.clear();
	locals_name.clear();
	clearError();
}

inline
void FBtnVM::AddObject( QString o_name, long ln )
{
	static const QString warn_mess = _T("child frame name %1 already exists and will be ignored");
	if( object_name.indexOf( o_name ) != -1 )
		dmm.AddLine( dmm_page, warn_mess, ln, o_name );
	object_name.push_back( o_name );
}

inline
long FBtnVM::getObjectIdx( QString& n )
{
	return object_name.indexOf( n );
}

inline
long FBtnVM::getObjectNumber()
{
	return (long) object_name.size();
}

inline
void FBtnVM::AddPrimitive( QString p_name, long ln )
{
	static const QString warn_mess = _T("primitive name %1 already exists and will be ignored");
	if( primitive_name.indexOf( p_name ) != -1 || 
		object_name.indexOf( p_name ) != -1 )
		dmm.AddLine( dmm_page, warn_mess, ln, p_name );
	primitive_name.push_back( p_name );
	char _t = 0;
	primitive_assign_table.push_back( _t );
}

inline
long FBtnVM::getPrimitiveIdx( QString& n )
{
	return primitive_name.indexOf( n );
}

inline
QString FBtnVM::getPrimitiveName( long i )
{
	return primitive_name.at(i);
}

inline
long FBtnVM::getPrimitiveNumber()
{
	return (long) primitive_name.size();
}

inline
void FBtnVM::AddLocal( QString l_name, long /*ln*/ )
{
	if( locals_name.indexOf( l_name ) == -1 )
	{
		locals_name.push_back( l_name );
		double _t = 0.0;
		commands.locals.push_back( _t );
	}
}

inline
void FBtnVM::FlushCode()
{
	commands.clear();
	clearError();
}

inline
void FBtnVM::Nop( long ln )
{
	commands.push_back( FBtnPiCode(ln, LPC_NONE) );
}

inline
void FBtnVM::Pop( long ln )
{
	commands.push_back( FBtnPiCode(ln, LPC_POP) );
}

inline
void FBtnVM::Op( enum FBtnVM_Operation op, long ln )
{
	commands.push_back( FBtnPiCode(ln, (enum FBtnPiCode_Command)op) );
}

inline
void FBtnVM::Assign( QString obj_name, QString var_name, long ln )
{
	QString warn1 = _T("child frame (%1) position fields are read-only, assignment is ignored");
	long a, b;
	if( obj_name == _T( "" ) ) // local variable
	{
		if( locals_name.indexOf( var_name ) == -1 )
			AddLocal( var_name, ln );
	}

	int ret = getAddress( obj_name, var_name, a, b, ln );
	if( !ret )
	{
		if( isFrameAddress( a ) && !isMargin( b ) )
		{
			Pop( ln );
			dmm.AddLine( dmm_page, warn1, ln, obj_name );
		}
		else
		{
			commands.Add( ln, LPC_ASIGN, a, b );
			if( isPrimitiveAddress( a ) )
			{
				char _t = 1 << b;
				primitive_assign_table[ - a - 1 ] |= _t;
			}
			else
				AddLocal( var_name );
		}
	}
	else
	{
		setError();
		Pop( ln );
	}
}

inline
void FBtnVM::PushInt( long arg, long ln )
{
	commands.Add( ln, LPC_PUSHVAL, double(arg) );
}

inline
void FBtnVM::PushFloat( double arg, long ln )
{
	commands.Add( ln, LPC_PUSHVAL, arg );
}

inline
void FBtnVM::Call( enum FBtnVM_Function vmf, long ln )
{
	commands.Add( ln, (enum FBtnPiCode_Command)vmf );
}

inline
void FBtnVM::PushVar( QString obj_name, QString var_name, long ln )
{
	long a, b;
	int ret = getAddress( obj_name, var_name, a, b, ln );
	if( !ret )
		commands.Add( ln, LPC_PUSH, a, b );
	else
	{
		setError();
		PushFloat( 0.0, ln );
	}
}

///////////////////////////////////////////////////////////////////////////////

#endif
