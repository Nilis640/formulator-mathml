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

#if !defined( __BULAN_VMI_PICODE_H__ )
#define __BULAN_VMI_PICODE_H__

#include <QPair>
#include <QDataStream>

#include "../../fmlcore_global.h"
#include "HUtils/ihkstring.h"

///////////////////////////////////////////////////////////////////////////////
// Button Language VM defines

enum FBtnPiCode_Command
{
	LPC_NONE = 0,

	LPC_ASIGN,	// = POP with asignment
	LPC_POP,
	LPC_PUSH,
	LPC_PUSHVAL,
	LPC_MINUS,
	LPC_ADD,
	LPC_SUB,
	LPC_MUL,
	LPC_DIV,
	LPC_ABS,		// 1 arg
	LPC_AVERAGE,	// 2 args
	LPC_MIN,		// 2 args
	LPC_MAX,		// 2 args

	LPC_LAST,
};

extern QString FBtnPiCode_Command_Str[ LPC_LAST ];

enum FBtnVM_Operation
{
	ASTO_Add = LPC_ADD,
	ASTO_Sub = LPC_SUB,
	ASTO_Mul = LPC_MUL,
	ASTO_Div = LPC_DIV,
	ASTO_Minus = LPC_MINUS,
	ASTO_Last
};

enum FBtnVM_Function
{
	ASTOF_Abs = LPC_ABS,
	ASTOF_Average = LPC_AVERAGE,
	ASTOF_Min = LPC_MIN,
	ASTOF_Max = LPC_MAX,
	ASTOF_Last
};

///////////////////////////////////////////////////////////////////////////////
// Button Language classes

struct __HERMITECH_FORMULIB__ FBtnPiCode
{
	qint32 line;
	double val;
	QPair<qint32, qint32> frame_var;
	enum FBtnPiCode_Command command;

	FBtnPiCode( enum FBtnPiCode_Command c = LPC_NONE, long fr_no = 0, long v_no = 0 );
	FBtnPiCode( enum FBtnPiCode_Command c, double aval );
	FBtnPiCode( long ln, enum FBtnPiCode_Command c = LPC_NONE, long fr_no = 0, long v_no = 0 );
	FBtnPiCode( long ln, enum FBtnPiCode_Command c, double aval );
	void operator=( const FBtnPiCode& ls );

	int operator == ( const FBtnPiCode& lpic ) const
	{
		return line == lpic.line && val == lpic.val && 
				frame_var.first == lpic.frame_var.first && 
				frame_var.second == lpic.frame_var.second && command == lpic.command;
	}
};

/////////////////////////////////////////////////////////////////////

inline
QDataStream& operator << ( QDataStream& ar, const FBtnPiCode& lp )
{
	qint32 v = lp.command;
	return ar << lp.line << lp.val << lp.frame_var << v;
}

inline
QDataStream& operator >> ( QDataStream& ar, FBtnPiCode& lp )
{
	qint32 v;
	QDataStream& ret = ar >> lp.line >> lp.val >> lp.frame_var >> v;
	lp.command = (enum FBtnPiCode_Command) v;
	return ret;
}

/////////////////////////////////////////////////////////////////////

inline
FBtnPiCode::FBtnPiCode( enum FBtnPiCode_Command c, long fr_no, long v_no )
{
	command = c;
	val = 0;
	frame_var.first = fr_no;
	frame_var.second = v_no;
	line = 0;
}

inline
FBtnPiCode::FBtnPiCode( enum FBtnPiCode_Command c, double aval )
{
	command = c;
	frame_var.first = 0;
	frame_var.second = 0;
	val = aval;
	line = 0;
}

inline
FBtnPiCode::FBtnPiCode( long ln, enum FBtnPiCode_Command c, long fr_no, long v_no )
{
	command = c;
	val = 0;
	frame_var.first = fr_no;
	frame_var.second = v_no;
	line = ln;
}

inline
FBtnPiCode::FBtnPiCode( long ln, enum FBtnPiCode_Command c, double aval )
{
	command = c;
	frame_var.first = 0;
	frame_var.second = 0;
	val = aval;
	line = ln;
}

///////////////////////////////////////////////////////////////////////////////

#endif
