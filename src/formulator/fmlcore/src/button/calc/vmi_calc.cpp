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

#include "HUtils/idrawstruct.h"
#include "HUtils/ihkarray.h"

#include "vmi_calc.h"

///////////////////////////////////////////////////////////////////////////////

void FBtnCalc::debug_print( QString& str )
{
	str = _T("BEGIN .CODE\n");
	for( long i = 0; i < (long) size(); i++ )
	{
		if( at(i).command >= LPC_NONE && at(i).command < LPC_LAST )
			str += FBtnPiCode_Command_Str[ at(i).command ];
		else
			str += _T("<unknown>");
		str += QString(" args = (%1, %2) value = %3 line = %4\n").
					arg( at(i).frame_var.first).arg( at(i).frame_var.second).arg( at(i).val, 0, 'g', 1 ).arg( at(i).line );
	}
	str += _T("END .CODE\n");
}

void FBtnCalc::operator = ( const FBtnCalc& ls )
{
	(QVector<FBtnPiCode>&) *this = (const QVector<FBtnPiCode>&) ls;
	locals = ls.locals;
}

int FBtnCalc::PositionRun( QVector<RealFde>& frames_ltrb, QVector<RealFde>& frames_margin_ltrb, QVector<RealFde>& primitive_ltrb )
{
	int ret = 0;

	//////////////////////////////
	// Use Calculation Scheme
	//////////////////////////////

	//QVector<double> ev_stack;
	MArray<double> ev_stack(64, 0, 64);
	long ev_stack_top = 0;
	FBtnPiCode *cmd;

	for( long i = 0; i < size(); i++ )
	{
		cmd = &((*this)[i]);
		switch( cmd->command )
		{
		case LPC_ASIGN:
			if( isLocalAddress( cmd->frame_var.first ) )
			{
				ret = isParentMarginAddr( cmd->frame_var.second );
				locals[ cmd->frame_var.second ] = (RealFde) ev_stack[ --ev_stack_top ];
			}
			else if( isPrimitiveAddress( cmd->frame_var.first ) )
				primitive_ltrb[ (- cmd->frame_var.first - 1) * 4 + cmd->frame_var.second ] = (RealFde) ev_stack[ --ev_stack_top ];
			else if( isFrameAddress( cmd->frame_var.first ) && isMargin( cmd->frame_var.second ) )
			{
				frames_margin_ltrb[ (cmd->frame_var.first - 1) * 4 + (cmd->frame_var.second + 4) ] = (RealFde) ev_stack[ --ev_stack_top ];
				ret = 1;
			}
			break;
		case LPC_POP:
			--ev_stack_top;
			break;
		case LPC_PUSH:
			if( isFrameAddress( cmd->frame_var.first ) )
			{
				if( isMargin( cmd->frame_var.second ) )
					ev_stack[ ev_stack_top++ ] = 
						frames_margin_ltrb[ (cmd->frame_var.first - 1) * 4 + (cmd->frame_var.second + 4) ];
				else
					ev_stack[ ev_stack_top++ ] = 
						frames_ltrb[ (cmd->frame_var.first - 1) * 4 + cmd->frame_var.second ];
			}
			else if( isLocalAddress( cmd->frame_var.first ) )
				ev_stack[ ev_stack_top++ ] = locals[ cmd->frame_var.second ];
			else if( isPrimitiveAddress( cmd->frame_var.first ) )
				ev_stack[ ev_stack_top++ ] = primitive_ltrb[ (- cmd->frame_var.first - 1) * 4 + cmd->frame_var.second ];
			break;
		case LPC_PUSHVAL:
			ev_stack[ ev_stack_top++ ] = cmd->val;
			break;
		case LPC_MINUS:
			ev_stack[ ev_stack_top - 1 ] = - ev_stack[ ev_stack_top - 1 ];
			break;
		case LPC_ADD:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = ev_stack[ ev_stack_top - 1 ] + ev_stack[ ev_stack_top ];
			break;
		case LPC_SUB:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = ev_stack[ ev_stack_top - 1 ] - ev_stack[ ev_stack_top ];
			break;
		case LPC_MUL:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = ev_stack[ ev_stack_top - 1 ] * ev_stack[ ev_stack_top ];
			break;
		case LPC_DIV:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = ev_stack[ ev_stack_top - 1 ] / ev_stack[ ev_stack_top ];
			break;
		case LPC_AVERAGE:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = (ev_stack[ ev_stack_top - 1 ] + ev_stack[ ev_stack_top ]) / 2.0;
			break;
		case LPC_MIN:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = ev_stack[ ev_stack_top - 1 ] > ev_stack[ ev_stack_top ] ?
					ev_stack[ ev_stack_top ] : ev_stack[ ev_stack_top - 1 ];
			break;
		case LPC_MAX:
			ev_stack_top--;
			ev_stack[ ev_stack_top - 1 ] = ev_stack[ ev_stack_top - 1 ] > ev_stack[ ev_stack_top ] ?
					ev_stack[ ev_stack_top - 1 ] : ev_stack[ ev_stack_top ];
			break;
		default:
			break;
		}
	}

	return ret;
}

///////////////////////////////////////////////////////////////////////////////
